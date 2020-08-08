DROP TABLE IF EXISTS signs;
CREATE TABLE signs (
	signId int not null auto_increment primary key,
	networkId int not null,
	signRemoteId int not null
);

DROP TABLE IF EXISTS signMessages;
CREATE TABLE signMessages (
	signId int not null,
	message text,
	setDate datetime
);

DROP TABLE IF EXISTS signMessageArchive;
CREATE TABLE signMessageArchive (
	signArchiveId int not null auto_increment primary key,
	signId int not null,
	message text,
	setDate datetime,
	removeDate datetime
);





