<?php

$dictionary['Users'] = array(
	'table' => 'users',
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
	'username' => array (
		'name' => 'username',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_USERNAME',
		'len' => '32',
		'not_null' => true,
	),
	'password' => array (
		'name' => 'password',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_PASSWORD',
		'len' => '32',
		'not_null' => true,
	),
	'user_level' => array (
		'name' => 'user_level',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_USER_LEVEL',
		'len' => '10',
		'not_null' => true,
	),
	'status' => array (
		'name' => 'status',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_STATUS',
		'len' => '10',
		'not_null' => true,
	),
),

);


?>