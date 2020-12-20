<?php
	echo "Rebooting down Pi";

	 User::getInstance()->requestShutdown(3);
?>
