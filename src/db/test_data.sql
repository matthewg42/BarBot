-- All of our dispenser types
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Optic', 'ml', 'ml', 25);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Mixer tap', 'ml', 'ml', 25);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Dasher', 'dash', 'dashes', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Syringe', 'ml', 'ml', 25);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Conveyor', 'cherry', 'cherries', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Stirrer', 'stir', 'stirs', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Slice dispenser', 'slice', 'slices', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Umbrella dropper', 'cocktail', 'umbrellas', 1);

-- All possible ingredients will will have availabale
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Vodka', id, 2800, 1 FROM dispenser_type WHERE name = 'Optic';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Gin', id, 3000, 1 FROM dispenser_type WHERE name = 'Optic';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Whisky', id, 3000, 1 FROM dispenser_type WHERE name = 'Optic';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Rum', id, 3500, 1 FROM dispenser_type WHERE name = 'Optic';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Tequila', id, 3000, 1 FROM dispenser_type WHERE name = 'Optic';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Blue Curacao', id, 4000, 0 FROM dispenser_type WHERE name = 'Optic';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Coke', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Tonic water', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Soda water', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lemonade', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Bitter lemon', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Ginger ale', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Orange juice', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Pineapple juice', id, 2000, 0 FROM dispenser_type WHERE name = 'Mixer tap';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Worcestershire sauce', id, 1, 0 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Tabasco', id, 1, 0 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lemon juice', id, 1, 0 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lime juice', id, 1, 0 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Angorstura bitters', id, 1, 1 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Grenadine', id, 1000, 0 FROM dispenser_type WHERE name = 'Syringe';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Cherry', id, 1, 0 FROM dispenser_type WHERE name = 'Conveyor';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lemon', id, 1, 0 FROM dispenser_type WHERE name = 'Slice dispenser';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lime', id, 1, 0 FROM dispenser_type WHERE name = 'Slice dispenser';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Good stir', id, 1, 0 FROM dispenser_type WHERE name = 'Stirrer';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Umbrella', id, 1, 0 FROM dispenser_type WHERE name = 'Umbrella dropper';

-- Ingredients we have loaded into dispensers (also dispenser configs)
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  1, dispenser_type_id, id, 'Optic 0',             0 FROM ingredient WHERE name = 'Vodka';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  2, dispenser_type_id, id, 'Optic 1',           546 FROM ingredient WHERE name = 'Whisky';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  3, dispenser_type_id, id, 'Optic 2',          1093 FROM ingredient WHERE name = 'Vodka';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  4, dispenser_type_id, id, 'Optic 3',          1640 FROM ingredient WHERE name = 'Gin';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  5, dispenser_type_id, id, 'Optic 4',          2187 FROM ingredient WHERE name = 'Tequila';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  6, dispenser_type_id, id, 'Optic 5',          2734 FROM ingredient WHERE name = 'Rum';

INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  7, dispenser_type_id, id, 'Pressure 0',       3117 FROM ingredient WHERE name = 'Ginger ale';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  8, dispenser_type_id, id, 'Pressure 1',       3417 FROM ingredient WHERE name = 'Coke';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT  9, dispenser_type_id, id, 'Pressure 2',       3746 FROM ingredient WHERE name = 'Lemonade';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 10, dispenser_type_id, id, 'Pressure 3',       4057 FROM ingredient WHERE name = 'Tonic water';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 11, dispenser_type_id, id, 'Pressure 4',       4357 FROM ingredient WHERE name = 'Soda water';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 12, dispenser_type_id, id, 'Pressure 5',       4714 FROM ingredient WHERE name = 'Orange juice';

INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 13, dispenser_type_id, id, 'Dasher 0',         5058 FROM ingredient WHERE name = 'Angorstura bitters';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 14, dispenser_type_id, id, 'Dasher 1',         5332 FROM ingredient WHERE name = 'Lemon juice';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 15, dispenser_type_id, id, 'Dasher 2',         5550 FROM ingredient WHERE name = 'Tabasco';

INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 16, dispenser_type_id, id, 'Syringe' ,         6150 FROM ingredient WHERE name = 'Grenadine';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 17, dispenser_type_id, id, 'Conveyor',         5900 FROM ingredient WHERE name = 'Cherry';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 18, dispenser_type_id, id, 'Slice dispenser',  6525 FROM ingredient WHERE name = 'Lime';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 19, dispenser_type_id, id, 'Stirrer dispenser',6825 FROM ingredient WHERE name = 'Good stir';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 20, dispenser_type_id, id, 'Umbrella dropper', 7080 FROM ingredient WHERE name = 'Umbrella';

-- Cocktail recipes!
INSERT INTO recipe (name) VALUES ('Gin and tonic (lemon lime)');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 2 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lemon lime)' AND i.name = 'Gin'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 4 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lemon lime)' AND i.name = 'Tonic water'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 3, 1 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lemon lime)' AND i.name = 'Lime juice'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 4, 1 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lemon lime)' AND i.name = 'Lemon'; 

INSERT INTO recipe (name) VALUES ('Gin and tonic (lime lemon)');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 2 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lime lemon)' AND i.name = 'Gin'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 4 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lime lemon)' AND i.name = 'Tonic water'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 3, 1 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lime lemon)' AND i.name = 'Lemon juice'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 4, 1 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic (lime lemon)' AND i.name = 'Lime'; 

INSERT INTO recipe (name) VALUES ('Tequila sunrise');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 2 FROM recipe r, ingredient i WHERE r.name = 'Tequila sunrise' AND i.name = 'Tequila'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 4 FROM recipe r, ingredient i WHERE r.name = 'Tequila sunrise' AND i.name = 'Orange juice'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 3, 1 FROM recipe r, ingredient i WHERE r.name = 'Tequila sunrise' AND i.name = 'Grenadine'; 

INSERT INTO recipe (name) VALUES ('Highball');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 1 FROM recipe r, ingredient i WHERE r.name = 'Highball' AND i.name = 'Whisky'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 3 FROM recipe r, ingredient i WHERE r.name = 'Highball' AND i.name = 'Ginger ale'; 

INSERT INTO recipe (name) VALUES ('Whisky soda');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 1 FROM recipe r, ingredient i WHERE r.name = 'Whisky soda' AND i.name = 'Whisky'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 3 FROM recipe r, ingredient i WHERE r.name = 'Whisky soda' AND i.name = 'Soda water'; 

INSERT INTO recipe (name) VALUES ('Vodka and coke');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 1 FROM recipe r, ingredient i WHERE r.name = 'Vodka and coke' AND i.name = 'Vodka'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 3 FROM recipe r, ingredient i WHERE r.name = 'Vodka and coke' AND i.name = 'Coke'; 

