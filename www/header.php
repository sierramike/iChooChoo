<?php
require_once 'appconfig.php';
require_once 'functions.php';
?>
<!doctype html>
<html lang="en-US">
<head>
	<title><?php echo $g_title . ' - ' . $g_appname; ?></title>
	<link rel="stylesheet" id="appstyle" href="<?php echo $g_root; ?>/style.css">
	<link rel="stylesheet" id="gfonts" href="https://fonts.googleapis.com/css?family=Sanchez:400,400italic|Maven+Pro:400,700" type='text/css'>
</head>
<body>
<header class="main-header">
	<div class="wrapper">
		<div class="logo-wrap">
			<a href="<?php echo $g_root; ?>/" title="<?php echo $g_appname; ?>" class="logotext"><?php echo $g_appname; ?></a>
		</div>
		<nav class="main-menu" role="navigation" itemscope="itemscope" itemtype="http://schema.org/SiteNavigationElement">
			<a id="toggle-menu-icon"><span class="icon-list"></span></a>
			<ul id="menu-principal" class="top-level-menu">
				<li id="menu-item-1" class="menu-item menu-item-object-page current-menu-item page_item"><a href="<?php echo $g_root; ?>/">Home</a></li>
				<li id="menu-item-1" class="menu-item menu-item-object-page current-menu-item page_item"><a href="<?php echo $g_root; ?>/settings">Settings</a></li>
			</ul>
		</nav>
	</div>
</header>
<section class="description headerbar">
	<div class="wrapper">
		<h1><?php echo $g_title; ?></h1>
	</div>
</section>
