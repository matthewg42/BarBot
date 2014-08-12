-- Determine which cocktails can be made based on the ingredients
-- currently configured in the dispenser table

.mode column
.width 30 6 30 8 20

SELECT      r.name, ri.seq + '.', i.name, ri.qty * dt.unit_size, dt.unit_name
FROM        recipe_ingredient ri,
            recipe r,
            ingredient i,
            dispenser_type dt
WHERE       r.id = ri.recipe_id
AND         ri.ingredient_id = i.id
AND         i.dispenser_type_id = dt.id
AND 		r.id IN (
	SELECT 		DISTINCT r.id
	FROM 		recipe r 
	WHERE 		r.id NOT IN (
		-- sub-select lists IDs of recipes which have missing ingredients
		SELECT 		r.id
		FROM 		recipe_ingredient ri, 
				recipe r, 
				ingredient i 
		WHERE 		r.id = ri.recipe_id
		AND 		ri.ingredient_id = i.id
		AND 		NOT EXISTS (
			SELECT 	1
			FROM	dispenser d
			WHERE	d.ingredient_id = i.id
		)
	)
)
ORDER BY    r.id, ri.seq;

