
package main

import (
  "fmt"
  "html/template"
  "net/http"
  "database/sql"
  "time"
  "strings"
  _ "github.com/mattn/go-sqlite3"
  "github.com/tarm/goserial"
  "io"
  "strconv"
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

var MakeDrinkChan chan int

// showMenu displays the list of available drinks to the user
// TODO: limit display to only those drinks that can currently be made
func showMenu(db *sql.DB, w http.ResponseWriter) {

      // Load drinks
      rows, err := db.Query("select id, name from recipe")
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

// showMenuItem shows details of a drink selected from the menu (ingredients, etc)
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
  
  drink_id, err := strconv.Atoi(p)
  if err != nil {
    return false   
  } 
  
  MakeDrinkChan <- drink_id
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

   drink_id := r.URL.Path[len("/order/"):]

   // Check drink is known
   var menuitem MenuItem
   row := tx.QueryRow("select id, name from recipe where id = ?", drink_id)
   err = row.Scan(&menuitem.Id, &menuitem.DrinkName)
   if err == sql.ErrNoRows {
     http.NotFound(w, r)
     return
   }

   alcoholic := recipeContainsAlcohol(tx, drink_id)

   // Generate order
   _, insertErr := tx.Exec(
     "insert into drink_order (create_ts, recipe_id, alcohol, id_checked, cancelled) VALUES (?, ?, ?, ?, ?)",
     int32(time.Now().Unix()),
     drink_id,
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
func BBSerial(c chan int) {
  
  // Open serial port
  port := &serial.Config{Name: "/dev/ttyS0", Baud: 115200} // TODO: belongs in a config file or maybe database
  s, err := serial.OpenPort(port)
  if err != nil {
    panic(fmt.Sprintf("BBSerial failed to open serial port: %v", err))
  }
  
  for {
    select {
      case drink_id := <-c:
        SendDrink(drink_id, s)
    }
  }
  
}


// SendDrink looks up the required ingredients for drink_id, and transmits the necessary instructions over serial. Or it will do.
func SendDrink(drink_id int, s io.ReadWriteCloser) {
  // TODO
  
  _, err := s.Write([]byte(fmt.Sprintf("MAKE DRINK %d\n", drink_id)))
  if err != nil {
    panic(fmt.Sprintf("SendDrink failed to transmit instruction: %v", err))
  }
}

func main() { 
  http.HandleFunc("/menu/", drinksMenuHandler)
  http.HandleFunc("/order/", orderDrinkHandler)
  http.HandleFunc("/orderlist/", orderListHandler) // TODO: password protect (e.g. using go-http-auth)
  http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))
  http.Handle("/", http.FileServer(http.Dir("static")))
  
  MakeDrinkChan = make(chan int);
  go BBSerial(MakeDrinkChan)

  http.ListenAndServe(":8080", nil)
}

