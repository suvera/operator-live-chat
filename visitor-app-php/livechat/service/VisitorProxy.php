<?php
require_once("include/entryPoint.php");
require_once("include/Proxy.php");

$prxy = new Proxy();

//print_r($prxy->input);

$output = $prxy->execute();


$json_arr = array(
	'id' => $prxy->req->id,
	'result' => $output,
);

print $json->encode($json_arr);
?>