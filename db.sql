CREATE TABLE user(
	id integer PRIMARY KEY AUTOINCREMENT,
	name varchar,
	surname varchar,
	nickname varchar,
	password varchar,
	access integer NOT NULL DEFAULT 1
);

INSERT INTO user VALUES(null, 'Bastien', 'CRAMILLET', 'bcramill', 'bigz', '1');
INSERT INTO user VALUES(null, 'Adrien', 'GAVIGNET', 'agavigne', 'asb', '2');
INSERT INTO user VALUES(null, 'Jérémy', 'MALTIS', 'jmaltis', 'moltes', '1');

CREATE TABLE room(
	id integer PRIMARY KEY AUTOINCREMENT,
	name varchar,
	capacity integer
);

INSERT INTO room VALUES(null, 'A200', '10');
INSERT INTO room VALUES(null, 'A312', '6');
INSERT INTO room VALUES(null, 'B402', '5');
INSERT INTO room VALUES(null, 'I102', '30');