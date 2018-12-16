<?php
//
require_once("data/class.definition.php");


class Attic extends Definition {

	var $db;

	var $action;

	var $module = '';

	var $summary;

	var $column_fields;

	var $required_fields;

	var $lists;

	var $is_new = false;

	var $relations = array();


	function Attic() {
		global $atticList, $Lists;

		parent::Definition();

		//$this->db = & DBManager::getInstance();

		$this->column_fields = $this->getDefs('source', array(null, '', 'db'));
		$this->required_fields = $this->getDefs('required', true);

		$this->lists = $Lists[$this->module];
		$this->relations = $atticList[$this->module]['relationships'];
		$this->summary = $atticList[$this->module]['summary'];
	}

	//
	function retrieve($id) {

		if (empty($id)) $id = -1;

		$query = "SELECT " . implode(",", $this->column_fields) . " FROM " . $this->table . " WHERE id='" . $id . "' ";
		$result = query($query);

		$this->populate($result);
	}

	//
	function populate($result) {

		$data = fetchByAssoc($result, -1, false);

		if (! $data) return;

		foreach ($data as $field => $value) {

			$this->$field = $value;
		}
	}
	
	//
	function populate_by_array($data) {

		if (! $data) return;

		foreach ($data as $field => $value) {

			$this->$field = addslashes(stripslashes($value));
		}
	}

	//
	function save() {
		global $atticList, $current_user;
		
		if(empty($this->table)) return false;

		$query = "INSERT INTO " . $this->table . " ";
		$isUpdate = false;

		if (!empty($this->id) && !$this->is_new) {
			
			$query = "UPDATE " . $this->table . " ";
			$isUpdate = true;
		} else {
			$this->created_date = date("Y-m-d G:i:s");
			$this->created_by = $current_user->id;
		}

		$this->modified_date = date("Y-m-d G:i:s");
		$this->modified_by = $current_user->id;

		$query .= " SET ";

		foreach ($this->column_fields as $field ) {

			if($field == 'id' && empty($this->id) ) continue;

			$isNotNull = @$this->definitions['fields'][$field]['not_null'];

			if (empty($this->$field) ) {
				if(!$isUpdate) {
					$default = @$this->definitions['fields'][$field]['default'];
					if (!$isNotNull) {
						if (isset($this->definitions['fields'][$field]['default']))
							$query .= $field . " ='$default',";
						else
							$query .= $field . " =null,";
					} else {
						$query .= $field . " = '" . $default . "',";
					}
				} else {
				}
			}
			else
				$query .= $field . " = '" . $this->$field . "',";
		}
		$query = substr($query, 0, -1);
		//echo $query."<br><br>";
		 
		if ($isUpdate) {
			$query .= " WHERE id = '" . $this->id . "' ";
			query($query);
		} else {
			query($query);
			//$this->id = mysql_insert_id($GLOBALS['db']);
		}
		
		$this->save_relationship();
		//$this->sendNotification($isUpdate);
		
		return true;
	}

	//
	function delete() {
		
		if (!empty($this->id)) {
			$query = "UPDATE " . $this->table . " SET deleted='1' WHERE id = '{$this->id}'";
			query($query);
			return true;
		}
		return false;
	}

	//
	function save_relationship() {
		global $atticList;

	} //function Ends

	//
	function update_field($field, $value, $where) {
		
		if(!$field || !$value || !$where) return false;

		$query = "UPDATE {$this->table} SET $field='$value' WHERE $where";
		$result = query($query);

		return $result;
	}

}

?>