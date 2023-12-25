<?php

	include_once('../User.inc.php');
	if(!isset($_REQUEST['signId']) || !is_numeric($_REQUEST['signId'])) {
		$r = array('error'=>'invalid sign id');
	} else {
		$message = User::getInstance()->getMessage($_REQUEST['signId']);
		$r = array('signId' => $_REQUEST['signId'], 'message' => $message);
	}

	print json_encode($r);
?>
