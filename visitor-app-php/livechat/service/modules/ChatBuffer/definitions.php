<?php

$dictionary['ChatBuffer'] = array(
	'table' => 'chatbuffer',
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
		'multiple_key' => true,
	),
	'sender' => array (
		'name' => 'sender',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_SENDER',
		'len' => '255',
		'not_null' => true,
		'multiple_key' => true,
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
		'multiple_key' => true,
	),
	'status' => array (
		'name' => 'status',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_STATUS',
		'len' => '12',
		'not_null' => true,
	),
	'is_read' => array (
		'name' => 'is_read',
		'type' => 'int',
		'dbtype' => 'int',
		'vname' => 'LBL_IS_READ',
		'default' => '0',
		'len' => '1',
		'not_null' => true,
		'multiple_key' => true,
	),
),

);


?>