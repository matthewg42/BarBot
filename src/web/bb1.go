
package main

import (
  "fmt"
  "html/template"
  "net/http"
  "database/sql"
  _ "github.com/mattn/go-sqlite3"
  "time"
  "strings"
  "github.com/tarm/goserial"
  "io"
  "strconv"
  "flag"
)

const ORDER_FMT = "%05d"


type Recipe struct {
  Id   int
  Name string
}

type DrinksMenu struct {
  Title     string
  Recipes   []Recipe
}

type MenuItemIngredient struct {
  Id      int
  Name    string
  ActQty  int
  UoM     string
}

type MenuItem struct {
  Id          int
  DrinkName   string
  Ingredients []MenuItemIngredient
}

type OrderLogged struct {
 // Id int
  OrderId string
}

type OrderDetails struct {
  DrinkName string
  Alcohol   bool
  IdCheck   bool
  OrderRef  string
  OrderRefs []string  // list of order refs for order selection list on left of screen
}

type DispenserIngredients struct {
  Id int
  Name string
  Current bool // Currently selected
}

type DispenserDetails struct {
  Id  int
  Name string
  Ingredients []DispenserIngredients
}

type AdminRecipeIngr struct {
  Id    int
  Name  string
  qty   int
  UoM   string
}

type AdminRecipe struct {
  RecipieName     string
  RecipieId       int
  Recipes         []Recipe
  AllIngredients  []AdminRecipeIngr  // All known ingrediants for "Add" listbox
  RecIngredients  []AdminRecipeIngr  // Ingrediants in currently selected receipe
}



const (
  DISPENSER_OPTIC    = 1
  DISPENSER_MIXER    = 2 
  DISPENSER_DASHER   = 3
  DISPENSER_SYRINGE  = 4
  DISPENSER_CONVEYOR = 5
  DISPENSER_STIRRER  = 6
  DISPENSER_SLICE    = 7
  DISPENSER_UMBRELLA = 8
)


var MakeDrinkChan chan int

// showMenu displays the list of available drinks to the user
func showMenu(db *sql.DB, w http.ResponseWriter) {

      // Load drinks - only show those that can currently be made
      rows, err := db.Query(
         `select r.id, r.name 
          from recipe r
          where not exists 
          (
            select null
            from recipe r2
            inner join recipe_ingredient ri on r2.id = ri.recipe_id
            left outer join dispenser d on cast(d.ingredient_id as integer) = cast(ri.ingredient_id as integer)
            where d.id is null
            and r2.id = r.id
          )`)
      if err != nil {
        // TODO
        panic(fmt.Sprintf("%v", err))
      }
      defer rows.Close()

      var recipes []Recipe
      for rows.Next() {
        var recipe Recipe
        rows.Scan(&recipe.Id, &recipe.Name)
        recipes = append(recipes, recipe)
      }
      rows.Close()

      menu := DrinksMenu{"Drinks", recipes}

      t, _ := template.ParseFiles("menu.html")
      t.Execute(w, menu)
}

// showMenuItem shows details of a  In   int // current ingrediant drink selected from the menu (ingredients, etc)
func showMenuItem(db *sql.DB, w http.ResponseWriter, r *http.Request) {
      var menuitem MenuItem
      drink_id := r.URL.Path[len("/menu/"):]

      // Get basic receipe information
      row := db.QueryRow("select id, name from recipe where id = ?", drink_id)
      err := row.Scan(&menuitem.Id, &menuitem.DrinkName)
      if err == sql.ErrNoRows {
        http.NotFound(w, r)
        return
      }

      // Get details of the ingredients
      sql := `
        select 
          i.id,
          i.name, 
          ri.qty * dt.unit_size as act_act, 
          case when ri.qty = 1 then dt.unit_name else dt.unit_plural end as uom
        from recipe r
        inner join recipe_ingredient ri on ri.recipe_id = r.id
        inner join ingredient i on i.id = ri.ingredient_id
        inner join dispenser_type dt on dt.id = i.dispenser_type_id
        where r.id = ?`

      rows, err := db.Query(sql, drink_id)
      if err != nil {
        // TODO
        panic(fmt.Sprintf("%v", err))
      }
      defer rows.Close()

      for rows.Next() {
        var ingr MenuItemIngredient
        rows.Scan(&ingr.Id, &ingr.Name, &ingr.ActQty, &ingr.UoM )
        menuitem.Ingredients = append(menuitem.Ingredients, ingr)
      }

      t, _ := template.ParseFiles("menu_item.html")
      t.Execute(w, menuitem)
}


