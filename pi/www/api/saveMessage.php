<?php

	include_once('../User.inc.php');
	include_once('../Message.inc.php');

	if(!isset($_REQUEST['message'])) {
		$r = array('error' => 'message not set');
	} else {
		$messageId = Message::saveMessage($_REQUEST['message']);
		$r = array("message" => $_REQUEST['message'], 'messageId' => $messageId);
		}
	}

	print json_encode($r);
?>
