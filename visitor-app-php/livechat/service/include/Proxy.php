<?php
/*
*
*
*/

class Proxy {

	var $req;

	var $input;

	function Proxy() {
		global $json;

		$this->input = file_get_contents('php://input', 1000000);
		$this->req = $json->decode($this->input);
	}

	function execute() {
		$bean = get_object($this->req->module);
		
		//file_put_contents("test.log", "\n".var_export($this->input, true)."\n", FILE_APPEND);
		if($bean && method_exists($bean, $this->req->action)) {
			return $bean->{$this->req->action}($this->req->args);
		}

		return 'Hello'.$bean->module.$this->req->action;
	}
}


?>