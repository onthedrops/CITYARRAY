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

	public function requestShutdown()
	{
		$this->dbh->Query("UPDATE shutdown SET shutdown_requested = 1");
	}

	public function getSignGroupMessages($groupId)
	{
		$ret = array();
		if(!$this->userId)
			return $ret;

		$dbh = $this->dbh;
		$dbh->Query("SELECT sm.messageId, m.message, sm.signId FROM signMessage sm, messages m, signsXgroups sxg where sxg.groupId = " . $dbh->equote($groupId) . " AND sxg.signId = sm.signId AND m.messageId = sm.messageId");
		while($dbh->next_record()) {
			$rowArray = array(
				'signId' => $dbh->f("signId"),
				'messageId' => $dbh->f("messageId"),
				'message' => $dbh->f("message")
			);
	
			array_push($ret, $rowArray);
		}
		
		return $ret;
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

	public function owns($signId)
	{
		// todo : make sure user is allowed to access sign

		return true;
	}
	

	public function getMessage($signId) 
	{
		if(!$this->userId)
			return false;

		$dbh = $this->dbh;

		$message = $dbh->selectOne("SELECT message FROM messages m, signMessage sm WHERE m.messageId = sm.messageId AND sm.signId = " . $dbh->equote($signId));
		return $message;	
	}

	public function getMessages()
	{
		$ret = array();

		if(!$this->userId)
			return $ret;

		

		$dbh = $this->dbh;
		$dbh->Query("SELECT messageId, message FROM messages WHERE userId = " . $this->userId);
		while($dbh->next_record()) {
			$key = $dbh->f("messageId");
			$ret[$key] = $dbh->f('message');
		}
	
		return $ret;
	}

	public function addPreset($presetName)
	{
		if(!$this->userId)
			return false;

		
		$dbh = $this->dbh;
		$dbh->Query("INSERT INTO signPresets SET presetName = " . $dbh->equote($presetName) . ", userId = " .  $this->userId);

	}
	
	public function setPresetValue($presetId, $signId, $signValue)
	{
		$dbh = $this->dbh;
		

		$dbh->Query("SELECT presetDataId FROM signPresetData WHERE presetId = " . $dbh->equote($presetId) . " AND signId = " . $dbh->equote($signId));
		$dbh->next_record();

		$presetDataId = $dbh->f("presetDataId");
		if($presetDataId) {
			$dbh->Query("UPDATE signPresetData SET messageId = " . $dbh->equote($signValue) . " WHERE presetDataId = $presetDataId");
		} else {
			$dbh->Query("INSERT INTO signPresetData SET presetId = " . $dbh->equote($presetId) . ", signId = " . $dbh->equote($signId) . ", messageId = " . $dbh->equote($signValue));
		}
	}

	public function getPreset($presetId) 
	{
		// todo : ensure user owns preset!

		$ret = array();

		$dbh = $this->dbh;
		$dbh->Query("SELECT s.signId, ss.signName, s.messageId, m.message FROM signPresetData s, signs ss, messages m WHERE ss.signId = s.signId AND m.messageId = s.messageId AND s.presetId = " . $dbh->equote($presetId));
	
		while($dbh->next_record()) {
			$rowArray = array('signId' => $dbh->f('signId'),
					'messageId' => $dbh->f('messageId'),
					'signName' => $dbh->f('signName'),
					'message' => $dbh->f('message'));

			$ret[$dbh->f('signId')] = $rowArray;
		}
		
		return $ret;
	}

	public function getPresetId($presetName)
	{
		if(!$this->userId)
			return 0;

		$dbh = $this->dbh;
		$dbh->Query("SELECT s.presetId AS id FROM signPresets s WHERE s.userId = " . $this->userId . " AND s.presetName = " . $dbh->equote($presetName));
		if($dbh->next_record()) {
			return $dbh->f("id");
		}
	}


	public function getPresets()
	{
		$ret = array();

		if(!$this->userId)
			return $ret;

		$dbh = $this->dbh;
		$dbh->Query("SELECT s.presetId, s.presetName FROM signPresets s WHERE s.userId = " . $this->userId);
		while($dbh->next_record()) {
			$key = $dbh->f("presetId");
			$name = $dbh->f("presetName");

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

	public function getSignConfig($signId, $configKey)
	{
		if(!$this->userId)
			return null;

			
		$dbh = $this->dbh;
		$dbh->Query("SELECT s.signId, sc.configValue FROM signs s, signsXnetworks sn, usersXnetworks un, signConfig sc WHERE s.signId = $signId AND s.signId = sn.signId AND sn.networkId = un.networkId AND un.userId = " . $this->userId . " AND sc.signId = s.signId AND sc.configKey = " . $dbh->equote($configKey));

		if($dbh->next_record()) {
			return $dbh->f("configValue");
		}

		return null;

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

	public function setUserId($userId) {
                $this->userId = $userId;
	}

	public function getUserId() {
		return $this->userId;
	}

	public function Logout() {
                unset( $_SESSION['userId'] );
        }

	public static function setSignMessage($signId, $messageId) {
		$user = self::getInstance();
		$userId = $user->userId;

		$dbh = $user->dbh;
		$dbh->Query("INSERT INTO signMessageArchive SET signId = $signId, messageId=$messageId,setDate=NOW()");
		$dbh->Query("UPDATE signMessage SET setDate=NOW(), messageId = $messageId WHERE signId = $signId");
		$dbh->Query("INSERT INTO signNotifies SET signId=$signId,setTime=NOW(),notified=0");

		// todo: validate input before acting on it!
	}

	public static function setSignGroupMessage($signGroupId, $messageId) {
		$user = self::getInstance();
		$userId = $user->userId;
		$dbh = $user->dbh;

		$signList = array();

		$dbh->Query("SELECT signId FROM signsXgroups WHERE groupId = $signGroupId");
		while($dbh->next_record()) {
			array_push($signList, $dbh->f("signId"));
		}

		foreach($signList as $signId) {
			self::setSignMessage($signId, $messageId);
		}
	}
		
}
