<?php

$dictionary['OperatorOnline'] = array(
	'table' => 'operator_online',
	'fields' => array(

	'operator_id' => array (
		'name' => 'operator_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_OPERATOR_ID',
		'len' => '12',
		'not_null' => true,
		'primary_key' => true,
	),
	'site_id' => array (
		'name' => 'site_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_SITE_ID',
		'len' => '12',
		'not_null' => true,
		'primary_key' => true,
	),
	'status' => array (
		'name' => 'status',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_STATUS',
		'len' => '20',
		'not_null' => true,
	),
	'last_login' => array (
		'name' => 'last_login',
		'type' => 'datetime',
		'dbtype' => 'datetime',
		'vname' => 'LBL_LAST_LOGIN',
		'len' => '19',
		'binary' => true,
	),
),

);


?>