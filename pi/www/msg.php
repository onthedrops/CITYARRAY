<?php

	include_once('lib/mysqlng.inc.php');

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
	$signIp = $dbh->f("sighIp");
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

	if($_REQUEST['ver'] != '0.15') {
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

	$message = $dbh->selectOne("SELECT message FROM messages WHERE messageId = $messageId");
	echo $message;
