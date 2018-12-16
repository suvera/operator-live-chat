<?php
global $dictionary;

$dictionary['Generic'] = array(
	'table' => 'content',
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
	'pid' => array (
		'name' => 'pid',
		'default' => '',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_PID',
		'len' => '255',
		'multiple_key' => true,
	),
	'deleted' => array (
		'name' => 'deleted',
		'default' => '0',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_DELETED',
		'len' => '1',
		'not_null' => true,
	),
	'created_by' => array (
		'name' => 'created_by',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CREATED_BY',
		'len' => '12',
		'not_null' => true,
	),
	'modified_by' => array (
		'name' => 'modified_by',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_MODIFIED_BY',
		'len' => '12',
		'not_null' => true,
	),
	'created_date' => array (
		'name' => 'created_date',
		'type' => 'datetime',
		'dbtype' => 'datetime',
		'vname' => 'LBL_CREATED_DATE',
		'len' => '19',
		'not_null' => true,
		'binary' => true,
	),
	'modified_date' => array (
		'name' => 'modified_date',
		'type' => 'datetime',
		'dbtype' => 'datetime',
		'vname' => 'LBL_MODIFIED_DATE',
		'len' => '19',
		'not_null' => true,
		'binary' => true,
	),
	'title' => array (
		'name' => 'title',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_TITLE',
		'len' => '60',
		'not_null' => true,
		'required' => true,
		'show_href' => true,
	),
	'cat_id' => array (
		'name' => 'cat_id',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_CAT_ID',
		'len' => '255',
	),
	'type' => array (
		'name' => 'type',
		'type' => 'string',
		'dbtype' => 'string',
		'vname' => 'LBL_TYPE',
		'required' => true,
		'len' => '20',
	),
	'content' => array (
		'name' => 'content',
		'type' => 'blob',
		'dbtype' => 'blob',
		'vname' => 'LBL_CONTENT',
		'len' => '-1',
		'not_null' => true,
		'blob' => true,
	),
),

);

global $Lists;
$Lists['Generic'] = array (
	'default' => array (
		'where' => "deleted!='1'",
		'order_by' => 'modified_date desc',
		'cols' => array (
			'image' => array (
				'DISPLAY_ONLY' => true,
				'function' => 'display_listview_image',
				'vname' => 'LBL_IMAGE',
				'HTML' => array('width'=>'10%','align'=>'center','valign'=>'middle',),
			),
			'title' => array (
				'HTML' => array('align'=>'left','valign'=>'middle',),
			),
			'type' => array (
				'HTML' => array('width'=>'10%','align'=>'left','valign'=>'middle',),
			),
			'visits' => array (
				'function' => 'get_num_of_visits',
				'vname' => 'LBL_NUM_VISITS',
				'DISPLAY_ONLY' => true,
				'HTML' => array('width'=>'10%','align'=>'center','valign'=>'middle',),
			),

		), //COLS End
	), //list1 ends
	'approve' => array (
		'where' => "approved!='1' AND deleted!='1'",
		'order_by' => 'modified_date desc',
		'cols' => array (
			'name' => array (
				'HTML' => array('width'=>'15%','align'=>'left','valign'=>'middle',),
			),
			'area' => array (
				'HTML' => array('width'=>'15%','align'=>'center','valign'=>'middle',),
			),
			'address' => array (
				'HTML' => array('width'=>'15%','align'=>'center','valign'=>'middle',),
			),
			'approve' => array (
				'HTML' => array('width'=>'5%','align'=>'center','valign'=>'middle',),
				'DISPLAY_ONLY' => true,
			),
			'edit' => array (
				'HTML' => array('width'=>'5%','align'=>'center','valign'=>'middle',),
				'DISPLAY_ONLY' => true,
			),
			'delete' => array (
				'HTML' => array('width'=>'5%','align'=>'center','valign'=>'middle',),
				'DISPLAY_ONLY' => true,
			),

		), //COLS End
	), //list1 ends
	'search' => array (
		'where' => "deleted!='1'",
		'order_by' => 'modified_date desc',
		'cols' => array (
			'image' => array (
				'DISPLAY_ONLY' => true,
				'function' => 'display_listview_image',
				'vname' => 'LBL_IMAGE',
				'HTML' => array('width'=>'10%','align'=>'center','valign'=>'middle',),
			),
			'title' => array (
				'HTML' => array('align'=>'left','valign'=>'middle',),
			),
			'type' => array (
				'HTML' => array('width'=>'10%','align'=>'left','valign'=>'middle',),
			),
			'visits' => array (
				'function' => 'get_num_of_visits',
				'vname' => 'LBL_NUM_VISITS',
				'DISPLAY_ONLY' => true,
				'HTML' => array('width'=>'10%','align'=>'center','valign'=>'middle',),
			),

		), //COLS End
	), //list1 ends
);

?>