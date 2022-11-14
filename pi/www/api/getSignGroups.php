<?php

	include_once('../User.inc.php');
	$r = User::getInstance()->getSignGroups();

	print json_encode($r);
?>
