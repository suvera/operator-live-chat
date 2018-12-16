<?php
//require_once("include/entryPoint.php");
//require_once("functions.php");
require_once('XTemplate/xtpl.php');

$xtpl = new XTemplate("themes/default/template.html");

$xtpl->assign("MAIN_SITE", $_config['site']);
$xtpl->assign("SITE", $_config['site_url']);
$xtpl->assign("SITE_LOGIN", $_config['site_login']);
$xtpl->assign("LOGIN", login_form($xtpl) );
$xtpl->assign("THEME", "default");

$titlekey = "LIVECHAT.NARAMALA.com: ";
$defaultkey = " free Live Chat software";

if(empty($title)) {
	$page_title = $titlekey . $defaultkey;
} else {
	$page_title = $titlekey . $title . $defaultkey;
}



$xtpl->assign("TITLE", $page_title);
$xtpl->assign("KEYWORDS", "$page_title Narmala Ramanarayana Naramala free Live Chat software Advanced articles, Download pictures, Naidupet directory, Play games, Discussions Forums SEOTOOLS Tech Mastro TechMastro");
$xtpl->assign("DESCRIPTION", "$page_title read the advanced articles, download pictures, search the Naidupet directory, Play games, check the discussions or forums and use SEOTOOLS to check your website statistics. Login to NARAMALA.COM seotools seo-tools seo tools web tools Narmala Ramanarayana Naramala Tech Mastro TechMastro");

$xtpl->parse("header");
$xtpl->out("header");

function login_form(&$xtpl) {
	global $_config, $current_user;
	
	if(!empty($_SESSION['_SESS_USERID'])) {
		$xtpl->assign("USER_NAME", $current_user->user_name);
		$xtpl->parse("logout");
		return $xtpl->text("logout");
	} else {
		$xtpl->parse("login_left");
		return $xtpl->text("login_left");
	}
}

?>