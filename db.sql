PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS havemeeting;
DROP TABLE IF EXISTS organizemeeting;
DROP TABLE IF EXISTS haveequipment;
DROP TABLE IF EXISTS message;
DROP TABLE IF EXISTS belongtogroup;
DROP TABLE IF EXISTS user;
DROP TABLE IF EXISTS grp;
DROP TABLE IF EXISTS meeting;
DROP TABLE IF EXISTS equipment;
DROP TABLE IF EXISTS room;

CREATE TABLE room(
	room_id integer PRIMARY KEY AUTOINCREMENT,
	room_name varchar,
	room_capacity integer
);

INSERT INTO room VALUES(null, 'A200', '10');
INSERT INTO room VALUES(null, 'A312', '10');
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
	meeting_periodic integer,
	meeting_compulsary integer,
	meeting_color varchar,
	FOREIGN KEY('room_id') REFERENCES 'room' ('room_id')
);

INSERT INTO meeting VALUES(null, '4', '2010-06-21 08:00', '2010-06-21 11:00', 'Retraite de Jean-Paul', '0', '0', );
INSERT INTO meeting VALUES(null, '4', '2010-06-21 13:00', '2010-06-21 14:30', 'Anniversaire de George', '0', '0');
INSERT INTO meeting VALUES(null, '2', '2010-06-22 09:00', '2010-06-22 11:30', 'Réunion hebdomadaire LO45', '1', '1');
INSERT INTO meeting VALUES(null, '2', '2010-06-22 15:30', '2010-06-22 17:30', 'Bambo Clash hebdomadaire', '1', '0');
INSERT INTO meeting VALUES(null, '2', '2010-06-23 08:00', '2010-06-23 10:30', 'Réunion mensuelle finance', '2', '0');
INSERT INTO meeting VALUES(null, '2', '2010-06-23 13:00', '2010-06-23 15:00', 'Lancement nouveau projet R&D', '0', '1');
INSERT INTO meeting VALUES(null, '4', '2010-06-24 13:00', '2010-06-24 15:00', 'Réunion mensuelle GI02', '2', '0');

CREATE TABLE grp(
	grp_id integer PRIMARY KEY AUTOINCREMENT,
	grp_name varchar
);

INSERT INTO grp VALUES(null, 'R&D');
INSERT INTO grp VALUES(null, 'Marketing');
INSERT INTO grp VALUES(null, 'GI02');
INSERT INTO grp VALUES(null, 'LO45');
INSERT INTO grp VALUES(null, 'Bambo Clash');


CREATE TABLE user(
	user_id integer PRIMARY KEY AUTOINCREMENT,
	user_name varchar,
	user_surname varchar,
	user_nickname varchar,
	user_password varchar,
	user_access integer NOT NULL DEFAULT 1
);

INSERT INTO user VALUES(null, 'Bastien', 'CRAMILLET', 'bcramill', 'bigz', '2');
INSERT INTO user VALUES(null, 'Adrien', 'GAVIGNET', 'agavigne', 'asb', '2');
INSERT INTO user VALUES(null, 'Jérémy', 'MALTIS', 'jmaltis', 'moltes', '1');
INSERT INTO user VALUES(null, 'Xavier', 'MICHEL', 'xmichel', 'saffir', '1');
INSERT INTO user VALUES(null, 'Alexandre', 'OULD-HAMOUDA', 'aould', 'tetex', '1');
INSERT INTO user VALUES(null, 'Kévin', 'LAURENT', 'klaurent', 'khand', '1');
INSERT INTO user VALUES(null, 'Claude', 'RENAUD', 'crenaud', 'cloclo', '2');

CREATE TABLE belongtogroup(
	grp_id integer,
	user_id integer,
	PRIMARY KEY ('grp_id', 'user_id'),
	FOREIGN KEY ('grp_id') REFERENCES 'grp' ('grp_id'),
	FOREIGN KEY ('user_id') REFERENCES 'user' ('user_id')
);

INSERT INTO belongtogroup VALUES('1', '1');
INSERT INTO belongtogroup VALUES('1', '2');
INSERT INTO belongtogroup VALUES('1', '7');
INSERT INTO belongtogroup VALUES('2', '3');
INSERT INTO belongtogroup VALUES('2', '6');
INSERT INTO belongtogroup VALUES('3', '1');
INSERT INTO belongtogroup VALUES('3', '2');
INSERT INTO belongtogroup VALUES('3', '3');
INSERT INTO belongtogroup VALUES('3', '4');
INSERT INTO belongtogroup VALUES('3', '5');
INSERT INTO belongtogroup VALUES('3', '6');
INSERT INTO belongtogroup VALUES('3', '7');
INSERT INTO belongtogroup VALUES('4', '1');
INSERT INTO belongtogroup VALUES('4', '2');
INSERT INTO belongtogroup VALUES('4', '3');
INSERT INTO belongtogroup VALUES('4', '4');
INSERT INTO belongtogroup VALUES('4', '5');
INSERT INTO belongtogroup VALUES('4', '7');
INSERT INTO belongtogroup VALUES('5', '1');
INSERT INTO belongtogroup VALUES('5', '2');
INSERT INTO belongtogroup VALUES('5', '4');
INSERT INTO belongtogroup VALUES('5', '5');

