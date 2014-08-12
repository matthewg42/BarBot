DELETE FROM  dispenser;

INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 1, dispenser_type_id, id, 'Optic 0', 0 FROM ingredient WHERE name = 'Tequila';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 2, dispenser_type_id, id, 'Optic 1', 546 FROM ingredient WHERE name = 'Vodka';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 3, dispenser_type_id, id, 'Optic 2', 1093 FROM ingredient WHERE name = 'Gin';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 4, dispenser_type_id, id, 'Optic 3', 1640 FROM ingredient WHERE name = 'Whisky';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 5, dispenser_type_id, id, 'Optic 4', 2187 FROM ingredient WHERE name = 'Peach Schnapps';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 6, dispenser_type_id, id, 'Optic 5', 2734 FROM ingredient WHERE name = 'Bourbon';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 7, dispenser_type_id, id, 'Mixer 0', 3117 FROM ingredient WHERE name = 'Orange Juice';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 8, dispenser_type_id, id, 'Mixer 1', 3417 FROM ingredient WHERE name = 'Pineapple Juice';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 9, dispenser_type_id, id, 'Mixer 2', 3746 FROM ingredient WHERE name = 'Cranberry Juice';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 10, dispenser_type_id, id, 'Mixer 3', 4057 FROM ingredient WHERE name = 'Tonic Water';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 11, dispenser_type_id, id, 'Mixer 4', 4357 FROM ingredient WHERE name = 'Soda Water';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 12, dispenser_type_id, id, 'Mixer 5', 4714 FROM ingredient WHERE name = 'Cola';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 13, dispenser_type_id, id, 'Dasher 0', 5058 FROM ingredient WHERE name = 'Angostura Bitters';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 14, dispenser_type_id, id, 'Dasher 1', 5332 FROM ingredient WHERE name = 'Lemon Juice';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 15, dispenser_type_id, id, 'Dasher 2', 5550 FROM ingredient WHERE name = 'Agave Syrup';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 16, dispenser_type_id, id, 'Conveyor', 5900 FROM ingredient WHERE name = 'Olive';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 17, dispenser_type_id, id, 'Syringe', 6150 FROM ingredient WHERE name = 'Grenadine';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 18, dispenser_type_id, id, 'Slice Dispenser', 6525 FROM ingredient WHERE name = 'Lime Slice';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 19, dispenser_type_id, id, 'Stirrer', 6825 FROM ingredient WHERE name = 'Stir';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 20, dispenser_type_id, id, 'Umbrella Dropper', 7080 FROM ingredient WHERE name = 'Maraschino Cherry';
INSERT INTO dispenser (id, dispenser_type_id, ingredient_id, name, rail_position) SELECT 21, dispenser_type_id, id, 'Manual', 7080 FROM ingredient WHERE name = 'Ice';

