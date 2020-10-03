CREATE TABLE updates (
	updateId int not null auto_increment primary key,
	updatetext text,
	updateVersion varchar(64),
	updateTimestamp timestamp
);

