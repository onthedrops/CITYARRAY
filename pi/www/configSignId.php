<?php

        require('header.inc.php');
        include_once('firmware_version.php');
	include_once('BTSign.inc.php');


	$signId = $_POST['signId']; // todo: validation

	$btsign = new BTSign();


	if($_POST['P']) {
		$delta = 0;
		$fields = $btsign->getFields();
		foreach ($fields as $key => $notes) {
			$delta += $btsign->setData($signId,$key, $_POST[$key]);
		}

		if($delta) {
			$ok = $btsign->push($signId);
			if(!$ok) {
				echo "<FONT COLOR=Red>Error - was unable to save data to the sign insice the timeout window.";
			}
		}
	}

	$ok = $btsign->poll($signId);

	if(!$ok) {
		echo "<FONT COLOR=Red>Error - was unable to poll the sign insice the timeout window. Shift-reload to retry";
	} else {
		echo "<FONT COLOR=Green>OK - sign data loaded successfully";
	}


	$fields = $btsign->getFields();
	$data = $btsign->getData($signId);
?>
<FORM METHOD=POST>
<INPUT TYPE=HIDDEN NAME=P VALUE=1>
<INPUT TYPE=HIDDEN NAME=signId VALUE=<?php echo $signId ?>>

<TABLE>
<?php
	foreach ($fields as $field => $notes) {
		echo "<TR><TD>$field ($notes)</TD></TR>";
		echo "<TR><TD><INPUT NAME=$field SIZE=128 VALUE=\"" . $data[$field] . "\"></TD></TR>";
	}
?>
</TABLE>
<INPUT TYPE=Submit>
</FORM>

	


