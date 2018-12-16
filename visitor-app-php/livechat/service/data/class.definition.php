<?php
//
require_once("include/utils.php");
require_once("data/generic.php");

class Definition {

	var $module;
	var $definitions = array();

	function Definition() {

		if (file_exists("modules/" . $this->module . "/definitions.php")) {
			require("modules/" . $this->module . "/definitions.php");
			$this->definitions = $dictionary[$this->module];
		} 
	}

	//
	function getDefs( $type, $values ) {

		$ret = array();

		foreach ($this->definitions['fields'] as $field => $defs) {

			if (is_array( $values )) {

				if ( inArray( @$defs[$type], $values) )
					$ret[] = $field;

			} else if (strtolower(@$defs[$type]) == strtolower($values)) {
				$ret[] = $field;
			}
		}
		return $ret;
	}

	//


}

?>