<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: Operator.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/Operators/lists.php");

class Operator extends Attic {

	var $id;
	var $site_id;
	var $name;
	var $login;
	var $nickname;
	var $password;
	var $email;
	var $max_no_chats;
	var $accept_incoming;
	var $cobrowse;
	var $isblocked;


	var $table = 'operators';


	//
	function Operator () {
		global $atticList;

		$this->module = 'Operators';

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

	function login($args) {
		
		$pwd = md5(@$args->argv1);

		$sql = "SELECT * FROM operators WHERE login='".$args->argv0."' AND `password`='".$pwd."' AND site_id='".$args->argv2."'";
		$result = query($sql);

		$ret_arr = array('failed', '', '');

		if($row = fetchByAssoc($result) ) {
			$lastlogin = date("Y-m-d G:i:s");

			$sql = "INSERT INTO operator_online (operator_id, site_id, status, last_login ) VALUES ('".$row['id']."', '".$row['site_id']."', 'online', '$lastlogin')";
			query($sql);

			$ret_arr = array('success', $row['name'], $row['id']);
		}

		return $ret_arr;
	}

	function logout($args) {
		
		$id = $args->argv0;

		$sql = "DELETE FROM operator_online WHERE operator_id='$id' ";
		$result = query($sql);
	}
}

?>