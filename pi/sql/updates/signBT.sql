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
	signConfigKey varchar(32),
	notes varchar(255)
);

INSERT INTO signConfigKeys VALUES ('ssid', 'wifi SSID');
INSERT INTO signConfigKeys VALUES ('password', 'Set to ~!EMPTY if no password');
INSERT INTO signConfigKeys VALUES ('bluetoothID','ID sign should appear as. Should begin with SIGN-');
INSERT INTO signConfigKeys VALUES ('signID', 'Integer sign ID. Not used');
INSERT INTO signConfigKeys VALUES ('fetchHost', 'URL to fetch content from, including query string');
INSERT INTO signConfigKeys VALUES ('upgradeURL', 'URL to fetch upgrade binary from');
INSERT INTO signConfigKeys VALUES ('sigURL', 'URL to fetch signature for upgrade from');
