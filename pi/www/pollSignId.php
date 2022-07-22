<?php

        require('header.inc.php');
        include_once('firmware_version.php');
	include_once('BTSign.inc.php');


	$signId = $_POST['signId']; // todo: validation

	$btsign = new BTSign();

	if(isset($_POST['P'])) {
		echo "POST\n";
		if(isset($_POST['Reboot'])) {
			$ok = $btsign->reboot($signId);
		} else if(isset($_POST['Ident'])) {
			$ok = $btsign->ident($signId);
		}
	} else {
		$ok = $btsign->debugpoll($signId);
	}

	if(!$ok) {
		echo "<FONT COLOR=Red>Error - was unable to poll the sign insice the timeout window. Shift-reload to retry";
	} 


	$fields = $btsign->getFields();
	$data = $btsign->getDebugData($signId);
?>
<FORM METHOD=POST>
<INPUT TYPE=HIDDEN NAME=P VALUE=1>
<INPUT TYPE=HIDDEN NAME=signId VALUE=<?php echo $signId ?>>

<TEXTAREA ROWS=10 COLS=60><?php echo $data['debug']; ?></TEXTAREA><BR>
<TABLE>
<?php

	foreach ($fields as $field => $notes) {
		echo "<TR><TD>$field ($notes)</TD></TR>";
		echo "<TR><TD><INPUT NAME=$field SIZE=128 VALUE=\"" . $data[$field] . "\"></TD></TR>";
	}
?>
<TR><TD><INPUT TYPE=Submit NAME=Reboot VALUE=Reboot><INPUT TYPE=Submit NAME=Ident VALUE=Ident></TD></TR>

</TABLE>
</FORM>

	


