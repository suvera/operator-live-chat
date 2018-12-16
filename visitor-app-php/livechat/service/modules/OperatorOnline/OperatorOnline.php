<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: OperatorOnline.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/OperatorOnline/lists.php");

class OperatorOnline extends Attic {

	var $operator_id;
	var $site_id;
	var $status;
	var $last_login;


	var $table = 'operator_online';


	//
	function OperatorOnline () {
		global $atticList;

		$this->module = 'OperatorOnline';

		parent::Attic();

	}

	//
	function save () {
		global $atticList, $current_user;

		parent::save();
	}

	//
	function save_relationship() {

		parent::save_relationship();
	}

	//
	function get_list_where($where) {

		return $where;
	}

	//
	function get_where() {
		$where = '';

		return $where;
	}

	function get_operator_online($siteid) {
		$result = query("SELECT operator_id FROM {$this->table} WHERE site_id='$siteid' AND status='online' ORDER BY RAND() LIMIT 0,1");
		$row = fetchByAssoc($result);

		return $row['operator_id'];
	}

	function is_operator_online($operator_id) {
		$result = query("SELECT operator_id FROM {$this->table} WHERE operator_id='$operator_id' ");
		$row = fetchByAssoc($result);

		return ($row['operator_id']?true:false);
	}

	function change_status($args) {
		$id = $args->operator_id;
		$new_status = $args->new_status;
		
		if($id && $new_status) {
			$new_status = strtolower($new_status);

			query("UPDATE {$this->table} SET status='$new_status' WHERE operator_id='$id'");
			return array("success");
		} else {
			return array("failed");
		}
	}
}

?>