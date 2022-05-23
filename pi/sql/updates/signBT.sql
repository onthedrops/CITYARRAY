DROP TABLE IF EXISTS signBluetooth;

CREATE TABLE signBluetooth (
	signBluetoothId int not null auto_increment primary key,
	signMac varchar(32) not null,
	lastSeen timestamp,
	firstSeen timestamp,
	currentlySeen tinyint,
	seenAs varchar(64),
	poll tinyint
);

DROP TABLE IF EXISTS signConfigData;
CREATE TABLE signConfigData (
	signBluetoothId int not null,
	signConfigKey varchar(32),
	signConfigValue varchar(256),
	readTime timestamp,
	primary key(signBluetoothId, signConfigKey)
);

DROP TABLE IF EXISTS signConfigKeys;
CREATE TABLE signConfigKeys (
	signConfigKey varchar(32)
);

INSERT INTO signConfigKeys VALUES ('ssid');
INSERT INTO signConfigKeys VALUES ('password');
INSERT INTO signConfigKeys VALUES ('bluetoothID');
INSERT INTO signConfigKeys VALUES ('signID');
INSERT INTO signConfigKeys VALUES ('fetchHost');
INSERT INTO signConfigKeys VALUES ('upgradeURL');
INSERT INTO signConfigKeys VALUES ('sigURL');
