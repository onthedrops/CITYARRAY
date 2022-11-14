<?php

	include_once('../User.inc.php');
	include_once('../Message.inc.php');

	if(!isset($_REQUEST['message'])) {
		$r = array('error' => 'message not set');
	} else if(!isset($_REQUEST['groupId']) || !is_numeric($_REQUEST['groupId'])) {
		$r = array('error'=>'invalid group id');
	} else {
		if(!User::getInstance()->ownsGroup($_REQUEST['groupId'])) {
			$r = array('error' => 'user does not own group');
		} else {
			$messageId = Message::saveMessage($_REQUEST['message']);
			User::setSignGroupMessage($_REQUEST['groupId'], $messageId);
			$r = array("message" => $_REQUEST['message'], 'groupId' => $_REQUEST['groupId'], 'messageId' => $messageId);
		}
	}

	print json_encode($r);
?>
