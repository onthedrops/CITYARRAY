DROP TABLE IF EXISTS signConfig;
CREATE TABLE signConfig (
	signConfigId int not null auto_increment primary key,
	signId int,
	configKey varchar(64),
	configValue varchar(255)
);

