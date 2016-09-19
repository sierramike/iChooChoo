<?php

class ModuleIdent {
	public $InfoPresent;
	public $Major;
	public $Minor;
	public $Build;
	public $Address;
	public $Type;
	public $Description;
}

$types = array(0x10 => 'Traction S',
               0x11 => 'Traction L',
               0x20 => 'General purpose',
               0x21 => 'Lighting');

function redirect($url, $permanent = false)
{
//	if (headers_sent() === false)
	{
		header('Location: ' . $url, true, ($permanent === true ? 301 : 302));
	}
	die();
}

function icc_sendreceive($message)
{
	global $icc_host, $icc_port;

	$returnvalue = "";

	error_reporting(E_ALL);

	$address = gethostbyname($icc_host);

	$client = stream_socket_client("tcp://$address:$icc_port", $errno, $errmessage);
	if (!($client === false)) {
		fwrite($client, $message);
		$returnvalue = stream_get_contents($client);
		fclose($client);
	}

	return $returnvalue;
}

function read_moduleident_list($data)
{
	$modules = array();

	if (strncmp($data, "+OK", 3) == 0)
	{
		sscanf($data, "+OK %d %n", $nbmodules, $bytesread);
		$rest = substr($data, $bytesread);
		for ($i = 0; $i < $nbmodules; $i++)
		{
			$item = new ModuleIdent();
			$item->InfoPresent = false;
			sscanf($rest, "%02x %d.%d.%d %02x %s %n", $item->Address, $item->Major, $item->Minor, $item->Build, $item->Type, $item->Description, $bytesread);
			if ($item->Major !== 0 || $item->Minor !== 0 || $item->Build !== 0) { $item->InfoPresent = true; }
			if ($item->Description === "?") { $item->Description = ""; }
			$modules[$item->Address] = $item;
//			array_push($modules, $item);
			$rest = substr($rest, $bytesread);
		}
		return $modules;
	}
	else
		return NULL;
}
?>
