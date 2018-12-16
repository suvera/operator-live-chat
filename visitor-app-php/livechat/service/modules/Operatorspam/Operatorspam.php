<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: Operatorspam.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/Operatorspam/lists.php");

class Operatorspam extends Attic {

	var $id;
	var $operator_id;
	var $site_id;
	var $spamIP;


	var $table = 'operatorspam';


	//
	function Operatorspam () {
		global $atticList;

		$this->module = 'Operatorspam';

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
}

?>