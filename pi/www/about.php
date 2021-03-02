Pi Demo Kit V0.15 
<?php
	include_once('firmware_version.php');
	$fp = fopen("/home/sign-firmware/pi/www/git.txt", "r");
	$gitmd = fgets($fp);
	fclose($fp);

	echo " $gitmd <BR>Firmware version: " . firmware_version() . "<BR><HR>";
	
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

		fclose($fp);
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

		fclose($fp);
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
	fclose($fp);

