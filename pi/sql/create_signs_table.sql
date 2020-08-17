DROP TABLE IF EXISTS signs;
CREATE TABLE signs (
	signId int not null auto_increment primary key,
	networkId int not null,
	signRemoteId int not null,
	signName varchar(64)
);

INSERT INTO signs VALUES (NULL,1,1,'sign1');
INSERT INTO signs VALUES (NULL,1,2,'sign2');

DROP TABLE IF EXISTS signMessages;
CREATE TABLE signMessages (
	signId int not null,
	message text,
	setDate datetime,
	firstShownDate datetime,
	lastShownDate datetime
);

INSERT INTO signMessages VALUES (1,'Test Message',NOW(),NULL,NULL);
INSERT INTO signMessages VALUES (2,'~!GTest Message', NOW(), NULL, NULL);


DROP TABLE IF EXISTS signMessageArchive;
CREATE TABLE signMessageArchive (
	signArchiveId int not null auto_increment primary key,
	signId int not null,
	message text,
	setDate datetime,
	firstShownDate datetime,
	lastShownDate datetime,
	removeDate datetime
);





