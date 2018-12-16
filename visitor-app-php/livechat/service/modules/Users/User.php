<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: User.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/Users/lists.php");

class User extends Attic {

	var $id;
	var $username;
	var $password;
	var $user_level;
	var $status;


	var $table = 'users';


	//
	function User () {
		global $atticList;

		$this->module = 'Users';

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