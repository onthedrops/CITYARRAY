<?php

        require('header.inc.php');
?>
<PRE>
<?php
	include_once('firmware_version.php');
	$fp = fopen("/var/log/signd.log", "r");
	while($str = fgets($fp)) {
		echo "$str";
	}

	fclose($fp);

