<?php

        require('header.inc.php');
	include_once('lib/mysqlng.inc.php');
	
	$dbh = new SQLng();

	// todo: add network ID

	if(isset($_POST['P'])) {

		if(0 == strlen($_POST['NAME'])) {
			echo "<FONT COLOR=RED><B>Error: name must be non-zero length</FONT></B>";
		} else {
			$nameq = $dbh->equote($_POST['NAME']);
			$id = $dbh->equote($_POST['ID']);
			$dbh->Query("INSERT INTO signs SET networkId=1,signRemoteId=$id,signName=$nameq");
			$signId = $dbh->selectOne("SELECT LAST_INSERT_ID()");
			$dbh->Query("INSERT INTO signsXnetworks SET signId = $signId, networkId=1");
			echo "Done\n";
			exit(0);
		}
	}

	$maxId = $dbh->selectOne("SELECT max(signRemoteId) FROM signs");
	if($maxId) {
		$maxId++;
	}

?>
<H1>Create sign</H1><BR>


<FORM METHOD=POST>
<INPUT TYPE=Hidden NAME=P VALUE=1>
<TABLE>
<TR><TD>Sign name:</TD><TD><INPUT TYPE=TEXT NAME=NAME></TD></TR>
<TR><TD>Sign ID:</TD><TD><INPUT TYPE=TEXT NAME=ID VALUE=<?php echo $maxId; ?>></TD></TR>
<TR><TD><INPUT TYPE=Submit></TD></TR>
</TABLE>
</FORM>



