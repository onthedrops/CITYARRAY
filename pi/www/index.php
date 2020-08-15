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

?>
		</SELECT></TD><TD>Group of signs</TD><TD><SELECT ID="selGroup">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php

?>
	</SELECT>
	</TD></TR>
	<TR><TD COLSPAN=4><TEXTAREA ID="taMsg" ROWS="10" COLS="60"> </TEXTAREA></TD></TR>
	<TR><TD></TD><TD></TD><TD></TD><TD ALIGN="RIGHT"><INPUT TYPE="SUBMIT" /></TD></TR>
	</TABLE>
	</FORM>


