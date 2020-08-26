DROP TABLE IF EXISTS signPresets;
CREATE TABLE signPresets (
	presetId int not null auto_increment primary key,
	userId int not null,
	presetName varchar(255)
);

DROP TABLE IF EXISTS signPresetData;
CREATE TABLE signPresetData (
	presetDataId int not null auto_increment primary key,
	presetId int not null,
	signId int not null,
	messageId int not null
);