CREATE TABLE message(
	user_id_from integer,
	user_id_to integer,
	msg_date datetime,
	msg_from int,
	msg_subject varchar,
	msg_text text,
	PRIMARY KEY ('user_id_from', 'user_id_to', 'msg_date', 'msg_from'),
	FOREIGN KEY ('user_id_from') REFERENCES 'user' ('user_id'),
	FOREIGN KEY ('user_id_to') REFERENCES 'user' ('user_id')
);

INSERT INTO message VALUES('1', '2', '2010-06-03 20:34', '0', 'Demande de logement', 'Bonjour, je voulais savoir si je pouvais dormir chez vous ce soir ?');
INSERT INTO message VALUES('1', '2', '2010-06-03 20:34', '1', 'Demande de logement', 'Bonjour, je voulais savoir si je pouvais dormir chez vous ce soir ?');
INSERT INTO message VALUES('2', '3', '2010-06-03 18:43', '0', 'Projet AG41', 'Bonjour, êtes-vous dispo ce weekend pour bosser AG41 ?');
INSERT INTO message VALUES('2', '3', '2010-06-03 18:43', '1', 'Projet AG41', 'Bonjour, êtes-vous dispo ce weekend pour bosser AG41 ?');

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

CREATE TABLE organizemeeting(
	meeting_id integer,
	user_id integer,
	PRIMARY KEY ('meeting_id', 'user_id'),
	FOREIGN KEY ('meeting_id') REFERENCES 'meeting' ('meeting_id'),
	FOREIGN KEY ('user_id') REFERENCES 'user' ('user_id')
);

INSERT INTO organizemeeting VALUES('1', '7');
INSERT INTO organizemeeting VALUES('2', '7');
INSERT INTO organizemeeting VALUES('3', '7');
INSERT INTO organizemeeting VALUES('4', '1');
INSERT INTO organizemeeting VALUES('5', '3');
INSERT INTO organizemeeting VALUES('6', '2');
INSERT INTO organizemeeting VALUES('7', '3');

CREATE TABLE havemeeting(
	meeting_id integer,
	user_id integer,
	hm_state integer,
	PRIMARY KEY('meeting_id', 'user_id'),
	FOREIGN KEY('meeting_id') REFERENCES 'meeting' ('meeting_id'),
	FOREIGN KEY('user_id') REFERENCES 'user' ('user_id')
);


INSERT INTO havemeeting VALUES('1', '1', '1');
INSERT INTO havemeeting VALUES('1', '2', '1');
INSERT INTO havemeeting VALUES('1', '3', '0');
INSERT INTO havemeeting VALUES('1', '4', '0');
INSERT INTO havemeeting VALUES('1', '5', '1');
INSERT INTO havemeeting VALUES('1', '6', '1');
INSERT INTO havemeeting VALUES('1', '7', '1');

INSERT INTO havemeeting VALUES('2', '1', '0');
INSERT INTO havemeeting VALUES('2', '2', '0');
INSERT INTO havemeeting VALUES('2', '3', '1');
INSERT INTO havemeeting VALUES('2', '4', '1');
INSERT INTO havemeeting VALUES('2', '5', '1');
INSERT INTO havemeeting VALUES('2', '6', '0');
INSERT INTO havemeeting VALUES('2', '7', '1');

INSERT INTO havemeeting VALUES('3', '1', '1');
INSERT INTO havemeeting VALUES('3', '2', '1');
INSERT INTO havemeeting VALUES('3', '3', '1');
INSERT INTO havemeeting VALUES('3', '4', '1');
INSERT INTO havemeeting VALUES('3', '5', '1');
INSERT INTO havemeeting VALUES('3', '7', '1');

INSERT INTO havemeeting VALUES('4', '1', '1');
INSERT INTO havemeeting VALUES('4', '2', '0');
INSERT INTO havemeeting VALUES('4', '4', '1');
INSERT INTO havemeeting VALUES('4', '5', '0');

INSERT INTO havemeeting VALUES('5', '3', '1');
INSERT INTO havemeeting VALUES('5', '6', '1');

INSERT INTO havemeeting VALUES('6', '1', '1');
INSERT INTO havemeeting VALUES('6', '2', '1');
INSERT INTO havemeeting VALUES('6', '7', '1');

INSERT INTO havemeeting VALUES('7', '1', '1');
INSERT INTO havemeeting VALUES('7', '2', '1');
INSERT INTO havemeeting VALUES('7', '3', '1');
INSERT INTO havemeeting VALUES('7', '4', '1');
INSERT INTO havemeeting VALUES('7', '5', '1');
INSERT INTO havemeeting VALUES('7', '7', '1');
