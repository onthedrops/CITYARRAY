DROP TABLE IF EXISTS users;
CREATE TABLE users (
	userId int not null auto_increment primary key,
	userName varchar(128),
	userPassword varchar(128)
);


INSERT INTO users VALUES (NULL,'sheer',sha1('vicky'));
INSERT INTO users VALUES (NULL,'eben',sha1('rey'));

	
