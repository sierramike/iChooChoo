<html>
<head>
<title>Test</title>
</head>
<body>
DO_RESCAN:<span>
<?php
error_reporting(E_ALL);

$port=999;
$address = gethostbyname('localhost');
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
	echo "socket_create() failed: reason: " . socket_strerror(socket_lasterror()) . "\n";
}

echo "Attempting to connect to '$address' on port '$port' ...";
$result = socket_connect($socket, $address, $port);
if ($result === false) {
	echo "socket_connect() failed: reason: " . socket_strerror(socket_lasterror($socket)) . "\n";
}

$in = "DO_RESCAN\n";
$out = "";

echo "Sending data ...";
socket_write($socket, $in, strlen($in));

echo "Reading response:";
while ($out = socket_read($socket, 2048)) {
	echo $out;
}

socket_close($socket);
?>
</span>
</body>
</html>
