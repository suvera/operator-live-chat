<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: Visitor.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/Visitors/lists.php");

class Visitor extends Attic {

	var $id;
	var $name;
	var $country;
	var $city;
	var $organization;
	var $ISP;
	var $connectiontype;
	var $ip;
	var $host;
	var $state;
	var $email;
	var $currentpage;
	var $referrer;
	var $no_of_pages;
	var $cobrowse;
	var $lastvisit;
	var $lastchat;


	var $table = 'visitors';


	//
	function Visitor () {
		global $atticList;

		$this->module = 'Visitors';

		parent::Attic();

	}

	//
	function save () {
		global $atticList, $current_user;
		
		if(empty($this->country)) {
			$info = file_get_contents("http://api.hostip.info/get_html.php?ip=".$this->ip);
			$lines = explode("\n", $info);

			foreach($lines as $row) {
				list($key, $value) = explode(":", $row);
				$key = strtolower(trim($key));
				$this->$key = trim($value);
			}
		} else {
			$this->referrer = $this->currentpage;
		}

		$this->host = gethostbyaddr($this->ip);
		$this->name = $this->host;
		$this->currentpage = $_SERVER['HTTP_REFERER'];
		$this->no_of_pages++;
		$this->lastvisit = date("Y-m-d G:i:s");

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

	function get_info($args) {
		//file_put_contents("test.log", var_export($args, true)."\n", FILE_APPEND);
		$contact_id = $args->contact_id;

		$result = query("SELECT visitors.* FROM visitors JOIN visitors_online ON visitors.id=visitors_online.id WHERE visitors_online.contact_id='$contact_id'");
		$rows = array();

		if($row = fetchByAssoc($result)) {

			 $str = '';
			 foreach($row as $key => $val) {

				$str .= '<tr>';
				$str .= '<td width="10%" style="border:1px solid #999999;">';
				$str .= '<b>' . ucwords(str_replace("_", " ", $key)) . '</b>';
				$str .= '</td>';
				$str .= '<td width="90%" style="border:1px solid #999999;">';
				$str .= $val;
				$str .= '</td>';
				$str .= '</tr>';
			 }

			 $rows[] = $str;
		}

		return $rows;
	}
}

?>