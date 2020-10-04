<?php

	require_once('User.inc.php');


	User::getInstance()->setUserId(2);

	if(isset($_REQUEST['contact']) && $_REQUEST['contact'] == 1) {
		$presetId = User::getInstance()->getPresetId('drycontact');
                $signMessageArray = User::getInstance()->getPreset($presetId);
                foreach ($signMessageArray as $signId => $signData) {
                        if($signData['messageId'] != 0)
                                User::setSignMessage($signId, $signData['messageId']);
                }
		echo "Setting dry contact message\n";
	} else if(isset($_REQUEST['contact']) && $_REQUEST['contact'] == 0) {
		$dbh = User::getInstance()->getDBH();
		$presetId = User::getInstance()->getPresetId('drycontact');
                $signMessageArray = User::getInstance()->getPreset($presetId);
		foreach ($signMessageArray as $signId => $signData) {
			$notMessageId = $signData['messageId'];
			$messageId = $dbh->selectOne("SELECT messageId FROM signMessageArchive WHERE signId = $signId AND messageId != $notMessageId ORDER BY signArchiveId DESC");
			if($messageId) {
                                User::setSignMessage($signId, $messageId);
			}
		}
		echo "Clearing dry contact message\n";

	}
?>
