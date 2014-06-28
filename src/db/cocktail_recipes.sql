SELECT 		r.name, ri.seq + '.', i.name, ri.qty * d.unit_size, d.unit_name
FROM 		recipe_ingredient ri, 
		recipe r, 
		ingredient i, 
		dispenser_type d
WHERE 		r.id = ri.recipe_id
AND 		ri.ingredient_id = i.id
AND 		i.dispenser_type_id = d.id
ORDER BY 	r.id, ri.seq;
