<?php

require_once('lib/mysqlng.inc.php');
require_once('User.inc.php');

if(isset($_POST['username'])) {

	$username = $_POST['username'];
        $password = $_POST['password'];

        $error = null;
        $sql = new SQLng();

	$passq = $sql->equote($password);
	$userq = $sql->equote($username);
	
        $query = "SELECT userId FROM users WHERE userName = $userq AND userPassword = SHA1($passq)";
	
$sql->Query($query);
$sql->next_record();

if($sql->f("userId")) {
	session_start();

	$_SESSION['userId'] = $sql->f('userId');
	if($_SESSION['return']) {
		$return = $_SESSION['return'];
		$_SESSION['return'] = null;
	} else {
		$return = "/";
	}

	header('Location: ' . $return);
}
}
?>
<h1>Log into NEIOSYS sign management interface</h2>
<FORM METHOD=POST>
<TABLE>
<TR><TD>
Username:</TD><TD><INPUT TYPE=text COLS=40 NAME=username /></TD></TR>
<TR><TD>
Password:</TD><TD><INPUT TYPE=password COLS=40 NAME=password /></TD></TR>
<TR><TD></TD><TD><INPUT TYPE=Submit NAME=Submit /></TD></TR>
</TABLE>
</FORM>
