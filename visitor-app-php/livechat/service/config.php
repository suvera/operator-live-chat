<?php
error_reporting(0);
//error_reporting(E_ALL ^ E_NOTICE ^ E_WARNING);

$GLOBALS['starttTime'] = microtime();

$_config = array(
	'dbconfig' => array(
		/*"db_host_name" => "localhost",
		"db_user_name" => "naramala",
		"db_password" => "naramala",
		"db_name" => "livechat",
		*/
		"db_host_name" => "p50mysql127.secureserver.net",
		"db_user_name" => "rama_livechat",
		"db_password" => "Xcjynuc7zv",
		"db_name" => "rama_livechat",
		

		'db_type' => 'mysql',
	),
	'dbconfigoption' => array(
		'persistent' => true,
	),
	'site_url' => 'http://livechat.naramala.com/',
	
	'visitor_proxy' => "http://livechat.naramala.com/service/VisitorProxy.php?",

	'operator_proxy' => "http://livechat.naramala.com/service/OperatorProxy.php?",

	'ping_time' => 3000,
	
	'to_email' => 'webmaster@naramala.com',
);
?>