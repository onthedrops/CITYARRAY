<?php

	include_once('../User.inc.php');
	include_once('../Message.inc.php');

	if(!isset($_REQUEST['message'])) {
		$r = array('error' => 'message not set');
	} else if(!isset($_REQUEST['signId']) || !is_numeric($_REQUEST['signId'])) {
		$r = array('error'=>'invalid sign id');
	} else {
		if(!User::getInstance()->owns($_REQUEST['signId'])) {
			$r = array('error' => 'user does not own sign');
		} else {
			$messageId = Message::saveMessage($_REQUEST['message']);
			User::setSignMessage($_REQUEST['signId'], $messageId);
			$r = array("message" => $_REQUEST['message'], 'signId' => $_REQUEST['signId'], 'messageId' => $messageId);
		}
	}

	print json_encode($r);
?>
