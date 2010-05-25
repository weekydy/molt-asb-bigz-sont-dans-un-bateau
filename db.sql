PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS havemeeting;
DROP TABLE IF EXISTS haveequipment;
DROP TABLE IF EXISTS user;
DROP TABLE IF EXISTS meeting;
DROP TABLE IF EXISTS equipment;
DROP TABLE IF EXISTS room;

CREATE TABLE room(
	room_id integer PRIMARY KEY AUTOINCREMENT,
	room_name varchar,
	room_capacity integer
);

INSERT INTO room VALUES(null, 'A200', '10');
INSERT INTO room VALUES(null, 'A312', '6');
INSERT INTO room VALUES(null, 'B402', '5');
INSERT INTO room VALUES(null, 'I102', '30');


CREATE TABLE equipment(
	equip_id integer PRIMARY KEY AUTOINCREMENT,
	equip_name varchar
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
	meeting_id integer PRIMARY KEY AUTOINCREMENT,
	room_id integer,
	meeting_begin datetime,
	meeting_end datetime,
	meeting_label varchar,
	FOREIGN KEY('room_id') REFERENCES 'room' ('room_id')
);

INSERT INTO meeting VALUES(null, 2, '2010-05-20 08:45', '2010-05-21 11:30', 'Retraite de Jean-Paul');

CREATE TABLE user(
	user_id integer PRIMARY KEY AUTOINCREMENT,
	user_name varchar,
	user_surname varchar,
	user_nickname varchar,
	user_password varchar,
	user_access integer NOT NULL DEFAULT 1
);

INSERT INTO user VALUES(null, 'Bastien', 'CRAMILLET', 'bcramill', 'bigz', '1');
INSERT INTO user VALUES(null, 'Adrien', 'GAVIGNET', 'agavigne', 'asb', '2');
INSERT INTO user VALUES(null, 'Jérémy', 'MALTIS', 'jmaltis', 'moltes', '1');


CREATE TABLE haveequipment(
	room_id integer,
	equip_id integer,
	PRIMARY KEY ('room_id', 'equip_id'),
	FOREIGN KEY ('equip_id') REFERENCES 'equipment' ('equip_id'),
	FOREIGN KEY ('room_id') REFERENCES 'room' ('room_id')
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
	meeting_id integer,
	user_id integer,
	PRIMARY KEY('meeting_id', 'user_id'),
	FOREIGN KEY('meeting_id') REFERENCES 'meeting' ('meeting_id'),
	FOREIGN KEY('user_id') REFERENCES 'user' ('user_id')
);

INSERT INTO havemeeting VALUES('1', '1');
INSERT INTO havemeeting VALUES('1', '2');