<?php

require_once('lib/mysqlng.inc.php');

class User {

        private $dbh;
        public $userId;
	private static $instance = null;
        public $username;

	private function __construct()
	{
        	$this->dbh = new SQLng();
	}

	public function getSignGroups()
	{
		// returns a array of id -> groupName for this user
		$ret = array();
		if(!$this->userId)
			return $ret;

		$dbh = $this->dbh;
		$dbh->Query("select g.groupId, g.groupName FROM signGroups g, signsXgroups sg, signsXnetworks sn, usersXnetworks un WHERE g.groupId = sg.groupId AND sg.signId = sn.signId AND sn.networkId = un.networkId AND un.userId = " . $this->userId);
		
		while($dbh->next_record()) {
			$key = $dbh->f("groupId");
			$name = $dbh->f("groupName");

			$ret[$key] = $name;
		}

		return $ret;
	}

	public function getSigns()
	{
		// returns a array of id -> signName for this user
		$ret = array();

		if(!$this->userId)
			return $ret;

		$dbh = $this->dbh;
		$dbh->Query("SELECT s.signId, s.signName FROM signs s, signsXnetworks sn, usersXnetworks un  WHERE s.signId = sn.signId AND sn.networkId = un.networkId AND un.userId = " . $this->userId);

		while($dbh->next_record()) {
			$key = $dbh->f("signId");
			$name = $dbh->f("signName");

			$ret[$key] = $name;
		}

		return $ret;
	}


	public static function getInstance()
	{
		if (self::$instance == null)
		{
			self::$instance = new User();
		}
 
		return self::$instance;
	}

	public static function getDBH()
	{
		return self::$instance->dbh;
	}

	public function setUser($userId) {
                $this->userId = $userId;
	}

	 public function Logout() {
                unset( $_SESSION['userId'] );
        }
		
}
