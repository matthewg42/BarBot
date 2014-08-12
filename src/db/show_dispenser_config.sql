.mode column
.width 4 16 16 32 12 6

SELECT			d.id, d.name, dt.name, i.name, dt.unit_name
FROM			dispenser d,
				dispenser_type dt,
				ingredient i
WHERE			d.dispenser_type_id = dt.id
AND				d.ingredient_id = i.id
ORDER BY		d.id ASC;

