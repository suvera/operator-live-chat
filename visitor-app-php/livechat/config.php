<?php
if(!empty($_SERVER['NARAMALA_HOST'])) {
	ini_set('session.cookie_domain', ".remo.com");
} else {
	ini_set('session.cookie_domain', ".naramala.com");
}
session_start();
error_reporting(0);
if(!empty($_SERVER['TEST_MODE'])) {
	error_reporting(E_ALL ^ E_NOTICE ^ E_WARNING);
}

$_config = array(
	'dbconfig' => array(
		"db_host_name" => "localhost",
		"db_user_name" => "root",
		"db_password" => "",
		"db_name" => "articles",
		'db_type' => 'mysql',
	),
	'dbconfigoption' => array(
		'persistent' => true,
	),
	'site_url' => 'http://livechat.naramala.com/',
	'site_login' => 'http://login.naramala.com/',
	'site' => 'http://www.naramala.com/',
 );
 if(!empty($_SERVER['NARAMALA_HOST'])) {
	$_config['site_url'] = $_SERVER['NARAMALA_LIVECHAT_SITE'];
	$_config['site'] = $_SERVER['NARAMALA_SITE'];
	$_config['site_login'] = $_SERVER['NARAMALA_USERS_SITE'];
}
?>