// drinksMenuHandler handles request to "/menu/[n]" - either showing all the drinks available, or details on the selected drink
func drinksMenuHandler(w http.ResponseWriter, r *http.Request) {

    // Open database
    db := getDBConnection()
    defer db.Close()
    
    if len(r.URL.Path) <= len("/menu/") {
      showMenu(db, w)
    } else {
      showMenuItem(db, w, r)
    }
}

func adminHandler(w http.ResponseWriter, r *http.Request) {
  
  // Default admin page is dispenser config for now. So if no subpage is specified, redirect to that
  if (r.URL.Path == "/admin") || (r.URL.Path == "/admin/") {
    http.Redirect(w, r, "/admin/dispenser/", http.StatusSeeOther)
    return
  }
  
  req_page := r.URL.Path[len("/admin/"):]
  
  switch {
    case strings.HasPrefix(req_page, "dispenser/"):
      adminDispenser(w, r, req_page[len("dispenser/"):])
      return;

    case strings.HasPrefix(req_page, "recipe/"):
      adminRecipe(w, r, req_page[len("recipe/"):])
      return;

    default:
      http.NotFound(w, r)
      return
  }
}


// adminRecipe allows a recipe to be added / amended
func adminRecipe(w http.ResponseWriter, r *http.Request, param string) {

  tmpl, _ := template.ParseFiles("admin_header.html", "admin_recipe.html", "admin_footer.html")

  // Open database
  db := getDBConnection()
  defer db.Close()

  if (param == "add_drink") {
    // returned form is receipe_name=<drink name entered>
    r.ParseForm()

    _, err := db.Exec("insert into recipe (name) values (?)", r.Form.Get("new_drink_name"))
    if err != nil {
      panic(fmt.Sprintf("Failed to update db: %v", err))
    }
    
    http.Redirect(w, r, "/admin/recipe/", http.StatusSeeOther)
    return
  }
  
  var adminR AdminRecipe
  
  // Get a list of all drinks for list box
  rows, err := db.Query("select r.id, r.name  from recipe r order by r.name")
  if err != nil {
    panic(fmt.Sprintf("%v", err))
  }
  defer rows.Close()

  for rows.Next() {
    var recipe Recipe
    rows.Scan(&recipe.Id, &recipe.Name)
    adminR.Recipes = append(adminR.Recipes, recipe)
  }
  rows.Close()
 
  // Get a list of all ingrediants for the "add" list box
  rows, err = db.Query("select i.id, i.name from ingredient i order by i.name")
  if err != nil {
    panic(fmt.Sprintf("%v", err))
  }
  defer rows.Close()

  for rows.Next() {
    var recipeIngr AdminRecipeIngr
    rows.Scan(&recipeIngr.Id, &recipeIngr.Name)
    adminR.AllIngredients = append(adminR.AllIngredients, recipeIngr)
  }
  rows.Close()
  
  // Get a list of all ingrediants in the currently selected drink
  recipe_id, err := strconv.Atoi(r.Form.Get("new_drink_name"))
  if err != nil {
    recipe_id = -1  
  } 
  
  sqlstr :=  
    `   select
          i.id,  
          i.name,
          ri.qty,
          case when ri.qty = 1 then dt.unit_name else dt.unit_plural end as uom
        from recipe_ingredient ri
        inner join ingredient i on ri.ingredient_id = i.id
        inner join dispenser_type dt on dt.id = i.dispenser_type_id
        where ri.recipe_id = ?
        order by ri.seq`
        
  rows, err = db.Query(sqlstr, recipe_id)
  if err != nil {
    panic(fmt.Sprintf("%v", err))
  }
  defer rows.Close()

  for rows.Next() {
    var recipeIngr AdminRecipeIngr
    rows.Scan(&recipeIngr.Id, &recipeIngr.Name)
    adminR.RecIngredients = append(adminR.RecIngredients, recipeIngr)
  }
  rows.Close()
   

  
  tmpl.ExecuteTemplate(w, "admin_header", nil)
  tmpl.ExecuteTemplate(w, "admin_recipe", adminR)
  tmpl.ExecuteTemplate(w, "admin_footer", nil)
  return
}

 
// adminDispenser shows the despenser selection page of the admin interface
func adminDispenser(w http.ResponseWriter, r *http.Request, param string) {

  tmpl, _ := template.ParseFiles("admin_header.html", "admin_dispenser.html", "admin_footer.html")

  // Open database
  db := getDBConnection()
  defer db.Close()

  if (param == "update") {
    // returned form is dispenser_id=ingredient_id
    r.ParseForm()

    for dispenser_id, ingredient_id := range r.Form {
      _, err := db.Exec(
              "update dispenser set ingredient_id = ? where id = ?",
              ingredient_id[0],
              dispenser_id,
      )
      if err != nil {
        panic(fmt.Sprintf("Failed to update db: %v", err))
      }

    }

    http.Redirect(w, r, "/admin/dispenser/", http.StatusSeeOther)
    return
  }

  var dispensers = make([]DispenserDetails,21) // TODO: Do not hard code number of dispenersers...

  // Get a list of all dispensers, possible ingrediants and current ingrediant
  sql := `
    select
      d.id as dispenser_id,
      d.name as dispenser_name,
      case when d.ingredient_id = i.id then 1 else 0 end as current,
      i.id as ingredient_id,
      i.name as ingredient_name
    from dispenser d 
    left outer join ingredient i on d.dispenser_type_id = i.dispenser_type_id
    order by d.id, i.name
  `

  rows, err := db.Query(sql)
  if err != nil {
    // TODO
    panic(fmt.Sprintf("%v", err))
  }
  defer rows.Close()

  for rows.Next() {
    var ingr DispenserIngredients
    var dispenser_id int
    var dispenser_name string
    var current int
      
    rows.Scan(&dispenser_id, &dispenser_name, &current, &ingr.Id, &ingr.Name)
    if current==1 {
      ingr.Current = true
    } else {
      ingr.Current = false
    }
    dispensers[dispenser_id].Ingredients = append(dispensers[dispenser_id].Ingredients, ingr)
    dispensers[dispenser_id].Name = dispenser_name
    dispensers[dispenser_id].Id = dispenser_id
  }
  
  tmpl.ExecuteTemplate(w, "admin_header", nil)
  tmpl.ExecuteTemplate(w, "admin_dispenser", dispensers)
  tmpl.ExecuteTemplate(w, "admin_footer", nil)
  return
}

