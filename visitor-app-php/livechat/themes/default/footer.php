<?php
//require_once("include/entryPoint.php");
//require_once("functions.php");


$xtpl = new XTemplate("themes/default/template.html");

$xtpl->assign("MAIN_SITE", $_config['site']);
$xtpl->assign("SITE", $_config['site_url']);

$xtpl->parse("footer");
$xtpl->out("footer");
?>