DROP TABLE IF EXISTS signs;
CREATE TABLE signs (
	signId int not null auto_increment primary key,
	networkId int not null,
	signRemoteId int not null,
	signName varchar(64),
	signIp varchar(32)
);

INSERT INTO signs VALUES (NULL,1,1,'sign1',NULL);
INSERT INTO signs VALUES (NULL,1,2,'sign2',NULL);


DROP TABLE IF EXISTS messages;
CREATE TABLE messages (
	messageId int not null auto_increment primary key,
	userId int not null,
	message text,
	createDate datetime
);


INSERT INTO messages VALUES (NULL,1,'Test Message',NOW());
INSERT INTO messages VALUES (NULL,1,'~!GTest Message', NOW());

DROP TABLE IF EXISTS signMessage;
CREATE TABLE signMessage (
	signId int unique key not null,
	messageId int not null,
	setDate datetime,
	firstShownDate datetime,
	lastShownDate datetime
);

INSERT INTO signMessage VALUES (1,1,NOW(),NULL,NULL);
INSERT INTO signMessage VALUES (2,2,NOW(),NULL,NULL);


DROP TABLE IF EXISTS signMessageArchive;
CREATE TABLE signMessageArchive (
	signArchiveId int not null auto_increment primary key,
	signId int not null,
	messageId int not null,
	setDate datetime,
	firstShownDate datetime,
	lastShownDate datetime,
	removeDate datetime
);