// orderListHandler handles requests to /orderlist/
func orderListHandler(w http.ResponseWriter, r *http.Request) {

    // Open database
    db := getDBConnection()
    defer db.Close()


    sqlstr := "select id from drink_order where cancelled = 0"

    rows, err := db.Query(sqlstr)
    if err != nil {
      // TODO
      panic(fmt.Sprintf("%v", err))
    }
    defer rows.Close()

    var orderdetails OrderDetails
    for rows.Next() {
      var id int
      rows.Scan(&id)
      orderdetails.OrderRefs = append(orderdetails.OrderRefs, fmt.Sprintf(ORDER_FMT, id))
    }

    if len(r.URL.Path) > len("/orderlist/") {

      // Check if user has clicked on a on order
      var p string = r.URL.Path[len("/orderlist/"):] 
      switch  {
        case strings.HasPrefix(p, "remove/"):
          removeOrder(db, w, r, p[len("remove/"):])
          http.Redirect(w, r, "/orderlist", http.StatusSeeOther)
          return

        case strings.HasPrefix(p, "make/"):
          if !makeOrder(db, w, r, p[len("make/"):]) {
            http.NotFound(w, r)
          }
          return
      }

      // Assume order ref passed in (->404 if not), so also get the details of that order
      orderdetails.OrderRef = r.URL.Path[len("/orderlist/"):]

      sqlstr = `
        select
          do.alcohol,
          do.id_checked,
          r.name
        from drink_order do
        inner join recipe r on do.recipe_id = r.id
        where do.id = ?`

      row := db.QueryRow(sqlstr, orderdetails.OrderRef)

      err := row.Scan(&orderdetails.Alcohol, &orderdetails.IdCheck, &orderdetails.DrinkName)
      if err == sql.ErrNoRows {
        http.NotFound(w, r)
        return
      } else {
        if err != nil {
          panic(fmt.Sprintf("orderListHandler - failed to get order details: %#v", err))
        }
      }
    }

    t, _ := template.ParseFiles("order_list.html")
    t.Execute(w, orderdetails)

}

