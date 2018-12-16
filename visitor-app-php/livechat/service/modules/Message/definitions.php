<?php

$dictionary['Message'] = array(
	'table' => 'message',
	'fields' => array(

	'id' => array (
		'name' => 'id',
		'type' => 'real',
		'dbtype' => 'real',
		'vname' => 'LBL_ID',
		'len' => '22',
		'not_null' => true,
		'primary_key' => true,
	),
	'contact_id' => array (
		'name' => 'contact_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CONTACT_ID',
		'len' => '36',
		'not_null' => true,
	),
	'sender' => array (
		'name' => 'sender',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_SENDER',
		'len' => '255',
		'not_null' => true,
	),
	'message' => array (
		'name' => 'message',
		'type' => 'blob',
		'dbtype' => 'blob',
		'vname' => 'LBL_MESSAGE',
		'len' => '65535',
		'not_null' => true,
		'blob' => true,
	),
	'receiver' => array (
		'name' => 'receiver',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_RECEIVER',
		'len' => '255',
		'not_null' => true,
	),
	'status' => array (
		'name' => 'status',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_STATUS',
		'len' => '12',
		'not_null' => true,
	),
),

);


?>