<?php
	include_once('User.inc.php');

	echo "Enabling remote tech support";

	 User::getInstance()->requestShutdown(5);
?>
