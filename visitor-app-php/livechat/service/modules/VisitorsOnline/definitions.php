<?php

$dictionary['VisitorsOnline'] = array(
	'table' => 'visitors_online',
	'fields' => array(

	'id' => array (
		'name' => 'id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_ID',
		'len' => '255',
		'not_null' => true,
		'primary_key' => true,
	),
	'httpsession' => array (
		'name' => 'httpsession',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_HTTPSESSION',
		'len' => '128',
		'not_null' => true,
	),
	'ip' => array (
		'name' => 'ip',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_IP',
		'len' => '36',
		'not_null' => true,
	),
	'contact_id' => array (
		'name' => 'contact_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CONTACT_ID',
		'len' => '36',
		'not_null' => true,
	),
	'site_id' => array (
		'name' => 'site_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_SITE_ID',
		'len' => '12',
		'not_null' => true,
	),
	'operator_id' => array (
		'name' => 'operator_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_OPERATOR_ID',
		'len' => '12',
		'not_null' => true,
	),
	'status' => array (
		'name' => 'status',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_STATUS',
		'len' => '20',
		'not_null' => true,
	),
),

);


?>