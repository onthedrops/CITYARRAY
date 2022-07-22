<?php

require_once('User.inc.php');
require_once('Message.inc.php');

if (session_status() !== PHP_SESSION_ACTIVE) {session_start();}

if($_SESSION['userId']) {
	$user = User::getInstance();
	$user->setUserId($_SESSION['userId']);
} else {
	$_SESSION['return'] = $_SERVER['REQUEST_URI'];
	header('Location: ' . $BASE . '/login.php');
	exit(0);
}

drawHeader();

function noCache() {
        header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
        header("Cache-Control: no-store, no-cache, must-revalidate");
        header("Cache-Control: post-check=0, pre-check=0", false);
        header("Pragma: no-cache");
}

function drawHeader() {
	echo "<A HREF=/>Main</A>&nbsp;&nbsp;";

	if(is_dir('/home/pi')) {
		echo "<A HREF=/configSign.php>Configure Signs</A>&nbsp;&nbsp;<A HREF=/pollSign.php>Debug Signs</A>&nbsp;&nbsp;<A HREF=/update.php>Update</A>&nbsp;&nbsp;<A HREF=/log.php>Logs</A>&nbsp;&nbsp;<A HREF=/remote.php>Remote Support</A>";
	}
		
	echo "<HR>";

}

