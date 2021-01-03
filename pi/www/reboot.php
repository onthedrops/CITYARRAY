<?php
	include_once('User.inc.php');

	echo "Rebooting Pi";

	 User::getInstance()->requestShutdown(3);
?>
