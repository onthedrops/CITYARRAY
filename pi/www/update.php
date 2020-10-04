<?php

	require('header.inc.php');
	
	if($_REQUEST['Update']) {
		$dbh = User::getDBH();
		$dbh->Query("INSERT INTO updates SET updateId = NULL");
		$dbh->Query("SELECT LAST_INSERT_ID() AS LID");
		$dbh->next_record();
		$updateId = $dbh->f("LID");
		
		header('Location: /update-status.php?updateId=' . $updateId);
	}

?>
<TABLE><TR><TD>
Click this to update from the git repo: </TD><TD>
<FORM METHOD=POST>
<INPUT TYPE=Submit NAME=Update VALUE=Update>
</FORM></TD></TR></TABLE>