// removeOrder is called when an order is selected and "remove" clicked. In reality it actaully cancels, not deletes, it.
func removeOrder(db *sql.DB, w http.ResponseWriter, r *http.Request, p string) bool {
  
  sqlstr := `
    update drink_order 
    set cancelled = ?
    where id = ?
      and made_start_ts is null
      and made_end_ts is null`
      
  _, err := db.Exec(sqlstr, true, p)

  if err != nil {
    panic(fmt.Sprintf("removeOrder failed: %#v", err))
  }
  return false
}


func makeOrder(db *sql.DB, w http.ResponseWriter, r *http.Request, p string) bool {
  // TODO - don't block (error instead)
  //      - update database 
  
  drink_order_id, err := strconv.Atoi(p)
  if err != nil {
    return false   
  } 
  
  MakeDrinkChan <- drink_order_id
  return true
}


func recipeContainsAlcohol(tx *sql.Tx, recipe_id string) bool {

  sql := `
        select 
          count(*)
        from recipe r
        inner join recipe_ingredient ri on ri.recipe_id = r.id
        inner join ingredient i on i.id = ri.ingredient_id
        where r.id = ?
          and alcoholic = 1`
  
  var alcoholic int
  row := tx.QueryRow(sql, recipe_id)

  err := row.Scan(&alcoholic)
  if err != nil {
    panic(fmt.Sprintf("recipeContainsAlcohol failed: %v", err))
    return true
  }
  
  if alcoholic > 0 {
    return true
  } else {
    return false
  }

}

func orderDrinkHandler(w http.ResponseWriter, r *http.Request) {
  
    var err error
    var db *sql.DB
    
    if len(r.URL.Path) <= len("/order/") {
      http.NotFound(w, r)
      return
    } else {
       // Open database
      db = getDBConnection()
   }
   defer db.Close()
   tx, _ := db.Begin()
   defer tx.Rollback()

   recipe_id := r.URL.Path[len("/order/"):]

   // Check drink is known
   var menuitem MenuItem
   row := tx.QueryRow("select id, name from recipe where id = ?", recipe_id)
   err = row.Scan(&menuitem.Id, &menuitem.DrinkName)
   if err == sql.ErrNoRows {
     http.NotFound(w, r)
     return
   }

   alcoholic := recipeContainsAlcohol(tx, recipe_id)

   // Generate order
   _, insertErr := tx.Exec(
     "insert into drink_order (create_ts, recipe_id, alcohol, id_checked, cancelled) VALUES (?, ?, ?, ?, ?)",
     int32(time.Now().Unix()),
     recipe_id,
     alcoholic,
     false,
     false,
   )
   if insertErr != nil {
     panic(fmt.Sprintf("Insert order failed: %v", insertErr))
   }

   var orderLogged OrderLogged

    // Order reference (id)
    row = tx.QueryRow("select max(id) from drink_order")
    var order_id int
    err = row.Scan(&order_id)
    orderLogged.OrderId = fmt.Sprintf(ORDER_FMT, order_id)
    if err == sql.ErrNoRows {
      http.NotFound(w, r)
      return
    }
    tx.Commit()
    t, _ := template.ParseFiles("order_logged.html")
    t.Execute(w, orderLogged)
  }


// getDBConnection opens and returns a database connection
func getDBConnection() *sql.DB {
  // Open database
  db, err := sql.Open("sqlite3", "db.sqlite3")
  if err != nil {
    // TODO
    panic(fmt.Sprintf("%#v", err))
  }
  return db
}

// BBSerial goroutine manages serial communications with barbot
func BBSerial(c chan int, serialPort string) {
  
  // Open serial port
  port := &serial.Config{Name: serialPort, Baud: 115200} 
  s, err := serial.OpenPort(port)
  if err != nil {
    panic(fmt.Sprintf("BBSerial failed to open serial port: %v", err))
  }
  
  for {
    select {
      case drink_order_id := <-c:
        SendDrink(drink_order_id, s)
    }
  }
  
}

