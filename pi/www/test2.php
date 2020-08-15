<?php

include('lib/mysqlng.inc.php');

$sql = new SQLng();
$sql->Query("SELECT * FROM test");

while($sql->next_record()) {
	echo "A: " . $sql->f("a") . "\n";
	echo "B: " . $sql->f("b") . "\n";
}

	


