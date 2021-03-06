<?php

$dictionary['Visitors'] = array(
	'table' => 'visitors',
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
	'name' => array (
		'name' => 'name',
		'required' => true,
		'show_href' => true,
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_NAME',
		'len' => '32',
		'not_null' => true,
	),
	'country' => array (
		'name' => 'country',
		'default' => 'IN',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_COUNTRY',
		'len' => '50',
		'not_null' => true,
	),
	'city' => array (
		'name' => 'city',
		'default' => 'naidupet',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CITY',
		'len' => '50',
		'not_null' => true,
	),
	'organization' => array (
		'name' => 'organization',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_ORGANIZATION',
		'len' => '150',
		'not_null' => true,
	),
	'ISP' => array (
		'name' => 'ISP',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_ISP',
		'len' => '100',
		'not_null' => true,
	),
	'connectiontype' => array (
		'name' => 'connectiontype',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CONNECTIONTYPE',
		'len' => '60',
		'not_null' => true,
	),
	'ip' => array (
		'name' => 'ip',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_IP',
		'len' => '50',
		'not_null' => true,
	),
	'host' => array (
		'name' => 'host',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_HOST',
		'len' => '100',
		'not_null' => true,
	),
	'state' => array (
		'name' => 'state',
		'default' => '',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_STATE',
		'len' => '100',
		'not_null' => true,
	),
	'email' => array (
		'name' => 'email',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_EMAIL',
		'len' => '120',
		'not_null' => true,
	),
	'currentpage' => array (
		'name' => 'currentpage',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CURRENTPAGE',
		'len' => '255',
	),
	'referrer' => array (
		'name' => 'referrer',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_REFERRER',
		'len' => '255',
	),
	'no_of_pages' => array (
		'name' => 'no_of_pages',
		'type' => 'int',
		'dbtype' => 'int',
		'vname' => 'LBL_NO_OF_PAGES',
		'len' => '11',
	),
	'cobrowse' => array (
		'name' => 'cobrowse',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_COBROWSE',
		'len' => '12',
	),
	'lastvisit' => array (
		'name' => 'lastvisit',
		'type' => 'datetime',
		'dbtype' => 'datetime',
		'vname' => 'LBL_LASTVISIT',
		'len' => '19',
		'binary' => true,
	),
	'lastchat' => array (
		'name' => 'lastchat',
		'type' => 'datetime',
		'dbtype' => 'datetime',
		'vname' => 'LBL_LASTCHAT',
		'len' => '19',
		'binary' => true,
	),
),

);


?>