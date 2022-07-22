<?php

	$file = '/var/log/program.log';

        if(isset($_REQUEST['position'])) {
                $position = $_REQUEST['position'];
        } else {
                $position = 0;
        }

 if(file_exists($file)) {
                $fp = fopen($file, "rb");
        } else {
                $fp = null;
        }

        if($fp) {
                if($position) {
                        fseek($fp, $position, SEEK_SET);
                }

		$done = 0;
                while($str = fgets($fp)) {
                        if(strlen($str) > 160) {
                                $str = preg_replace("/(.{160})/", "$1\n\r", $str);
                        }
                        $buf .= htmlentities($str);
			if(strstr($str,"EOF")) {
				$done = 1;		
			}
                }

                $endPosition = ftell($fp);
        } else {
                $endPosition = 0;
        }


        $return = array('bytes' => $buf, 'pos' => $endPosition, 'status' => $status, 'cancellable' => $cancellable, 'statusHTML' => $statusHTML, 'statusText' => $statusText, 'done' => $done);
        echo json_encode($return);
