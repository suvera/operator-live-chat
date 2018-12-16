<?php
if (! defined("_VALID_ENTRY") ) die('Not a valid entry');
/*
 * Created on Mar 23, 2008 By nramanarayana
 * File Id: SiteDetail.php
 * 
 * Description: TODO
 * 
 */

require_once("modules/SiteDetails/lists.php");

class SiteDetail extends Attic {

	var $id;
	var $company;
	var $website;
	var $category;
	var $firstname;
	var $lastname;
	var $email;
	var $phone;
	var $country;
	var $password;


	var $table = 'site_details';


	//
	function SiteDetail () {
		global $atticList;

		$this->module = 'SiteDetails';

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
	function cross_check($siteid) {
		$sql = "SELECT * FROM `site_details` WHERE id='$siteid' ";
		$result = query($sql);

		if($row = fetchByAssoc($result)) {
			return true;
		}

		return false;
	}
}

?>