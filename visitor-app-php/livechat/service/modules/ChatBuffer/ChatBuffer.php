<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Apr 05, 2008 By nramanarayana
 * File Id: ChatBuffer.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/ChatBuffer/lists.php");

class ChatBuffer extends Attic {

	var $id;
	var $contact_id;
	var $sender;
	var $message;
	var $receiver;
	var $status;
	var $is_read;


	var $table = 'chatbuffer';


	//
	function ChatBuffer () {
		global $atticList;

		$this->module = 'ChatBuffer';

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

	//
	function add($args) {
		//file_put_contents("test.log", var_export($args, true)."\n", FILE_APPEND);
		
		$status = 'online';
		if($args->status) $status = $args->status;

		$this->contact_id = $args->contact_id;
		$this->sender = $args->sender;
		$this->message = $args->message;
		$this->receiver = $args->receiver;
		$this->status = $status;
		$this->is_new = true;
		$this->is_read = '0';
		$this->save();

		return array("success");
	}

	//
	function get_read($args) {
		$contact_id = $args->contact_id;
		$sender = $args->sender;
		$message = $args->message;
		$receiver = $args->receiver;
		
		$result = query("SELECT message,id,status,sender,receiver FROM chatbuffer WHERE receiver='$receiver' AND contact_id='$contact_id' AND is_read='0' ");

		$rows = array();

		while($row = fetchByAssoc($result)) {
			$row['message'] = wordwrap(urldecode($row['message']), 60, "<br>");
			$rows[] = array_values($row);
			query("UPDATE chatbuffer SET is_read='1' WHERE id='" . $row['id'] . "' ");
		}

		return $rows;
	}
}

?>