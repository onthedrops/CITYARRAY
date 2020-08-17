<?php

	require('header.inc.php');

	
	
	// display list of signs
	
?>
<FORM METHOD="POST">
	<TABLE>
	<TD>Individual sign</TD><TD>
		<SELECT ID="selIndividual">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
	
		$signList = User::getInstance()->getSigns();
		foreach($signList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
		</SELECT></TD><TD>Group of signs</TD><TD><SELECT ID="selGroup">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
		$groupList = User::getInstance()->getSignGroups();
		foreach($groupList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
	</SELECT>
	</TD></TR>
	<TR><TD COLSPAN=4><TEXTAREA ID="taMsg" ROWS="10" COLS="60"> </TEXTAREA></TD></TR>
	<TR><TD></TD><TD></TD><TD></TD><TD ALIGN="RIGHT"><INPUT TYPE="SUBMIT" /></TD></TR>
	</TABLE>
	</FORM>


