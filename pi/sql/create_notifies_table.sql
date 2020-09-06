DROP TABLE IF EXISTS signNotifies;

CREATE TABLE signNotifies( signNotifyId int not null auto_increment primary key,
	signId int not null,
	setTime timestamp not null,
	notifyTime timestamp,
	notified tinyint not null default 0
);

	
