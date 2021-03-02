<?php

	require('header.inc.php');
	include_once('firmware_version.php');
	

	
	//print_r($_REQUEST);	
	// display list of signs
	if(isset($_REQUEST['AddPreset'])) {
		User::getInstance()->addPreset($_REQUEST['PresetName']);
	}

	if(isset($_REQUEST['Shutdown'])) {
		User::getInstance()->requestShutdown();
	}

	if(isset($_REQUEST['Reboot'])) {
		User::getInstance()->requestShutdown(3);
	}

	if(isset($_REQUEST['Activate'])) {
		$presetId = $_REQUEST['presetId'];
		$signMessageArray = User::getInstance()->getPreset($presetId);
		foreach ($signMessageArray as $signId => $signData) {
			if($signData['messageId'] != 0)
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
	
	if(isset($_REQUEST['Delete'])) {
		$presetId = $_REQUEST['presetId'];
		User::getInstance()->deletePreset($presetId);
	}

	if(isset($_REQUEST['Remove'])) {
		$messageId = $_REQUEST['messageId'];
		User::getInstance()->deleteMessage($messageId);
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
<script>
	var xmlReq = new XMLHttpRequest();
	var xmlCbReq = new XMLHttpRequest();


	function groupChange(e) {
		var value = document.getElementById('selGroup').value;

	 	xmlReq.onreadystatechange = onStatusChange;
                xmlReq.open('POST', '/signDO.php?groupId=' + value, true)
		xmlReq.withCredentials=true;
		xmlReq.send();
	}

	function individualChange(e) {
		var value = document.getElementById('selIndividual').value;

	 	xmlReq.onreadystatechange = onStatusChange;
                xmlReq.open('POST', '/signDO.php?id=' + value, true)
		xmlReq.withCredentials=true;
		xmlReq.send();
	}

	function onStatusChange() {
		   if(xmlReq.readyState == 4) {
                        if(xmlReq.status == 200) {
				var res = JSON.parse(xmlReq.responseText);
				if(res.error) {
					var d = document.getElementById('stdout');
					d.innerHTML = res.error;
				} else {
					var d = document.getElementById('taMsg');
					d.value = res.value;
				}
			}
		  }
	}
</script>
<FORM ACTION="/index.php" METHOD="POST">
	<DIV ID=stdout></DIV>
	<TABLE>
	<TD>Individual sign</TD><TD>
		<SELECT ID="selIndividual" NAME="selIndividual" onchange="individualChange()">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
	
		$signList = User::getInstance()->getSigns();
		foreach($signList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
		</SELECT></TD><TD>Group of signs</TD><TD><SELECT ID="selGroup" NAME="selGroup" onchange="groupChange()">
			<OPTION VALUE="0" SELECTED>None</OPTION>
<?php
		$groupList = User::getInstance()->getSignGroups();
		foreach($groupList as $id => $name) {
			echo "\t\t<OPTION VALUE=\"$id\">$name</OPTION>\n";
		}

?>
	</SELECT>
	</TD></TR>
	<TR><TD COLSPAN=4><TEXTAREA ID="taMsg" NAME="taMsg" ROWS="10" COLS="60"></TEXTAREA></TD></TR>
	<TR><TD></TD><TD></TD><TD></TD><TD ALIGN="RIGHT"><INPUT NAME="Submit" TYPE="SUBMIT" /></TD></TR>
	</TABLE>
	</FORM>


	<?php showPresets(); ?>
	<?php showSigns(); ?>

	<FORM METHOD=Post>
		<INPUT TYPE="SUBMIT" NAME="Shutdown" VALUE="Shutdown"/>
		<INPUT TYPE="SUBMIT" NAME="Reboot" VALUE="Reboot"/>
	</FORM>
<?php

function showSigns() {
	// show signs for this user, including current version 
	// and automatic update checklist

	echo "<BR><BR><TABLE>";
	echo "Current firmware version: " . firmware_version();
	$signList = User::getInstance()->getSigns();
	foreach($signList as $id => $name) {
		$ver = User::getInstance()->getSignVersion($id);
		$auto = User::getInstance()->getSignConfig($id, 'auto');
		if($auto) {
			$autocb = "CHECKED";
		} else {
			$autocb = "";
		}

		$cbid = "cb_" . $id;

		echo "<TR BGCOLOR=#999999><TD>$name</TD><TD>sign running version: $ver</TD><TD>Automatic update: <INPUT TYPE=Checkbox ID=$cbid onchange=\"autoUpdateChange($id, $cbid)\" $autocb> </TD></TR>\n";
	}
	echo "</TABLE>";
}

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
	echo "<TD ALIGN=Right><INPUT TYPE=Submit VALUE=Activate NAME=Activate><?TD><TD>";
	echo "<INPUT TYPE=SUbmit VALUE=Delete NAME=Delete></TD></TR></TABLE></FORM>";
}
echo "</TABLE>";

	echo "Remove: <FORM><SELECT NAME=messageId>";
	
	$messageArray = User::getInstance()->getMessages();
	foreach ($messageArray as $messageId => $message) {
			echo "<OPTION VALUE=$messageId>" . $message . "</OPTION>";
	}	
	echo "</SELECT><INPUT TYPE=Submit VALUE=Remove NAME=Remove>";
	echo "</FORM>";
	
?>


<script>
	function autoUpdateChange(id, cbid)
	{
		var value = 0; 
		if(cbid.checked)
			value = 1;

                xmlCbReq.open('POST', '/signConfigDO.php?signId=' + id + '&key=auto&value=' + value, true)
		xmlCbReq.withCredentials=true;
		xmlCbReq.send();
		
	}

</script>


