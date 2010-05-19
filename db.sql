PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS havemeeting;
DROP TABLE IF EXISTS haveequipment;
DROP TABLE IF EXISTS user;
DROP TABLE IF EXISTS meeting;
DROP TABLE IF EXISTS equipment;
DROP TABLE IF EXISTS room;

CREATE TABLE room(
	id integer PRIMARY KEY AUTOINCREMENT,
	name varchar,
	capacity integer
);

INSERT INTO room VALUES(null, 'A200', '10');
INSERT INTO room VALUES(null, 'A312', '6');
INSERT INTO room VALUES(null, 'B402', '5');
INSERT INTO room VALUES(null, 'I102', '30');


CREATE TABLE equipment(
	id integer PRIMARY KEY AUTOINCREMENT,
	name varchar
);

INSERT INTO equipment VALUES(null, 'TV');
INSERT INTO equipment VALUES(null, 'Vidéo projecteur');
INSERT INTO equipment VALUES(null, 'Rétro projecteur');
INSERT INTO equipment VALUES(null, 'Ordinateur');
INSERT INTO equipment VALUES(null, 'Lecteur DVD');
INSERT INTO equipment VALUES(null, 'Playstation 3');
INSERT INTO equipment VALUES(null, 'Micro-onde');
INSERT INTO equipment VALUES(null, 'Cafetière');


CREATE TABLE meeting(
	id integer PRIMARY KEY AUTOINCREMENT,
	id_room integer,
	start date,
	end date,
	FOREIGN KEY('id_room') REFERENCES 'room' ('id')
);


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


CREATE TABLE haveequipment(
	id_room integer,
	id_equipment integer,
	PRIMARY KEY ('id_room', 'id_equipment'),
	FOREIGN KEY ('id_equipment') REFERENCES 'equipment' ('id'),
	FOREIGN KEY ('id_room') REFERENCES 'room' ('id')
);

INSERT INTO haveequipment VALUES('1', '2');
INSERT INTO haveequipment VALUES('1', '4');
INSERT INTO haveequipment VALUES('2', '1');
INSERT INTO haveequipment VALUES('2', '3');
INSERT INTO haveequipment VALUES('2', '5');
INSERT INTO haveequipment VALUES('2', '8');
INSERT INTO haveequipment VALUES('3', '2');
INSERT INTO haveequipment VALUES('3', '6');
INSERT INTO haveequipment VALUES('4', '1');
INSERT INTO haveequipment VALUES('4', '5');
INSERT INTO haveequipment VALUES('4', '7');
INSERT INTO haveequipment VALUES('4', '8');

CREATE TABLE havemeeting(
	id_meeting integer,
	id_user integer,
	PRIMARY KEY('id_meeting', 'id_user'),
	FOREIGN KEY('id_meeting') REFERENCES 'meeting' ('id'),
	FOREIGN KEY('id_user') REFERENCES 'user' ('id')
);