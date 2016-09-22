<?php
require_once '../load.php';

$g_title = 'Module Test';
$err = 0;

$addr = $_GET["addr"];

// If form has been posted, process the request
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
	if (strlen($_POST["toggleon"]) !== 0)
	{
		$output = substr($_POST["toggleon"], 20);
		$data = icc_sendreceive("DO_SETOUT " . dechex($addr) . " " . $output . " 1\n");
		if ($data === "") { redirect ($g_root . '/error_connect.php'); }
		if ($data[0] === '-') { redirect ($g_root . '/error_comm.php'); }
	}
	else if (strlen($_POST["toggleoff"]) !== 0)
	{
		$output = substr($_POST["toggleoff"], 20);
		$data = icc_sendreceive("DO_SETOUT " . dechex($addr) . " " . $output . " 0\n");
		if ($data === "") { redirect ($g_root . '/error_connect.php'); }
		if ($data[0] === '-') { redirect ($g_root . '/error_comm.php'); }
	}
	else if (strlen($_POST["dimset"]) !== 0)
	{
		$output = substr($_POST["dimset"], 10);
		$data = icc_sendreceive("DO_SETDIMOUT " . dechex($addr) . " " . $output . " " . $_POST["value" . $output] . "\n");
		if ($data === "") { redirect ($g_root . '/error_connect.php'); }
		if ($data[0] === '-') { redirect ($g_root . '/error_comm.php'); }
	}
	else if (strlen($_POST["rescan"]) !== 0)
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

$data = icc_sendreceive("GET_MODULE " . dechex($addr) . "\n");
//echo $data; /////////////////////////////////////////////////// DEBUG
if ($data === "") { redirect ($g_root . '/error_connect.php'); }
if (substr($data, 0, 3) === "-KO") { redirect ($g_root . '/error_comm.php'); }

$modules = read_moduleident_list($data);
if (is_null($modules)) { redirect ($g_root . '/error_comm.php'); }
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
if ($data === "") { redirect ($g_root . '/error_connect.php'); }
if (substr($data, 0, 3) === "-KO") { redirect ($g_root . '/error_comm.php'); }

$status = array();
sscanf($data, "+OK %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", $status[0], $status[1], $status[2], $status[3],
	$status[4], $status[5], $status[6], $status[7],
	$status[8], $status[9], $status[10], $status[11],
	$status[12], $status[13], $status[14], $status[15]);
?>

<?php if ($module->Type === 0x20) { ?>
			<form method="post" id="frm">
			<table border="0" cellspacing="0">
				<tr>
					<th>Output</th>
					<th class="statuscol">Status</th>
					<th>&nbsp;</th>
				</tr>
<?php		for ($out_id = 0; $out_id < 16; $out_id++)
			{ ?>
				<tr>
					<td><?php echo strtoupper(dechex($out_id)); ?></td>
					<?php if ($status[$out_id] == 1) { ?>
						<td class="statuscol"><span class="status_on">ON</span></td>
						<td><input type="submit" name="toggleoff" value="Set to OFF - Output <?php echo strtoupper(dechex($out_id)); ?>"></td>
					<?php } else { ?>
						<td class="statuscol"><span class="status_off">OFF</span></td>
						<td><input type="submit" name="toggleon" value="Set to ON  - Output <?php echo strtoupper(dechex($out_id)); ?>"></td>
					<?php } ?>
				</tr>
<?php		} ?>
			</table>
			</form>
<?php }
      if ($module->Type === 0x21) { ?>
			<form method="post" id="frm">
			<table border="0" cellspacing="0">
				<tr>
					<th>Output</th>
					<th class="statuscol">Status</th>
					<th>&nbsp;</th>
				</tr>
<?php		for ($out_id = 0; $out_id < 10; $out_id++)
			{ ?>
				<tr>
					<td><?php echo strtoupper(dechex($out_id)); ?></td>
					<?php if ($status[$out_id] == 1) { ?>
						<td class="statuscol"><span class="status_on">ON</span></td>
						<td><input type="submit" name="toggleoff" value="Set to OFF - Output <?php echo strtoupper(dechex($out_id)); ?>"></td>
					<?php } else { ?>
						<td class="statuscol"><span class="status_off">OFF</span></td>
						<td><input type="submit" name="toggleon" value="Set to ON  - Output <?php echo strtoupper(dechex($out_id)); ?>"></td>
					<?php } ?>
				</tr>
<?php		}
     		for ($out_id = 10; $out_id < 16; $out_id++)
     		{
     			$out_id_letter = strtoupper(dechex($out_id)); ?>
<script>
$( function() {
	var handle = $("#custom-handle<?=$out_id_letter ?>");
	var hidden = $("#value<?=$out_id_letter ?>");
	$("#slider<?=$out_id_letter ?>").slider({
		min: 0, max: 255, value: <?=$status[$out_id] ?>,
		create: function() { handle.text($(this).slider("value")); hidden.attr("value", $(this).slider("value")); },
		slide: function(event, ui) { handle.text(ui.value); hidden.attr("value", ui.value); }
	});
});
</script>
				<input type="hidden" id="value<?=$out_id_letter ?>" name="value<?=$out_id_letter ?>">
				<tr>
					<td><?=$out_id_letter ?></td>
					<td class="statuscol">
						<div id="slider<?=$out_id_letter ?>"><div id="custom-handle<?=$out_id_letter ?>" class="custom-handle ui-slider-handle"></div></div>
					</td>
					<td><input type="submit" name="dimset" value="Set value <?=$out_id_letter ?>"></td>
				</tr>
<?php		} ?>
			</table>
			</form>
<?php } ?>

		</article>
	</div>
</section>
<?php require '../footer.php'; ?>
