-- Naming conventions:
-- _ts means timestamp

DROP TABLE drink_order;
DROP TABLE recipe_ingredient;
DROP TABLE recipe;
DROP TABLE ingredient;
DROP TABLE dispenser;
DROP TABLE dispenser_type;

CREATE TABLE drink_order (
	id			INTEGER PRIMARY KEY AUTOINCREMENT,
	create_ts		INTEGER NOT NULL,
	recipe_id		REFERENCES recipe(id),
	alcohol			BOOLEAN NOT NULL,
	id_checked		BOOLEAN NOT NULL,
	cancelled		BOOLEAN NOT NULL,
	made_start_ts		INTEGER NULL,
	made_end_ts		INTEGER NULL
);

CREATE TABLE recipe ( 
	id			INTEGER PRIMARY KEY AUTOINCREMENT,
 	name			TEXT NOT NULL
);

CREATE TABLE ingredient (
	id			INTEGER PRIMARY KEY AUTOINCREMENT,
	name			VARCHAR(255) NOT NULL,
	dispenser_type_id	REFERENCES dispenser_type(id),
	dispenser_param 	INTEGER NOT NULL,
	alcoholic		BOOLEAN NOT NULL
);

CREATE TABLE recipe_ingredient ( 
	recipe_id		REFERENCES recipe(id),
	ingredient_id		REFERENCES ingredient(id),
	seq			INTEGER NOT NULL,
	qty			INTEGER NOT NULL,
	PRIMARY KEY ( recipe_id, ingredient_id ),
	UNIQUE (recipe_id, seq)
);

CREATE TABLE dispenser_type (
	id			INTEGER PRIMARY KEY AUTOINCREMENT,
	name			VARCHAR(255) NOT NULL,
	unit_name		VARCHAR(32) NOT NULL,
	unit_plural		VARCHAR(32) NOT NULL,
	unit_size		INTEGER NOT NULL
);

CREATE TABLE dispenser (
	id			INTEGER PRIMARY KEY,
	dispenser_type_id	REFERENCES dispenser_type(id),
	ingredient_id		REFERENCES ingredient(id),
	name			VARCHAR(64),
	rail_position		INTEGER NOT NULL
);

-- test data

INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Optic', 'ml', 'ml', 25);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Mixer tap', 'ml', 'ml', 25);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Dasher', 'dash', 'dashes', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Syringe', 'ml', 'ml', 25);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Conveyor', 'cherry', 'cherries', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Stirrer', 'stir', 'stirs', 1);
INSERT INTO dispenser_type (name, unit_name, unit_plural, unit_size) VALUES ('Slice dispenser', 'slice', 'slices', 1);
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
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Worcestershire sauce', id, 2000, 0 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lime juice', id, 2000, 0 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Angorstura bitters', id, 2000, 1 FROM dispenser_type WHERE name = 'Dasher';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Grenadine', id, 2000, 1 FROM dispenser_type WHERE name = 'Syringe';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Cherry', id, 2000, 1 FROM dispenser_type WHERE name = 'Conveyor';
INSERT INTO ingredient (name, dispenser_type_id, dispenser_param, alcoholic) SELECT 'Lemon', id, 2000, 1 FROM dispenser_type WHERE name = 'Slice dispenser';


INSERT INTO recipe (name) VALUES ('Gin and tonic');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 2 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic' AND i.name = 'Gin'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 4 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic' AND i.name = 'Tonic water'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 3, 1 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic' AND i.name = 'Lime juice'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 4, 1 FROM recipe r, ingredient i WHERE r.name = 'Gin and tonic' AND i.name = 'Lemon'; 

INSERT INTO recipe (name) VALUES ('Tequila sunrise');
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 1, 2 FROM recipe r, ingredient i WHERE r.name = 'Tequila sunrise' AND i.name = 'Tequila'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 2, 4 FROM recipe r, ingredient i WHERE r.name = 'Tequila sunrise' AND i.name = 'Orange juice'; 
INSERT INTO recipe_ingredient (recipe_id, ingredient_id, seq, qty) SELECT r.id, i.id, 3, 1 FROM recipe r, ingredient i WHERE r.name = 'Tequila sunrise' AND i.name = 'Grenadine'; 



