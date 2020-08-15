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


	public static function getInstance()
	{
		if (self::$instance == null)
		{
			self::$instance = new User();
		}
 
		return self::$instance;
	}

	public function setUser($userId) {
                $this->userId = $userId;
	}

	 public function Logout() {
                unset( $_SESSION['userId'] );
        }
		
}
