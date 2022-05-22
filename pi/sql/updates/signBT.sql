DROP TABLE IF EXISTS signBluetooth;

CREATE TABLE signBluetooth (
	signBluetoothId int not null auto_increment primary key,
	signMac varchar(32) not null,
	firstSeen timestamp,
	lastSeen timestamp,
	currentlySeen tinyint,
	poll tinyint
);

DROP TABLE IF EXISTS signConfigData;
CREATE TABLE signConfigData (
	signConfigDataId int not null auto_increment primary key,
	signBluetoothId int not null,
	signConfigKey varchar(32),
	signConfigValue varchar(256),
	readTime timestamp
);

	
	
	

