<?php
require_once("include/entryPoint2.php");
require_once("modules/VisitorsOnline/VisitorsOnline.php");
require_once("modules/Visitors/Visitor.php");
require_once("modules/OperatorOnline/OperatorOnline.php");
require_once("modules/SiteDetails/SiteDetail.php");

$sess_id = session_id();
$ip = $_SERVER['REMOTE_ADDR'];
$siteid = $_REQUEST['siteid'];

$siteObj = new SiteDetail();

if(!$ip || !$siteid || !$siteObj->cross_check($siteid)) {
	 die;
}

$sessionid = $sess_id.$ip;
$contact_id = md5($sessionid);

$op_online = new OperatorOnline();
$operator_id = $op_online->get_operator_online($siteid);

if($operator_id) {
?>
<script src="js/yahoo-min.js"></script>
<script src="js/event-min.js"></script>
<script src="js/connection-min.js"></script>
<script src="js/JSON.js"></script>

<script>
var NPSite = "<?php echo $_config['site_url']; ?>";
var NPproxy = "<?php echo $_config['visitor_proxy']; ?>";
var NPpingTime = <?php echo $_config['ping_time']; ?>;
var NPsyncLock = false;
var NPcontactId = "<?php echo $contact_id; ?>";
var NPsiteId = "<?php echo $siteid; ?>";
var NPoperatorId = "<?php echo $operator_id; ?>";
var NPTimer;
var proActivediv;

function startNPTimer() {
	NPTimer = setInterval("syncServer()", NPpingTime);
}

function closeNPTimer() {
	clearInterval(NPTimer);
}

function NPprocess(type, msg) {
	if(type == 'invite') {
		proactive(); 
	}
}

var openChatWindow = function() {
	window.open(NPSite + '/service/livechat.php?siteid=' + NPsiteId + '&operator_id=' + NPoperatorId + '&contact_id=' + NPcontactId, 'LiveChat','location=0,status=0,scrollbars=0,width=426,height=320');
}

function proactive() {
	
	try {
		if(parent.document.getElementById("proActiveContainer") == null )
			proActivediv = parent.document.createElement("div");
		else
			proActivediv = parent.document.getElementById("proActiveContainer");

		proActivediv.setAttribute("id", "proActiveContainer");
		proActivediv.setAttribute("style", "left:400px;top:200px;");

		proActivediv.style.width = "200px";
		proActivediv.style.height = "100px";
		proActivediv.style.left = 400;
		proActivediv.style.top = 200;
		proActivediv.style.backgroundColor = "#CCCCCC";
		proActivediv.style.border = "1px solid #0066FF";
		proActivediv.style.zIndex = 999;
		proActivediv.style.position = 'absolute';

		proActivediv.innerHTML = '<p width="200" align="center"><b>Wanna chat? Click Here</b><br /><a href="javascript:void(document.openChatWindow());"><img src="' + NPSite + '/service/images/proactive.gif" align="middle" width="179" height="66" alt="" border="0"/></a><br><br><a href="javascript:void(document.getElementById(\'proActiveContainer\').style.display=\'none\');">(x) Close</a></p>';

		parent.document.body.appendChild(proActivediv);
		parent.document.openChatWindow = openChatWindow;
	} catch(e) {
		alert(e + " proactive");
	}
}

startNPTimer();

function syncServer()
{
	var handleSuccess = function(o) {
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

					try {
						NPprocess(arr[2], arr[0]);
					} catch(e) {
						alert("before_process call:"+e.message);
					}
					
					if(arr[2] == 'finish' || arr[2] == 'closed') {
						closeNPTimer();
					}
					
				}
			} catch(e) {
				alert(e + " syncServer");
			}
		}
	}

	var callback = { success:handleSuccess, failure:handleSuccess};
	var postData = '{"args":{"contact_id":"' + NPcontactId + '","sender":"' + NPcontactId + '","site_id":"' + NPsiteId + '","receiver":"' + NPcontactId + '"},';
	postData += '"module":"ChatBuffer","action":"get_read"}';

	var request = YAHOO.util.Connect.asyncRequest('POST', NPproxy, callback, postData);
}


</script>
<div id="container"></div>
<a href="javascript:void(openChatWindow());">
<?php
	print '<IMG SRC="' . $_config['site_url'] . '/service/images/livechat_on.gif" WIDTH="157" HEIGHT="71" BORDER="0" ALT="">';
	print "</a>";
} else {
	print '<IMG SRC="' . $_config['site_url'] . '/service/images/livechat_off.gif" WIDTH="157" HEIGHT="71" BORDER="0" ALT="">';
}

$online = new VisitorsOnline();
$online->retrieve($sessionid);
$visitor = new Visitor();
//print_r($online);die;;

if($online->id) {
	$visitor->retrieve($sessionid);
	$visitor->id = $sessionid;
	$visitor->ip = $ip;
	$visitor->save();
} else {
	$online->id = $sessionid;
	$online->httpsession = $sess_id;
	$online->ip = $ip;
	$online->site_id = $siteid;
	$online->operator_id = $operator_id;
	$online->contact_id = $contact_id;
	$online->status = 'online';
	$online->is_new = true;
	$online->save();
	
	$visitor->retrieve($sessionid);
	if(empty($visitor->id))
		$visitor->is_new = true;
	$visitor->id = $sessionid;
	$visitor->ip = $ip;
	$visitor->save();
}


exit;
?>