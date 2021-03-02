<?php

	require('header.inc.php');
	$dbh = User::getDBH();
	
	$id = $_REQUEST['updateId'];
	$idq = $dbh->equote($id);
	$dbh->Query("SELECT updatetext FROM updates WHERE updateId = $id");
	if($dbh->next_record()) {
		$updatetext = $dbh->f("updatetext");

		
	}

	if(isset($updatetext) && $updatetext) {
		echo "<PRE>$updatetext</PRE>";
		echo "<HR><A HREF=/index.php>Return home</A>";
	} else {
		sleep(1);
		header("Location: /update-status.php?updateId=" . $id);
	}
