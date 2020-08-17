DROP TABLE IF EXISTS users;
CREATE TABLE users (
	userId int not null auto_increment primary key,
	userName varchar(128),
	userPassword varchar(128)
);


INSERT INTO users VALUES (NULL,'sheer',sha1('vicky'));
INSERT INTO users VALUES (NULL,'eben',sha1('rey'));

DROP TABLE IF EXISTS usersXnetworks;
CREATE TABLE usersXnetworks (
	usersXnetworksId int not null auto_increment primary key,
	userId int not null,
	networkId int not null
);

INSERT INTO usersXnetworks VALUES (NULL,1,1);
INSERT INTO usersXnetworks VALUES (NULL,2,1);



	
