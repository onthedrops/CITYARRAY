DROP TABLE IF EXISTS signNetworks;
CREATE TABLE signNetworks (
	networkId int not null auto_Increment primary key,
	networkName varchar(20),
	networkPassword varchar(64)
);

INSERT INTO signNetworks VALUES (NULL,'neiosys',sha1('ebenrey'));


DROP TABLE IF EXISTS signGroups;
CREATE TABLE signGroups (
	groupId int not null auto_increment primary key,
	groupName varchar(64)
);

INSERT INTO signGroups VALUES(NULL,'Group');

DROP TABLE IF EXISTS signsXgroups;
CREATE TABLE signsXgroups (
	signXgroupId int not null auto_increment primary key,
	signId int not null,
	groupId int not null
);

INSERT INTO signsXgroups VALUES (NULL,1,1);
INSERT INTO signsXgroups VALUES (NULL,1,2);

