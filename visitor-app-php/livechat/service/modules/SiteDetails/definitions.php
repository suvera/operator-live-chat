<?php

$dictionary['SiteDetails'] = array(
	'table' => 'site_details',
	'fields' => array(

	'id' => array (
		'name' => 'id',
		'type' => 'int',
		'dbtype' => 'int',
		'vname' => 'LBL_ID',
		'len' => '11',
		'not_null' => true,
		'primary_key' => true,
	),
	'company' => array (
		'name' => 'company',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_COMPANY',
		'len' => '255',
		'not_null' => true,
	),
	'website' => array (
		'name' => 'website',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_WEBSITE',
		'len' => '255',
		'not_null' => true,
	),
	'category' => array (
		'name' => 'category',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CATEGORY',
		'len' => '100',
		'not_null' => true,
	),
	'firstname' => array (
		'name' => 'firstname',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_FIRSTNAME',
		'len' => '100',
		'not_null' => true,
	),
	'lastname' => array (
		'name' => 'lastname',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_LASTNAME',
		'len' => '100',
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
	'phone' => array (
		'name' => 'phone',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_PHONE',
		'len' => '32',
		'not_null' => true,
	),
	'country' => array (
		'name' => 'country',
		'default' => 'IN',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_COUNTRY',
		'len' => '60',
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
),

);


?>