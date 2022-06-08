<?php

	include_once('lib/mysqlng.inc.php');
	include_once('firmware_version.php');

	$dbh = new SQLng();

	$error = null;

	if(!isset($_REQUEST['netId'])) {
		header($_SERVER['SERVER_PROTOCOL'] . ' 500 missing netId', true, 500);
		echo "Missing netId";
		exit(0);

	}

	if(!isset($_REQUEST['netpw'])) {
		header($_SERVER['SERVER_PROTOCOL'] . ' 500 missing netpw', true, 500);
		echo "Missing netpw";
		exit(0);
	}

	if(!isset($_REQUEST['signId'])) {
		header($_SERVER['SERVER_PROTOCOL'] . ' 500 missing signId', true, 500);
		echo "Missing signId";
		exit(0);
	}

	$netIdq = $dbh->equote($_REQUEST['netId']);
	$netpwq = $dbh->equote($_REQUEST['netpw']);
	$signIdq = $dbh->equote($_REQUEST['signId']);
	
	// step 1 - check netId and net password
	$dbh->Query($sql = "SELECT networkId FROM signNetworks WHERE networkId = $netIdq AND networkPassword = SHA1($netpwq)");
	$dbh->next_record();

	$netId = $dbh->f("networkId");

	if(!$netId) {
		header($_SERVER['SERVER_PROTOCOL'] . ' 500 bad netId or netpw', true, 500);
		echo "Bad netId or netpw";
		exit(0);
	}

	// step 2 - find sign via netId and signRemoteId
	$dbh->Query("SELECT signId, signIp, signVersion FROM signs WHERE networkId = $netId AND signRemoteId = $signIdq");
	$dbh->next_record();
	$signId = $dbh->f("signId");
	$signIp = $dbh->f("signIp");
	$signVersion = $dbh->f("signVersion");

	if(!$signId) {
		header($_SERVER['SERVER_PROTOCOL'] . ' 500 bad signId', true, 500);
		echo "Bad signId";
		exit(0);
	}

	if($signVersion != $_REQUEST['ver']) {
		$signVersionq = $dbh->equote($_REQUEST['ver']);
		$dbh->Query("UPDATE signs SET signVersion = $signVersionq WHERE signId = $signId");
	}

	if($_REQUEST['ver'] != firmware_version()) {
		$dbh->Query("SELECT configValue FROM signConfig WHERE signId = $signId AND configKey = 'auto'");
		$dbh->next_record();
		$auto = $dbh->f("configValue");

		if($auto) {
			echo '~!UPGRADE' . "\n";
			exit(0);
		}
	}
	
	
	if(!$signIp) {
		$signIpq = $dbh->equote($_SERVER['REMOTE_ADDR']);
		$dbh->Query("UPDATE signs set signIp = $signIpq WHERE signId = $signId");
	}

	// step 3 - find messageId for sign

	$seqMode = 0;

	if(isset($_REQUEST['seq'])) {
		// specialized sleep version
		// enable seq mode

		$seqMode = 1;
		// step 1: if seq is <= our last seen seq,
		// fall through immediately
		$dbh->Query("SELECT sequenceId, messageId FROM signSequence WHERE signId = $signId");
		$dbh->next_record();

		$lastSeenSequence = $dbh->f("sequenceId");
		$lastSeenMessage = $dbh->f("messageId");
			
		if($lastSeenSequence == null || ($_REQUEST['seq'] > $lastSeenSequence)) {

			// check to see if messageId is different than last
			// transmitted message
			$continue = 1;
			$count = 0;

			while($continue) {

				$currentMessage = $dbh->selectOne("SELECT messageId FROM signMessage WHERE signId = $signId");

				if($lastSeenMessage == null || $lastSeenMessage != $currentMessage) {
					$continue = 0;
				} 

				if($count++ == 3000) {
					header($_SERVER['SERVER_PROTOCOL'] . ' 204 no new content', true, 204);
					exit(0);
				}
				
				usleep(100000);
			}

		}

	}

	
	$dbh->Query("SELECT messageId, firstShownDate FROM signMessage WHERE signId = $signId");
	$dbh->next_record();
	if(!$dbh->f("messageId")) {
		header($_SERVER['SERVER_PROTOCOL'] . ' 500 no current message', true, 500);
		exit(0);
	}


	$messageId = $dbh->f("messageId");
	$additionalSQL = "";

	if(!$dbh->f("firstShownDate")) {
		$additionalSQL = ",firstShownDate=NOW()";
	}	
	
	$dbh->Query("UPDATE signMessage SET lastShownDate=NOW() $additionalSQL");


	if($seqMode) {
		$seq = intval($_REQUEST['seq']);
		$dbh->Query($sql = "REPLACE INTO signSequence SET sequenceId=$seq, messageId=$messageId,signId = $signId");
	} 

	$message = $dbh->selectOne("SELECT message FROM messages WHERE messageId = $messageId");
	echo $message;
