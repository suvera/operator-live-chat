<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: Message.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/Message/lists.php");

class Message extends Attic {

	var $id;
	var $contact_id;
	var $sender;
	var $message;
	var $receiver;
	var $status;


	var $table = 'message';


	//
	function Message () {
		global $atticList;

		$this->module = 'Message';

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

	function add($args) {
		//file_put_contents("test.log", var_export($args, true)."\n", FILE_APPEND);
		
		$status = 'online';
		if($args->status) $status = $args->status;

		$this->id = microtime(true);
		$this->contact_id = $args->contact_id;
		$this->sender = $args->sender;
		$this->message = $args->message;
		$this->receiver = $args->receiver;
		$this->status = $status;
		$this->is_new = true;
		$this->save();

		return array("success");
	}

	function get_new_message($args) {
		$contact_id = $args->contact_id;
		$sender = $args->sender;
		$message = $args->message;
		$receiver = $args->receiver;
		$last_msg_id = (!empty($args->last_msg_id)?$args->last_msg_id:0);
		$firstTime = $args->firstTime;
		
		if($firstTime == 'yes') {
			$result = query("SELECT message,id,status,sender,receiver FROM message WHERE contact_id='$contact_id' ");
		} else {
			$result = query("SELECT message,id,status,sender,receiver FROM message WHERE receiver='$receiver' AND contact_id='$contact_id' AND id>$last_msg_id");
		}
		$rows = array();

		while($row = fetchByAssoc($result)) {
			$row['message'] = wordwrap(urldecode($row['message']), 60, "<br>");
			$rows[] = array_values($row);
		}

		return $rows;
	}
}

?>