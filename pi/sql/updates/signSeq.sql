DROP TABLE IF EXISTS signSequence;

CREATE TABLE signSequence ( signId int not null unique primary key, sequenceId int, messageId int, lastSeen timestamp  ) ENGINE=MEMORY;
