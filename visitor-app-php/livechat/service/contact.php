<?php
require_once("include/entryPoint2.php");

if(!empty($_REQUEST['yourname'])) {
	$to      = $_config['to_email'];
	$subject = "Games Query from " . $_REQUEST['yourname'];
	$message = nl2br($_REQUEST['comments']);
	$message .= "<br><br>Email: " . $_REQUEST['email'] . "<br>";
	
	// To send HTML mail, the Content-type header must be set
	$headers  = 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$headers .= 'From: Live Help <' . $_config['to_email'] . '>' . "\r\n";
	
	mail($to, $subject, $message, $headers);
}
?>
<br>
<br>
<br>
<center>
Thnak you!
</center>