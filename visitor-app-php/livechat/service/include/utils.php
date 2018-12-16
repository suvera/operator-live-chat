<?php

function query($query) {
	$result = mysql_query($query, $GLOBALS['db']);
	if(!$result && $GLOBALS['_de']) 
		print  $query . "<br>--" . mysql_error()."<br>";
	
	return $result;
}

function fetchByAssoc(&$result) {
	return mysql_fetch_assoc($result);
}

function getRowCount(&$result) {
	return mysql_num_rows($result);
}


function session_gc($time) {
	$time = $time * 60;
	$now = date("Y-m-d G:i:s");
	//print "DELETE visitors_online FROM visitors, visitors_online WHERE visitors.id=visitors_online.id AND TIME_TO_SEC(TIMEDIFF('$now', lastvisit)) >= '$time'";
	query("DELETE visitors_online FROM visitors, visitors_online WHERE visitors.id=visitors_online.id AND TIME_TO_SEC(TIMEDIFF('$now', lastvisit)) >= '$time'");
}

function get_object($module) {
	global $atticList;
	$name = $atticList[$module]['bean'];

	$viewfile = "modules/$module/$name.php";
	
	if(file_exists($viewfile)) {
		require_once($viewfile);
		return new $name();
	} else {
		return null;
	}
}

function create_guid()
{
	$microTime = microtime();
	list($a_dec, $a_sec) = explode(" ", $microTime);

	$dec_hex = sprintf("%x", $a_dec* 1000000);
	$sec_hex = sprintf("%x", $a_sec);

	ensure_length($dec_hex, 5);
	ensure_length($sec_hex, 6);

	$guid = "";
	$guid .= $dec_hex;
	$guid .= create_guid_section(3);
	$guid .= '-';
	$guid .= create_guid_section(4);
	$guid .= '-';
	$guid .= create_guid_section(4);
	$guid .= '-';
	$guid .= create_guid_section(4);
	$guid .= '-';
	$guid .= $sec_hex;
	$guid .= create_guid_section(6);

	return $guid;

}

function create_guid_section($characters)
{
	$return = "";
	for($i=0; $i<$characters; $i++)
	{
		$return .= sprintf("%x", mt_rand(0,15));
	}
	return $return;
}

function ensure_length(&$string, $length)
{
	$strlen = strlen($string);
	if($strlen < $length)
	{
		$string = str_pad($string,$length,"0");
	}
	else if($strlen > $length)
	{
		$string = substr($string, 0, $length);
	}
}

function inArray($item, $array_) {

	if ( ! is_array($array_) ) return false;

	$flag = false;
	foreach ($array_ as $k => $v) {
		if (strtolower($item) == strtolower($v)) {
			$flag = true;
			break;
		}
	}

	return $flag;
}



?>