<?php

        require('header.inc.php');
        include_once('firmware_version.php');

	$e = net_get_interfaces();

	foreach ($e['eth0']['unicast'] as $a) {
		if($a['family'] == 2) {
			echo "<A HREF=http://" . $a['address']. ">Ethernet interface</A>";
		}
	}
?>



