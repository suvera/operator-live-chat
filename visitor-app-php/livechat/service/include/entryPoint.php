<?php
if(!empty($_SERVER['NARAMALA_HOST'])) {
	ini_set('session.cookie_domain', ".remo.com");
} else {
	ini_set('session.cookie_domain', ".naramala.com");
}
session_cache_expire(30);
$cache_expire = session_cache_expire();

session_start();
define("_VALID_ENTRY", 1);

require_once("config.php");

$GLOBALS['db'] = mysql_pconnect($_config['dbconfig']['db_host_name'], $_config['dbconfig']['db_user_name'], $_config['dbconfig']['db_password']);
mysql_select_db($_config['dbconfig']['db_name'], $GLOBALS['db']);

require_once("attics.php");
require_once("include/JSON.php");
require_once("include/utils.php");
require_once("data/Attic.php");


// create a new instance of Services_JSON
$json = new Services_JSON();

session_gc($cache_expire);
?>