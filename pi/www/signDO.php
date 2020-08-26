<?php

	include('header.inc.php');
	
	if(isset($_REQUEST['id'])) {

		$signId = $_REQUEST['id'];
		$array = array();

		if(!User::getInstance()->owns($signId)) {
			$array['error'] = "<FONT COLOR=Red>Security error</FONT>";
		} else {
			$value = User::getInstance()->getMessage($signId);
			if(!$value) {
				$array['error'] = "<FONT COLOR=Red>No message set</FONT>";
			} else {
				$array['value'] = $value;
			}
		}	

		echo json_encode($array);
		exit(0);
	} else if(isset($_REQUEST['groupId'])) {
		$groupMessages = User::getInstance()->getSignGroupMessages($_REQUEST['groupId']);
		$myMessageId = 0;
		$myMesssage = "";
		$ok = 1;

		if(!sizeof($groupMessages)) {
			$array['error'] = "<FONT COLOR=Red>No messages set for group</FONT>";
			echo json_encode($array);
			exit(0);
		}

		
		foreach($groupMessages as $groupMessage) {
			if($myMessageId) {
				if($groupMessage['messageId'] != $myMessageId)
					$ok = 0;
			} else {
				$myMessageId = $groupMessage['messageId'];
				$myMessage = $groupMessage['message'];
			}
		}

		if($ok) {
			$array['value'] = $myMessage;
			echo json_encode($array);
			exit(0);
			
		} else {
			$array['error'] = "<FONT COLOR=Red>Group has different messages on different signs</FONT>";
			echo json_encode($array);
			exit(0);
		}

	}


	
