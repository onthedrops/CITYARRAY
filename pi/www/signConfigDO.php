<?php

	include('header.inc.php');
	
	if(isset($_REQUEST['signId'])) {

		$signId = $_REQUEST['signId'];
		$array = array();

		if(!User::getInstance()->owns($signId)) {
			$array['error'] = "<FONT COLOR=Red>Security error</FONT>";
		} else {
	
			User::getInstance()->setSignConfig($signId, $_REQUEST['key'], $_REQUEST['value']);

		}	

		echo json_encode($array);
		exit(0);
	} 
