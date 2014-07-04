-- Determine which cocktails can be made based on the ingredients
-- currently configured in the dispenser table

SELECT 		r.id
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
);

