Pi Demo Kit V0.15
<HR>
<?php
	$fp = popen("ifconfig -a", "r");
	while($str = fgets($fp)) {
		echo "<BR>$str";
	}
?>
<HR>
<?php
	if(file_exists("/home/sign-firmware/pi/www/remote_support.txt")) {
		$fp = fopen("/home/sign-firmware/pi/www/remote_support.txt", "r");
		while($str = fgets($fp)) {
			echo "<BR>$str";
		}
	} else {
		echo "Remote support not active\n";
	}
?>
<HR>
<?php
	if(file_exists("/home/sign-firmware/pi/www/signd.txt")) {
		$fp = fopen("/home/sign-firmware/pi/www/signd.txt", "r");
		while($str = fgets($fp)) {
			echo "<BR>$str";
		}
	} else {
		echo "Signd signature file missing\n";
	}
	
?>
<HR>
<PRE>
<?php
	$fp = popen("ps auxw", "r");
	while($str = fgets($fp)) {
		echo "<BR>$str";
	}

