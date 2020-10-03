<?php

	require_once('User.inc.php');


	User::getInstance()->setUserId(1);

	if(isset($_REQUEST['contact']) && $_REQUEST['contact'] == 1) {
		$presetId = User::getInstance()->getPresetId('drycontact');
                $signMessageArray = User::getInstance()->getPreset($presetId);
                foreach ($signMessageArray as $signId => $signData) {
                        if($signData['messageId'] != 0)
                                User::setSignMessage($signId, $signData['messageId']);
                }
	} else if(isset($_REQUEST['contact']) && $_REQUEST['contact'] == 0) {
		$dbh = User::getInstance()->getDBH();
		$presetId = User::getInstance()->getPresetId('drycontact');
                $signMessageArray = User::getInstance()->getPreset($presetId);
		foreach ($signMessageArray as $signId => $signData) {
			$messageId = $dbh->selectOne("SELECT messageId FROM signMessageArchive WHERE signId = $signId ORDER BY signArchiveId DESC LIMIT 2,1");
			if($messageId) {
                                User::setSignMessage($signId, $messageId);
			}
		}

	}
?>
