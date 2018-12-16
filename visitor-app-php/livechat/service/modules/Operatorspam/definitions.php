<?php

$dictionary['Operatorspam'] = array(
	'table' => 'operatorspam',
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
	'operator_id' => array (
		'name' => 'operator_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_OPERATOR_ID',
		'len' => '12',
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
	'spamIP' => array (
		'name' => 'spamIP',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_SPAMIP',
		'len' => '20',
		'not_null' => true,
	),
),

);


?>