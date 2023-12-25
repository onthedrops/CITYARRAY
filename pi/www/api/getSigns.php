<?php

	include_once('../User.inc.php');
	$r = User::getInstance()->getSigns();

	print json_encode($r);
?>
