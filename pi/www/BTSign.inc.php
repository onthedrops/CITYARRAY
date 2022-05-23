<?php

require_once('lib/mysqlng.inc.php');

class BTSign {
	private $dbh;

	public function __construct()
	{
		$this->dbh = new SQLng();
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
