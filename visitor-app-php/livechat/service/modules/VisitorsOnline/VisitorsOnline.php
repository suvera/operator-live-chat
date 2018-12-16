<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: VisitorsOnline.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/VisitorsOnline/lists.php");

class VisitorsOnline extends Attic {

	var $id;
	var $httpsession;
	var $ip;
	var $contact_id;
	var $site_id;
	var $operator_id;
	var $status;


	var $table = 'visitors_online';


	//
	function VisitorsOnline () {
		global $atticList;

		$this->module = 'VisitorsOnline';

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

	function get_online_visitors($args) {
		$siteid = $args->argv0;

		$query = "SELECT 
					vo.id,
					vo.contact_id,
					vo.site_id,
					vo.status,
					vo.ip,
					IFNULL(v.city, 'Un Known'),
					IFNULL(v.country, 'Un Known')
				FROM 
					visitors_online vo
					LEFT JOIN visitors v ON vo.id = v.id
				WHERE 
					vo.site_id='$siteid' 
				";

		$result = query($query);
		$rows = array();

		while($row = fetchByAssoc($result)) {
			$rows[] = array_values($row);
		}

		return $rows;
	}

	function offline($args) {
		$contact_id = $args->contact_id;

		$sql = "UPDATE visitors_online SET `status`='offline' WHERE contact_id='$contact_id' ";
		//$sql = "DELETE FROM visitors_online WHERE contact_id='$contact_id' ";
		$result = query($sql);

		return array("success");
	}
}

?>