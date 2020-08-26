<?php

	require('header.inc.php');
	

	
	//print_r($_REQUEST);	
	// display list of signs
	if(isset($_REQUEST['AddPreset'])) {
		User::getInstance()->addPreset($_REQUEST['PresetName']);
	}

	if(isset($_REQUEST['Activate'])) {
		$presetId = $_REQUEST['presetId'];
		$signMessageArray = User::getInstance()->getPreset($presetId);
		foreach ($signMessageArray as $signId => $signData) {
			User::setSignMessage($signId, $signData['messageId']);
		}
	}

	if(isset($_REQUEST['Save'])) {
		$signArray = User::getInstance()->getSigns();
		$presetId = $_REQUEST['presetId'];
		foreach ($signArray as $signId => $signName) {
			$signValue = $_REQUEST['set' . $signId];
			User::getInstance()->setPresetValue($presetId, $signId, $signValue);
		}
	}


	if(isset($_REQUEST['Submit'])) {
		$messageId = Message::saveMessage($_REQUEST['taMsg']);
		// if a individual sign is selected, set that sign to this message
		// if a group of signs is selected, set those signs to this message
		if(isset($_REQUEST['selIndividual']) && $_REQUEST['selIndividual']) {
			User::setSignMessage($_REQUEST['selIndividual'], $messageId);
		}

		if(isset($_REQUEST['selGroup']) && $_REQUEST['selGroup']) {
			User::setSignGroupMessage($_REQUEST['selGroup'],$messageId);
		}
	}
	
?>
<FORM ACTION="/index.php" METHOD="POST">
	<TABLE>
	<TD>Individual sign</TD><TD>
		<SELECT NAME="selIndividual">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
	
		$signList = User::getInstance()->getSigns();
		foreach($signList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
		</SELECT></TD><TD>Group of signs</TD><TD><SELECT NAME="selGroup">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
		$groupList = User::getInstance()->getSignGroups();
		foreach($groupList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
	</SELECT>
	</TD></TR>
	<TR><TD COLSPAN=4><TEXTAREA NAME="taMsg" ROWS="10" COLS="60"> </TEXTAREA></TD></TR>
	<TR><TD></TD><TD></TD><TD></TD><TD ALIGN="RIGHT"><INPUT NAME="Submit" TYPE="SUBMIT" /></TD></TR>
	</TABLE>
	</FORM>

	<?php showPresets(); ?>
<?php

function showPresets() {
	// get list of presets for this user
	// add preset button
	echo "<TABLE BGCOLOR=#333333>";
	$presets = User::getInstance()->getPresets();
	$rowCount = 0;
	foreach ($presets as $presetId => $presetName) {
		if(!($rowCount++ % 2)) {
			$altColor="#660066";
		} else {
			$altColor="#006600";
		}

		echo "<TR><TD BGCOLOR=$altColor><FONT COLOR=White>$presetName</FONT></TD><TD>";
		showPreset($presetId);
		echo "</TD></TR>";
	}
	echo "<TR><TD><FONT COLOR=White>Name:</FONT><FORM METHOD=POST><INPUT NAME=PresetName TYPE=TEXT SIZE=40></TD><TD><INPUT VALUE=\"Add Preset\" TYPE=Submit Name=\"AddPreset\"/></FORM></TD></tR>";
	echo "</TABLE>";

}

function showPreset($presetId) {
	// pull preset contents
	// get list of signs for this user

	// for each sign, get message to display (0 = no change)
	$signArray = User::getInstance()->getSigns();
	$messageArray = User::getInstance()->getMessages();
	$signMessageArray = User::getInstance()->getPreset($presetId);
	
	//print_r($signMessageArray);
	echo "<FORM METHOD=POST>";
	foreach ($signArray as $signId => $signName) {
		
		echo "&nbsp;&nbsp;&nbsp;<FONT COLOR=White>$signName -- </FONT>";
		echo "<INPUT TYPE=Hidden NAME=presetId VALUE=$presetId>";

		if($signMessageArray[$signId]['messageId'] == 0) 
				$sl = "SELECTED";
			else	$sl = "";

		echo "<SELECT NAME=set" . $signId . "><OPTION VALUE=0 $sl>Not included in this preset</OPTION>";
	
		foreach ($messageArray as $messageId => $message) {

			if($signMessageArray[$signId]['messageId'] == $messageId) 
				$sl = "SELECTED";
			else	$sl = "";

			echo "<OPTION VALUE=$messageId $sl>" . $message . "</OPTION>";
		}
		echo "</SELECT><BR>";
	}

	echo "<TABLE><TR>";
	echo "<TD><INPUT TYPE=Submit VALUE=Save NAME=Save></TD>";
	echo "<TD ALIGN=Right><INPUT TYPE=Submit VALUE=Activate NAME=Activate></TD></TR></TABLE></FORM>";
	
}

