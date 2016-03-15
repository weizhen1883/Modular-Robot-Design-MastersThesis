<?php
	include("menu.php");
	if (isset($_GET['go'])) {
		exec("/var/www/c_program/robot go",$out);
		echo "go<br>";
		print_r($out);
		echo "<br>";
	} else if (isset($_GET['stop'])) {
		exec("/var/www/c_program/robot stop",$out);
		echo "stop<br>";
		print_r($out);
		echo "<br>";
	}
?>
<html>
	<head>
		<title>Remote Controller</title>
	</head>
	<body>
		<button type="button" onclick="location.href='remoteController.php?go'">go</button>
		<button type="button" onclick="location.href='remoteController.php?stop'">stop</button>
	</body>
</html>