<?php

require_once('lib/mysqlng.inc.php');

class BTSign {
	private $dbh;

	public function __construct()
	{
		$this->dbh = new SQLng();
	}

	public function getFields()
	{
		$this->dbh->Query("SELECT signConfigKey, notes FROM signConfigKeys");
		$a = array();

		while($this->dbh->next_record()) {
			$a[$this->dbh->f("signConfigKey")] = $this->dbh->f("notes");
		}

		return $a;
	}

	public function getData($signId)
	{
		$this->dbh->Query("SELECT signConfigKey, signConfigValue FROM signConfigData WHERE signBluetoothId = $signId");
		$a = array();

		while($this->dbh->next_record()) {
			$a[$this->dbh->f("signConfigKey")] = $this->dbh->f("signConfigValue");
		}
			
		return $a;
	}

	public function getDebugData($signId)
	{
		$this->dbh->Query("SELECT signConfigKey, signConfigValue FROM signConfigData WHERE signBluetoothId = $signId");
		$a = array();

		while($this->dbh->next_record()) {
			$a[$this->dbh->f("signConfigKey")] = $this->dbh->f("signConfigValue");
		}
			
		return $a;

	}

	public function setData($signId, $key, $value)
	{
		$keyq = $this->dbh->equote($key);
		$valueq = $this->dbh->equote($value);

		$this->dbh->Query("SELECT COUNT(*) AS cnt FROM signConfigData WHERE signBluetoothId = $signId AND signConfigKey = $keyq AND signConfigValue = $valueq");
		$this->dbh->next_record();

		if($this->dbh->f("cnt") == 1) {
			return 0;
		}

		$this->dbh->Query("REPLACE INTO signConfigData SET signBluetoothId = $signId, signConfigKey = $keyq, signCOnfigValue = $valueq, dirty = 1");
		return 1;
	}

	public function push($signId)
	{
		$this->dbh->Query("UPDATE signBluetooth set poll = 2 WHERE signBluetoothId = $signId");
		$count = 0;

		while(1) {
			$pollValue = $this->dbh->selectOne("SELECT poll FROM signBluetooth WHERE signBluetoothId = $signId");
			if($pollValue == 0) 
				return 1;
			if($count++ > 30)
				return 0;
			sleep(1);
		}

	}

	public function poll($signId, $pollValue=1, $timeout=30)
	{
		$this->dbh->Query("UPDATE signBluetooth set poll = $pollValue WHERE signBluetoothId = $signId");
		$count = 0;

		while(1) {
			$pollValue = $this->dbh->selectOne("SELECT poll FROM signBluetooth WHERE signBluetoothId = $signId");
			if($pollValue == -1)
				return 0;
			if($pollValue == 0) 
				return 1;
			if($count++ > $timeout)
				return 0;
			sleep(1);
		}
	}

	public function debugpoll($signId)
	{
		return $this->poll($signId, 5, 60);
	}

	public function reboot($signId)
	{
		return $this->poll($signId, 6);
	}

	public function ident($signId)
	{
		return $this->poll($signId, 7);
	}

	public function getSigns()
	{
		// returns a array listing all signs that can be currently seen

		$this->dbh->Query("SELECT signBluetoothId, signMac, seenAs FROM signBluetooth WHERE currentlySeen =1");
		$ret = array();

		while($this->dbh->next_record()) {
			$s = array( 'id' => $this->dbh->f("signBluetoothId"),
					'mac' => $this->dbh->f('signMac'),
					'name' => $this->dbh->f('seenAs')
				);
			array_push($ret, $s);
		}

		return $ret;
	}
}
?>
