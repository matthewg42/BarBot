SELECT 		r.name, ri.seq + '.', i.name, ri.qty * dt.unit_size, dt.unit_name
FROM 		recipe_ingredient ri, 
		recipe r, 
		ingredient i, 
		dispenser_type dt
WHERE 		r.id = ri.recipe_id
AND 		ri.ingredient_id = i.id
AND 		i.dispenser_type_id = dt.id
ORDER BY 	r.id, ri.seq;
