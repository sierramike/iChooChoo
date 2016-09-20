<?php
require_once '../load.php';

$g_title = 'Settings';
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

$data = icc_sendreceive("GET_MODULELIST\n");
//echo $data; /////////////////////////////////////////////////// DEBUG
if ($data === "") { redirect ($g_root . '/connect_error.php'); }

$modules = read_moduleident_list($data);

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
<?php } ?>
			<p>This page lists all modules detected by the iChooChoo server. You can change description of modules and define address and type of new
			module. You can also soft-reset or hard-reset a module.</p>
			<p>Use rescan button at the bottom if you think this list is not up-to-date.</p>
			<p><strong>Caution:</strong> don't hard-reset a module if there is already a new module detected (address 0x77). This may cause two modules have
			the same 0x77 address and you won't be able to configure it unless you physically unconnect the module from the bus.</p>
			<table border="0" cellspacing="0">
				<tr>
					<th>Address</th>
					<th>Version</th>
					<th>Type</th>
					<th>Description</th>
					<th>&nbsp;</th>
					<th>&nbsp;</th>
					<th>&nbsp;</th>
					<th>&nbsp;</th>
				</tr>
<?php
if (is_null($modules))
{
	echo 'Error when reading modules list.';
}
else
{
//	echo count($modules) . ' module(s) found.<br>';
	foreach($modules as $address => $module)
	{
?>
		<form method="post" id="frm<?=$address ?>">
		<input type="hidden" name="address" value="<?=$module->Address ?>">
		<tr <?php if ($module->InfoPresent === false) { echo 'class="invalid"'; } ?>>
			<td><?php if ($module->InfoPresent) {
				if ($module->Address === 0x77) { ?>
				<select name="newaddress">
				<?php for ($i = 8; $i < 0x77; $i++) {
					if (is_null($modules[$i])) { ?>
					<option value="<?=$i ?>">0x<?php echo dechex($i); ?></option>
				<?php } } ?>
					<option value="119" selected>0x77</option>
				</select>
				<?php } else { ?>
				0x<?php echo dechex($module->Address) ?>
				<?php }
				} else { ?>0x<?php echo dechex($module->Address); } ?></td>
			<td><?php if ($module->InfoPresent) { ?> <?=$module->Major ?>.<?=$module->Minor ?>.<?=$module->Build ?> <?php } ?></td>
			<td><?php if ($module->InfoPresent) {
				if ($module->Address === 0x77) { ?>
				<select name="newtype">
					<option value="0">-</option>
				<?php foreach($types as $type => $desc) { ?>
					<option value="<?=$type ?>">0x<?php echo dechex($type); ?> - <?=$desc ?></option>
				<?php } ?>
				</select>
				<?php } else { ?>
				0x<?php echo dechex($module->Type) . ' - ' . $types[$module->Type]; ?>
				<?php }
				} else { ?>&nbsp;<?php } ?></td>
			<td><?php if ($module->InfoPresent && $module->Address !== 0x77) { ?>
				<input name="description" value="<?=$module->Description ?>" maxlength="14">
				<?php } else {
					if ($module->Address === 0x77) { ?>(New module)<?php }
					else { ?>(Unknown module)<?php }
				} ?></td>
			<td><?php if ($module->InfoPresent) { ?><input type="submit" name="save" value="Save"><?php } ?></td>
			<td><?php if ($module->InfoPresent) { ?><a href="moduletest.php?addr=<?=$module->Address ?>" class="button buttongreen">Test</a><?php } ?></td>
			<td><?php if ($module->InfoPresent) { ?><input type="submit" name="softreset" value="Soft-Reset"><?php } ?></td>
			<td><?php if ($module->InfoPresent && $module->Address !== 0x77 && is_null($modules[0x77])) { ?><input type="submit" name="hardreset" value="Hard-Reset"><?php } ?></td>
		</tr>
		</form>
<?php
	}
}
?>
			</table>
			<form method="post" id="rescan">
			<p class="center"><input type="submit" name="rescan" value="Rescan the bus"></p>
			</form>
		</article>
	</div>
</section>
<?php require '../footer.php'; ?>
