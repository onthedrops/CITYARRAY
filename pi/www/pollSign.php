<?php

        require('header.inc.php');
        include_once('firmware_version.php');
	include_once('BTSign.inc.php');


	$btsign = new BTSign();

	$signs = $btsign->getSigns();


?>
<I>Note - reload page to see most recent signs visible via bluetooth to the pi</I>

<FORM METHOD=POST ACTION=pollSignId.php>
<TABLE><TR><TD>
Debug sign:
</TD><TD>
	<SELECT ID=signId NAME=signId>
<?php
	foreach ($signs as $sign)
	{
		echo "<OPTION VALUE=\"" . $sign['id'] . "\">" . $sign['mac'] . ' (' . $sign['name'] . ')</OPTION>';

	}
?>
	</SELECT>
</TD><TD><INPUT TYPE=SUBMIT></TR><TABLE>



