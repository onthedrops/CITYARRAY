<?php
        require('header.inc.php');
	include_once('User.inc.php');

	echo "Programming sign";

	User::getInstance()->requestShutdown(6);
	sleep(4);
?>

<script>
        var textRequest = new XMLHttpRequest();
        var position = 0;
        var done = 0;
        var buffer = "";


function initLogRequest() {
                                textRequest.onreadystatechange = handleTextRequest;

                                textRequest.open('GET','/logDO.php?fileType=1&position=' + position, true);
                                textRequest.send();
        }

function handleTextRequest() {

                if(textRequest.readyState == 4) {
                        if(textRequest.status == 200) {
                                var json = JSON.parse(textRequest.responseText)
                                var div = document.getElementById("textDiv");
				if(json.bytes) {

	                                buffer = buffer + json.bytes;

					div.innerHTML = '<PRE>' +  buffer + '</PRE>';
				}

                          	position = json.pos;
				if(json.done)
					done = 1;

                                if(done == 0)
                                        setTimeout(reload, 1000);
                        }
		}
}

function init() {
                initLogRequest();
        }

        function reload() {
                initLogRequest();
        }

        window.onload = init;

</script>

<DIV ID="textDiv">Waiting for data.. </DIV>