// getCommandList takes a drink_order_id, and returns a set of insturctions to be sent to barbot to make it
func getCommandList(drink_order_id int) ([]string, int) {
/*
 * Instructions generated:
 *   M nnnnn               - move to rail position nnnnn
 *   D nn xxxx             - Dispense using dispenser nn, with parameter xxxx
 * 
 */
  
   db := getDBConnection()
   defer db.Close()
 
   // Get a list of ingrediants required
   sqlstr := `select 
                i.id,
                ri.qty,
                i.dispenser_param,
                dt.id,
                dt.unit_size
              from drink_order do
              inner join recipe r on r.id = do.recipe_id
              inner join recipe_ingredient ri on ri.recipe_id = r.id
              inner join ingredient i on i.id = ri.ingredient_id
              inner join dispenser_type dt on dt.id = i.dispenser_type_id
              where do.id = ?
              order by ri.seq`

  rows, err := db.Query(sqlstr, drink_order_id)
  if err != nil {
    panic(fmt.Sprintf("%v", err))
  }
  defer rows.Close()

  
  commandList := make([]string, 1)
  
  // Clear any previous instructions
  commandList = append(commandList, fmt.Sprintf("C"))
  
  for rows.Next() {
    var ingredient_id int
    var qty int
    var dispenser_param int
    var dispenser_type int
    var unit_size int
      
    rows.Scan(&ingredient_id, &qty, &dispenser_param, &dispenser_type, &unit_size)
    
    rail_position, dispenser_id := getIngredientPosition(ingredient_id)
    if dispenser_id == -1 {
      return nil, -1
    }

    // move to the correct position
    commandList = append(commandList, fmt.Sprintf("M %d", rail_position))

    // Dispense
    for qty > 0 {
      qty--
      if dispenser_type == DISPENSER_MIXER {
        commandList = append(commandList, fmt.Sprintf("D% d %d", dispenser_id, unit_size))
      } else {
        commandList = append(commandList, fmt.Sprintf("D% d %d", dispenser_id, dispenser_param))
      }
    }
  }
  
  // move to home position when done
  commandList = append(commandList, fmt.Sprintf("M 0"))
  
  // Go!
  commandList = append(commandList, fmt.Sprintf("G"))

  return commandList, 0
}

// getIngredientPosition returns a suitable rail_position and dispenser_id for the requested ingrediant
func getIngredientPosition(ingredient_id int) (int, int) {
  
   db := getDBConnection()
   defer db.Close()
  
  sqlstr := `select d.id, d.rail_position
             from dispenser d
             inner join ingredient i on i.id = d.ingredient_id
             where i.id = ?
             `
  row := db.QueryRow(sqlstr, ingredient_id)

  var dispenser_id int
  var rail_position int

  err := row.Scan(&dispenser_id, &rail_position)
  if err == sql.ErrNoRows {
    fmt.Printf("getIngredientPosition: ingredient_id = %d not found!\n", ingredient_id)
    return -1, -1
  }
  if err != nil {
    panic(fmt.Sprintf("getIngredientPosition failed: %v", err))
  }
  fmt.Printf("getIngredientPosition: ingredient_id=[%d] is on dispenser_id=[%d], position=[%d]\n", ingredient_id, dispenser_id, rail_position)

  return rail_position,dispenser_id
}


// SendDrink looks up the required ingredients for drink_order_id, and transmits the necessary instructions over serial. Or it will do.
func SendDrink(drink_order_id int, s io.ReadWriteCloser) {

  fmt.Printf("SendDrink: making drink_order_id=%d\n", drink_order_id)
  cmdList, ret := getCommandList(drink_order_id)
  
  if ret != 0 {
    fmt.Printf("SendDrink: failed to generate command list!\n")
    return
  }

  for _, cmd := range cmdList {
    fmt.Printf("> %s\n", cmd)
    _, err := s.Write([]byte(fmt.Sprintf("%s\n", cmd)))
    time.Sleep(10 * time.Millisecond) // 10ms delay between each instruction; don't send commands faster than the Arduino can process them
    if err != nil {
      panic(fmt.Sprintf("SendDrink failed to transmit instruction: %v", err))
    }
  }

}

func main() {
  
  var serialPort = flag.String("serial", "/dev/ttyS0", "Serial port to use")
  flag.Parse()
  
  http.HandleFunc("/menu/", drinksMenuHandler)
  http.HandleFunc("/order/", orderDrinkHandler)
  http.HandleFunc("/orderlist/", orderListHandler) // TODO: password protect (e.g. using go-http-auth)
  http.HandleFunc("/admin/", adminHandler)
  http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))
  http.Handle("/", http.FileServer(http.Dir("static")))
  
  MakeDrinkChan = make(chan int);
  go BBSerial(MakeDrinkChan, *serialPort)

  http.ListenAndServe(":8080", nil)
}

