<?php

require_once('lib/mysqlng.inc.php');
require_once('User.inc.php');

class Message {
	private $dbh;
	private static $instance = null;

	private function __construct()
	{
		$this->dbh = new SQLng();
	}

	public static function getInstance()
        {
                if (self::$instance == null)
                {
                        self::$instance = new Message();
                }

                return self::$instance;
        }

	public static function getDbh()
	{
		$me = self::getInstance();
		return $me->dbh;
	}

	public static function saveMessage($msg)
	{
		$dbh = self::getDbh();
		$userId = User::getInstance()->getUserId();
		$msgq = $dbh->equote($msg);
		
		if(!$userId) {
			return 0;
		}

		$dbh->Query("SELECT messageId FROM messages WHERE 
			userId = $userId AND message = $msgq");

		$dbh->next_record();
			
		$msgId = $dbh->f("messageId");

		if($msgId) {
			return $msgId;
		}

		$dbh->Query("INSERT INTO messages
				SET message = $msgq,userId = $userId,createDate=NOW()");
		
		$dbh->Query("SELECT last_insert_id() AS lid");
		
		$dbh->next_record();

		$msgId = $dbh->f("lid");

		return $msgId;
	}
}

