<?php

	require('header.inc.php');
	

	
//	print_r($_REQUEST);	
	// display list of signs
	if(isset($_REQUEST['Submit'])) {
		$messageId = Message::saveMessage($_REQUEST['taMsg']);
		// if a individual sign is selected, set that sign to this message
		// if a group of signs is selected, set those signs to this message
		if(isset($_REQUEST['selIndividual']) && $_REQUEST['selIndividual']) {
			User::setSignMessage($_REQUEST['selIndividual'], $messageId);
		}

		if(isset($_REQUEST['selGroup']) && $_REQUEST['selGroup']) {
			User::setSignGroupMessage($_REQUEST['selGroup'],$messageId);
		}
	}
	
?>
<FORM ACTION="/index.php" METHOD="POST">
	<TABLE>
	<TD>Individual sign</TD><TD>
		<SELECT NAME="selIndividual">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
	
		$signList = User::getInstance()->getSigns();
		foreach($signList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
		</SELECT></TD><TD>Group of signs</TD><TD><SELECT NAME="selGroup">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
		$groupList = User::getInstance()->getSignGroups();
		foreach($groupList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
	</SELECT>
	</TD></TR>
	<TR><TD COLSPAN=4><TEXTAREA NAME="taMsg" ROWS="10" COLS="60"> </TEXTAREA></TD></TR>
	<TR><TD></TD><TD></TD><TD></TD><TD ALIGN="RIGHT"><INPUT NAME="Submit" TYPE="SUBMIT" /></TD></TR>
	</TABLE>
	</FORM>


