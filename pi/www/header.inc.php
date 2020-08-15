<?php

require_once('User.inc.php');

if (session_status() !== PHP_SESSION_ACTIVE) {session_start();}

if($_SESSION['userId']) {
	$user = User::getInstance();
	$user->setUser($_SESSION['userId']);
} else {
	$_SESSION['return'] = $_SERVER['REQUEST_URI'];
	header('Location: ' . $BASE . '/login.php');
	exit(0);
}

function noCache() {
        header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
        header("Cache-Control: no-store, no-cache, must-revalidate");
        header("Cache-Control: post-check=0, pre-check=0", false);
        header("Pragma: no-cache");
}
