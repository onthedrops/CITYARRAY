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

	public function setData($signId, $key, $value)
	{

	}

	public function push($signId)
	{

	}

	public function poll($signId)
	{
		$this->dbh->Query("UPDATE signBluetooth set poll = 1 WHERE signBluetoothId = $signId");
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


	public function getSigns()
	{
		// returns a array listing all signs that can be currently seen

		$this->dbh->Query("SELECT signBluetoothId, signMac, seenAs FROM signBluetooth");
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
