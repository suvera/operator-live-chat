<?php
require_once("include/entryPoint2.php");
require_once("modules/SiteDetails/SiteDetail.php");
require_once("modules/OperatorOnline/OperatorOnline.php");

$siteid = $_REQUEST['siteid'];
$operator_id = $_REQUEST['operator_id'];
$contact_id = $_REQUEST['contact_id'];

$siteObj = new SiteDetail();

if(empty($siteid) || !$siteObj->cross_check($siteid) ) {
	die("Unable to process your request. Please contact sit admin.");
}

?>
<html>
<head>
<link href="textstyles.css" rel="stylesheet" type="text/css">
<title>Live Chat Window</title>
<script src="js/yahoo-min.js"></script>
<script src="js/event-min.js"></script>
<script src="js/connection-min.js"></script>
<script src="js/JSON.js"></script>
</head>
<body onLoad="startTimer()" onbeforeunload="windowUnloaded()" bgcolor='#0000FF' leftmargin="0" topmargin="0">
<?php
$op_online = new OperatorOnline();

if(empty($operator_id) && !$op_online->is_operator_online($operator_id) ) {
?>
<script>
function windowUnloaded()
{
}
function startTimer() {
}
</script>
<p>Online Help is unavailable now, please drop us a mail.<br>

<form name="contactform" action="contact.php" method="post">
<p>Your Name: <input name="yourname" type="text"></p>
<p>Your Email Id:<input name="email" type="text" size="30"></p>
<p>Comments: <br><textarea name="comments" cols="30" rows="3"></textarea></p>
<p><input name="Submit" type="submit" value="Submit"></p>
</form>
</p>
<?php
}
else {
?>
<script>
var proxy = "<?php echo $_config['visitor_proxy']; ?>";
var pingTime = <?php echo $_config['ping_time']; ?>;
var syncLock = false;
var contactId = "<?php echo $contact_id; ?>";
var siteId = "<?php echo $siteid; ?>";
var operatorId = "<?php echo $operator_id; ?>";
var lastMsgId = "0";
var Timer;
var firstTime = 'yes';

function windowUnloaded()
{
	var callback = {};
    var postData = '{"args":{"contact_id":"'+contactId+'"}';
	postData += ',"module":"VisitorsOnline","action":"offline"}';
	
    var request = YAHOO.util.Connect.asyncRequest('POST', proxy, callback, postData);
	alert('closed');
}
function LoadMessage()
{
	if(document.sendtext.message.value == "") {
		alert('enter message');
	} else {
		var handleSuccess = function(o){
			if(o.responseText !== undefined){
				//o.responseText
			}
		}
		
		var callback = { success:handleSuccess, failure:handleSuccess};
		var postData = '{"args":{"contact_id":"'+contactId+'","sender":"'+contactId+'","site_id":"'+siteId+'","receiver":"'+operatorId+'","last_msg_id":"'+lastMsgId+'",';
		postData += '"message":"'+escape(document.sendtext.message.value)+'"},"module":"Message","action":"add"}';
		
		var request = YAHOO.util.Connect.asyncRequest('POST', proxy, callback, postData);
		
		insertText("me", document.sendtext.message.value);
		document.sendtext.message.value='';
	 } 
	 return false;
}
function getNewMessages()
{
	if(syncLock) {
		//Do Nothing...
	} else {
		var handleSuccess = function(o) {
			syncLock = false;
			firstTime = 'no';
			//alert(o.responseText);
			if(o.responseText !== undefined){
				var result = {};
				try {
					result = eval(JSON.decode(o.responseText));
				} catch(e) {
					//alert("Eval: "+e);
				}
				//alert(result.result.length);
				
				try{
					for(var i=0; i<result.result.length; i++) {
						arr = result['result'][i];
						from = "Operator";
						
						try {
							if(arr[3] == contactId)
								from = "me";
						} catch(e) { }

						try {
							before_process(arr[2], arr[0]);
						} catch(e) {
							alert("before_process call:"+e);
						}

						insertText(from, arr[0]);
						lastMsgId = arr[1];
						
						if(arr[2] == 'finish' || arr[2] == 'closed') {
							closeTimer();
						}
						
					}
				} catch(e) {
					alert("For Each:"+e);
				}
			}
		}
		
		var callback = { success:handleSuccess, failure:handleSuccess};
		var postData = '{"args":{"contact_id":"'+contactId+'","site_id":"'+siteId+'","receiver":"'+contactId+'","last_msg_id":"'+lastMsgId+'","firstTime":"'+firstTime+'"},';
		postData += '"module":"Message","action":"get_new_message"}';
		
		syncLock = true;
		var request = YAHOO.util.Connect.asyncRequest('POST', proxy, callback, postData);
	 }
}
function before_process(type, msg) {
	if(type == 'winopen') {
		try {
			if(firstTime != 'yes')
				window.open(msg, '_blank'); 
		} catch(e) { 
			alert("Proactive:"+e); 
		}
	}
}
function insertText(user, text) {
	
	var TextFrame = document.getElementById("visitorFrame");
	var tbody = document.getElementById("chatMessageTbody");
	
	var tr = document.createElement("tr");
	var td1 = document.createElement("td");
	var td2 = document.createElement("td");
	if(user == 'me') {
		try {
			td1.className = "visitorBold";
			td2.className = "visitorText";
		} catch(e) { }
		td1.setAttribute("class", "visitorBold");
		td2.setAttribute("class", "visitorText");
	} else {
		try {
			td1.className = "operatorBold";
			td2.className = "operatorText";
		} catch(e) { }
		td1.setAttribute("class", "operatorBold");
		td2.setAttribute("class", "operatorText");
	}

	var txt1 = document.createTextNode(user+":");
	var txt2 = document.createTextNode(unescape(text));
	td1.appendChild(txt1);
	td2.appendChild(txt2);
	tr.appendChild(td1);
	tr.appendChild(td2);
	tbody.appendChild(tr);
	
	TextFrame.scrollTop = TextFrame.scrollHeight;

	//setTimeout("insertText()", 3000);
}

function startTimer() {
	Timer = setInterval("getNewMessages()", pingTime);
}

function closeTimer() {
	clearInterval(Timer);
}
</script>
<b class="visitorBold" style="background-color:#999999;border:1px solid #000000;">NRML Live Chat</b>
<table width=400 cellpadding=0 cellspacing=0 border=0 align="center">
    <tr>
        <td align="left" valign="top"><div id="visitorFrame" class="scroll">
            <table id="chatMessageTab" width="100%" border="0" cellspacing="2" cellpadding="2">
			<tbody id="chatMessageTbody">
                <tr>
                    <th colspan="2" align="left" valign="top" class="operatorStatus">Hi, How may I help you?</th>
                </tr>
                <tr align="left" valign="top">
                    <td width="10%"></td>
					<td width="90%"></td>
                </tr>
			</tbody>
            </table>
        </div></td>
    </tr>
    <tr>
        <td align="left" valign="top"><table width="100%" cellpadding=1 cellspacing=1 border=0>
                <form action="#" name=sendtext onSubmit='javascript: return LoadMessage();'>
                    <tr>
                        <td width=80% align=left><textarea name="message" cols=40 rows=2></textarea></td>
                        <td width=20% align=center><input type=submit value=send></td>
                    </tr>
                </form>
            </table></td>
    </tr>
</table>
<?php
}

?>
</body>
</html>
