DROP TABLE IF EXISTS apiKeys;
CREATE TABLE apiKeys (
	apiKeyId int not null auto_increment primary key,
	userId int not null,
	apiKey varchar(32)
);

INSERT INTO apiKeys VALUES (NULL, 1, 'ebenrey');

