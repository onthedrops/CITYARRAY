<?php
	echo "Shutting down Pi";

	 User::getInstance()->requestShutdown();
?>
