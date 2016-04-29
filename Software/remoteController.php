<?php
	include("menu.php");
	if (isset($_GET['go'])) {
		$cmd = 'c_program/robot go 30 1 30 1';
		$escaped_command = escapeshellcmd($cmd);
		exec($escaped_command,$out);
		echo "go<br>";
		print_r($out);
		echo "<br>";
	} else if (isset($_GET['stop'])) {
		$cmd = 'c_program/robot stop';
		$escaped_command = escapeshellcmd($cmd);
		exec($escaped_command,$out);
		echo "stop<br>";
		print_r($out);
		echo "<br>";
	} else if (isset($_GET['turn_left'])) {
		$cmd = 'c_program/robot go 30 1 30 0';
		$escaped_command = escapeshellcmd($cmd);
		exec($escaped_command,$out);
		echo "go<br>";
		print_r($out);
		echo "<br>";
	} else if (isset($_GET['turn_right'])) {
		$cmd = 'c_program/robot go 30 0 30 1';
		$escaped_command = escapeshellcmd($cmd);
		exec($escaped_command,$out);
		echo "go<br>";
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
		<button type="button" onclick="location.href='remoteController.php?turn_left'">turn_left</button>
		<button type="button" onclick="location.href='remoteController.php?turn_right'">turn_right</button>
	</body>
</html>