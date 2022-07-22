<?php

        require('header.inc.php');
?>
<A HREF=/log.php?file=bt>Bluetooth</A>&nbsp;&nbsp;<A HREF=/log.php?file=web>Web</A>&nbsp;&nbsp;<A HREF=/log.php?file=sign>sign</A>&nbsp;&nbsp;
<PRE>
<?php
	include_once('firmware_version.php');

	if(isset($_REQUEST['file'])) {
		switch($_REQUEST['file']) {
			case "bt": $file = '/var/log/signbtd.log'; break;
			case "web": $file = '/var/log/apache2/access.log'; break;
			default: $file = '/var/log/signd.log'; break;
		}
	} else {
		$file = '/var/log/signd.log';
	}

	$fp = popen("tail -n 1000 $file", "r");
	if(!$fp) {
		echo "Could not open $file";
	} else {
		echo  "<BR><B>$file</B><BR>";
	
	}

	while($str = fgets($fp)) {
		echo "$str";
	}
	fclose($fp);

