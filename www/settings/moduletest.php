<?php
require_once '../load.php';

$g_title = 'Module Test';
$err = 0;

// If form has been posted, process the request
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
	if (strlen($_POST["rescan"]) !== 0)
	{
		// User requested a bus rescan
		$data = icc_sendreceive("DO_RESCAN\n");
		if ($data === "") { redirect ($g_root . '/error_connect.php'); }
		if ($data[0] === '-') { redirect ($g_root . '/error_comm.php'); }
		sleep(2);
	}
	else
	{
		$address = (int)$_POST["address"];
		if (strlen($_POST["save"]) !== 0)
		{
			// User saved new settings for a module
			$newaddress = (int)$_POST["newaddress"];
			$newtype = (int)$_POST["newtype"];
			$newdesc = $_POST["description"];
			if ($address === 0x77)
			{
				// User saved settings for a new module
				if ($newaddress === 0x77 || $newtype === 0) { $err = 1; }
				else
				{
					$data = icc_sendreceive("SET_ADDR " . dechex($newaddress) . "\n");
					if ($data[0] === '-') { $err = 2; }
					else
					{
						$data = icc_sendreceive("SET_TYPE " . dechex($newtype) . "\n");
						if ($data[0] === '-') { $err = 2; }
						else
						{
							$data = icc_sendreceive("DO_SOFTRESET 77\n");
							if ($data[0] === '-') { $err = 2; }
							sleep(4);
							$data = icc_sendreceive("DO_RESCAN\n");
							if ($data[0] === '-') { $err = 2; }
							sleep(2);
						}
					}
				}
			}
			else
			{
				// User saved settings for an existring module (description)
				if (strlen($newdesc) === 0 || strlen($newdesc) > 14) { $err = 3; }
				else if (strpos($newdesc, ' ') !== FALSE) { $err = 3; }
				else
				{
					$data = icc_sendreceive("SET_DESC " . dechex($address) . ' ' . $newdesc . "\n");
					if ($data[0] === '-') { $err = 2; }
					sleep(2);
					$data = icc_sendreceive("DO_RESCAN\n");
					if ($data[0] === '-') { $err = 2; }
					sleep(2);
				}
			}
		}
		else if (strlen($_POST["softreset"]) !== 0)
		{
			// User requested softreset
			$data = icc_sendreceive("DO_SOFTRESET " . dechex($address) . "\n");
			if ($data[0] === '-') { $err = 2; }
			sleep(4);
			$data = icc_sendreceive("DO_RESCAN\n");
			if ($data[0] === '-') { $err = 2; }
			sleep(2);
		}
		else if (strlen($_POST["hardreset"]) != 0)
		{
			// User requested hardreset
			$data = icc_sendreceive("DO_HARDRESET " . dechex($address) . "\n");
			if ($data[0] === '-') { $err = 2; }
			sleep(4);
			$data = icc_sendreceive("DO_RESCAN\n");
			if ($data[0] === '-') { $err = 2; }
			sleep(2);
		}
	}
}

$addr = $_GET["addr"];
$data = icc_sendreceive("GET_MODULE " . dechex($addr) . "\n");
//echo $data; /////////////////////////////////////////////////// DEBUG
if ($data === "") { redirect ($g_root . '/error_connect.php'); }
if (substr($data, 0, 3) === "-KO") { redirect ($g_root . '/error_comm.php'); }

$modules = read_moduleident_list($data);

//if (is_null($modules)) { redirect ($g_root . '/error_comm.php'); }

$module = array_values($modules)[0];

require '../header.php';
?>
<section class="content">
	<div class="wrapper">
		<article class="article page">
<?php if ($err === 1) { ?>
			<p class="invalid"><strong>Error:</strong> Please select an address other than 0x77 and a valid type!</p>
<?php } else if ($err === 2) { ?>
			<p class="invalid"><strong>Error:</strong> An error occured when applying changes. Please try again or contact support if the error persists.</p>
<?php } else if ($err === 3) { ?>
			<p class="invalid"><strong>Error:</strong> Please enter a valid description (at least 1 and at most 14 characters, no white space).</p>
<?php }?>

			<h1>0x<?php echo dechex($module->Address) ?> (<?php echo($types[$module->Type]) ?>): <?=$module->Description ?></h1>
			<p>&nbsp;</p>
			<p>This page lets you test the module via basic commands.</p>
<?php
$data = icc_sendreceive("GET_STATUS " . dechex($addr) . "\n");
echo $data;
?>			
			<table border="0" cellspacing="0">
				<tr>
					<th>Output</th>
					<th>Status</th>
					<th>&nbsp;</th>
				</tr>
<?php		for ($out_id = 0; $out_id < 16; $out_id++)
			{ ?>
				<tr>
					<td><?php echo dechex($out_id); ?></td>
					<td><?php if (substr($data, 4 + ($out_id * 2), 1) === "1") { echo "ON"; } else { echo "OFF"; } ?></td>
					<td>&nbsp;</td>
				</tr>
<?php		} ?>
		</article>
	</div>
</section>
<?php require '../footer.php'; ?>
