DROP TABLE IF EXISTS users;
CREATE TABLE users (
	userId int not null auto_increment primary key,
	userName varchar(128),
	userPassword varchar(128)
);

	
