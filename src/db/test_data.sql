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
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Grenadine', id, 1000, 1 FROM dispenser_type WHERE name = 'Syringe';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Cherry', id, 1, 1 FROM dispenser_type WHERE name = 'Conveyor';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lemon', id, 1, 1 FROM dispenser_type WHERE name = 'Slice dispenser';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lime', id, 1, 1 FROM dispenser_type WHERE name = 'Slice dispenser';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Good stir', id, 1, 1 FROM dispenser_type WHERE name = 'Stirrer';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Umbrella', id, 1, 1 FROM dispenser_type WHERE name = 'Umbrella dropper';

-- Ingredients we have loaded into dispensers (also dispenser configs)
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Optic 0',     10 FROM ingredient WHERE name = 'Vodka';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Optic 1',     20 FROM ingredient WHERE name = 'Whisky';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Optic 2',     30 FROM ingredient WHERE name = 'Vodka';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Optic 3',     40 FROM ingredient WHERE name = 'Gin';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Optic 4',     50 FROM ingredient WHERE name = 'Tequila';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Optic 5',     60 FROM ingredient WHERE name = 'Rum';

INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Pressure 0',  70 FROM ingredient WHERE name = 'Ginger ale';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Pressure 1',  76 FROM ingredient WHERE name = 'Coke';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Pressure 2',  82 FROM ingredient WHERE name = 'Lemonade';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Pressure 3',  88 FROM ingredient WHERE name = 'Tonic water';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Pressure 4',  94 FROM ingredient WHERE name = 'Soda water';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Pressure 5', 100 FROM ingredient WHERE name = 'Orange juice';

INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Dasher 0', 106 FROM ingredient WHERE name = 'Angorstura bitters';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Dasher 1', 112 FROM ingredient WHERE name = 'Lemon juice';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Dasher 2', 118 FROM ingredient WHERE name = 'Tabasco';

INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Syringe', 122 FROM ingredient WHERE name = 'Grenadine';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Conveyor', 126 FROM ingredient WHERE name = 'Cherry';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Slice dispenser', 136 FROM ingredient WHERE name = 'Lime';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Stirrer dispenser', 142 FROM ingredient WHERE name = 'Good stir';
INSERT INTO dispenser (dispenser_type_id, ingredient_id, name, rail_position) SELECT dispenser_type_id, id, 'Umbrella dropper', 146 FROM ingredient WHERE name = 'Umbrella';

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

