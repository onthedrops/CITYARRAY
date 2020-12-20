<?php
	include_once('User.inc.php');

	echo "Shutting down Pi";

	 User::getInstance()->requestShutdown();
?>
