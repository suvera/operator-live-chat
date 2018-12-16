<?php

$dictionary['Operators'] = array(
	'table' => 'operators',
	'fields' => array(

	'id' => array (
		'name' => 'id',
		'type' => 'int',
		'dbtype' => 'int',
		'vname' => 'LBL_ID',
		'len' => '11',
		'not_null' => true,
		'primary_key' => true,
		'auto_increment' => true,
	),
	'site_id' => array (
		'name' => 'site_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_SITE_ID',
		'len' => '12',
		'not_null' => true,
	),
	'name' => array (
		'name' => 'name',
		'required' => true,
		'show_href' => true,
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_NAME',
		'len' => '100',
		'not_null' => true,
	),
	'login' => array (
		'name' => 'login',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_LOGIN',
		'len' => '32',
		'not_null' => true,
	),
	'nickname' => array (
		'name' => 'nickname',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_NICKNAME',
		'len' => '50',
		'not_null' => true,
	),
	'password' => array (
		'name' => 'password',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_PASSWORD',
		'len' => '50',
		'not_null' => true,
	),
	'email' => array (
		'name' => 'email',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_EMAIL',
		'len' => '100',
		'not_null' => true,
	),
	'max_no_chats' => array (
		'name' => 'max_no_chats',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_MAX_NO_CHATS',
		'len' => '20',
		'not_null' => true,
	),
	'accept_incoming' => array (
		'name' => 'accept_incoming',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_ACCEPT_INCOMING',
		'len' => '20',
		'not_null' => true,
	),
	'cobrowse' => array (
		'name' => 'cobrowse',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_COBROWSE',
		'len' => '20',
		'not_null' => true,
	),
	'isblocked' => array (
		'name' => 'isblocked',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_ISBLOCKED',
		'len' => '12',
		'not_null' => true,
	),
),

);


?>