-- Naming conventions:
-- _ts means timestamp

DROP TABLE drink_order;
DROP TABLE recipe_ingredient;
DROP TABLE recipe;
DROP TABLE ingredient;
DROP TABLE dispenser;
DROP TABLE dispenser_type;

CREATE TABLE drink_order (
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
	create_ts         INTEGER NOT NULL,
	recipe_id         REFERENCES recipe(id),
	alcohol           BOOLEAN NOT NULL,
	id_checked        BOOLEAN NOT NULL,
	cancelled         BOOLEAN NOT NULL,
	made_start_ts     INTEGER NULL,
	made_end_ts       INTEGER NULL
);

CREATE TABLE recipe ( 
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
 	name              TEXT NOT NULL,
	show_in_menu      BOOLEAN NOT NULL DEFAULT FALSE
);

CREATE TABLE ingredient (
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
	name              VARCHAR(255) NOT NULL,
	dispenser_type_id REFERENCES dispenser_type(id),
	dispenser_param   INTEGER NOT NULL,
	alcoholic         BOOLEAN NOT NULL
);

CREATE TABLE recipe_ingredient ( 
	recipe_id         REFERENCES recipe(id),
	ingredient_id     REFERENCES ingredient(id),
	seq               INTEGER NOT NULL,
	qty               INTEGER NOT NULL,
	PRIMARY KEY ( recipe_id, ingredient_id ),
	UNIQUE (recipe_id, seq)
);

CREATE TABLE dispenser_type (
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
	name              VARCHAR(255) NOT NULL,
	unit_name         VARCHAR(32) NOT NULL,
	unit_plural       VARCHAR(32) NOT NULL,
	unit_size         INTEGER NOT NULL
);

CREATE TABLE dispenser (
	id                INTEGER PRIMARY KEY,
	dispenser_type_id REFERENCES dispenser_type(id),
	ingredient_id     REFERENCES ingredient(id),
	name              VARCHAR(64),
	rail_position     INTEGER NOT NULL
);

