window.alert = function(msg){
			$.messager.show({
					title:'提示',
					msg:msg
				});
		}

Date.prototype.format = function(format){

	 var o = {
	  "M+" :  this.getMonth()+1,  //month
	  "d+" :  this.getDate(),     //day
	  "h+" :  this.getHours(),    //hour
		  "m+" :  this.getMinutes(),  //minute
		  "s+" :  this.getSeconds(), //second
		  "q+" :  Math.floor((this.getMonth()+3)/3),  //quarter
		  "S"  :  this.getMilliseconds() //millisecond
	   }
	 
	   if(/(y+)/.test(format)) {
		format = format.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));
	   }
	 
	   for(var k in o) {
		if(new RegExp("("+ k +")").test(format)) {
		  format = format.replace(RegExp.$1, RegExp.$1.length==1 ? o[k] : ("00"+ o[k]).substr((""+ o[k]).length));
		}
	   }
	 return format;
	}

function formatSeconds(value) {
	   var theTime = parseInt(value);// 秒
	   var theTime1 = 0;// 分
	   var theTime2 = 0;// 小时
	  // alert(theTime);
	   if(theTime > 60) {
		  theTime1 = parseInt(theTime/60);
		  theTime = parseInt(theTime%60);
		  // alert(theTime1+"-"+theTime);
		  if(theTime1 > 60) {
			 theTime2 = parseInt(theTime1/60);
			 theTime1 = parseInt(theTime1%60);
		   }
	   }
		   var result = ""+parseInt(theTime)+"秒";
		   if(theTime1 > 0) {
		   result = ""+parseInt(theTime1)+"分"+result;
		   }
		   if(theTime2 > 0) {
		   result = ""+parseInt(theTime2)+"小时"+result;
		   }
		   return result;
   }

////////////////////////////ajax////////////////////////
function post(url,callback)
	{
		 var　　req;
     　　if (window.XMLHttpRequest) { 
             　　req = new XMLHttpRequest(); 
     　　}else if (window.ActiveXObject) { 
             　　req = new ActiveXObject("Microsoft.XMLHTTP"); 
     　　} 
     　　if(req){
			req.open("POST",url, true); 
			req.onreadystatechange =  function a(){
				 　　if (req.readyState == 4) { 
						 　　if (req.status == 200) {
									if(callback != undefined&&callback != ""&&callback.length>1){
										eval(callback);
										return;
									}
								}
				 　　}
			 }; 
			 req.send(); 
     　　} 
	}


//////////////////////界面的方法//////////////////////////
var first = 1;
function OnTabSelect(title){
	if($('#tt').tabs("tabs").length<2)
		return;
	var tabs = $('#tt').tabs("tabs");
	$.each(tabs,function(){
		try{
			var tab = this;
			var opt = tab.panel("options");
			var currTitle = opt.title;
			if(currTitle != title){
				$('#tt').tabs('close',currTitle);
			}
		}catch(e){
		
		}
		
	});
	
}

///////////////////////////////公共的一些方法////////////////////////////////////
	
	function getExeStr(module,funcName,param,callback){
			return  '{"module":"'+module+'","func":"'+funcName+'","param":"'+param+'","callback":"'+callback+'"}';
	}


	function CfgUtils(iniFileName,sessionName,key,callback){
		this.iniFileName = iniFileName;
		this.sessionName = sessionName;
		this.key = key;
		this.getExeStr = function(){
			//return  '{"module":"SoftwareUtils","func":"GetIniCfg","param":"'+iniFileName+','+sessionName+','+key+'","callback":"js:alert(\'%r\');"}';
			return getExeStr("SoftwareUtils","GetIniCfg",iniFileName+','+sessionName+','+key,callback);
		}
	};

	function getCfgValue(sessionName,key,callback){
		var cfg = new CfgUtils("arm_state_cfg.ini",sessionName,key,callback);
		external.Control(cfg.getExeStr());
	}
/////////////////////////全局参数//////////////////////////////////////////
	var strWarnImg = "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<IMG SRC=\"img/warning.jpg\" WIDTH=\"25\" HEIGHT=\"25\" BORDER=\"0\" ALT=\"\">";

	var queryArmStateInfo = getExeStr("Test","GetArmStateInfo","","js:handleSysCoreInfo(\'%r\');");//'{"module":"Test","func":"GetArmStateInfo","param":"","callback":"js:handleSysCoreInfo(\'%r\');"}';
	var GetArmPrinterState = getExeStr("HardwareUtils","GetArmPrinterState","","js:printer_code = \'%r\';"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
	
	var check_frequency;
	var printer_code;
	var arm_run_time;


///////////////////////////////测试代码段///////////////////////////////////////
var varTest;
function TestProc(){
	var Test = getExeStr("Test","Test","Test","js:varTest=\'%r\';"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Test);
		alert(varTest);
}


//////////////////////////查询系统核心参数//////////////////////////////////////////

	function getSysCoreInfo() {
				//var ret = external.AsyncRun(queryArmStateInfo);
				alert(1);
			}
	function handleSysCoreInfo(data){
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			var arr = data.split(",");
			frm.document.getElementById("cpu_rate").innerHTML = arr[0]+"%";
			frm.document.getElementById("ram_rate").innerHTML = arr[1]+"%";
		}
		post(server_listen_url+"?flag=1&data="+data);
	}
////////////////////////查询ARM热敏打印机状态//////////////////////////////////////////
	var arm_printer_state=".";
	function getArmPrinterState(){
		external.Control(GetArmPrinterState);
		var cfg = new CfgUtils("arm_comm_code.ini","code",printer_code,"js:arm_printer_state=\'%r\';");
		external.Control(cfg.getExeStr());
		//alert(arm_printer_state);
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			if(arm_printer_state == "")
				return;
			if(arm_printer_state != "302000"){
				arm_printer_state += strWarnImg;
			}
			frm.document.getElementById("arm_printer").innerHTML = arm_printer_state;
		}

	}

/////////////////////////查询ARM后台网络状态//////////////////////////////////////////
	function GetNetStateProc(){
		//alert(server_listen_url);
		var GetNetState = getExeStr("HardwareUtils","CheckNetState",server_listen_url,"handleGetNetState"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(GetNetState);
	}

	function handleGetNetState(data){
		var frm = top.document.frames["armgl"];
		//alert(data);
		if(frm != undefined){
			if(data == "1")
				frm.document.getElementById("arm_server_state").innerHTML = "正常";
			else if(data == "0")
				frm.document.getElementById("arm_server_state").innerHTML = "无法连接到ARM服务器"+strWarnImg;
		}
	}
/////////////////////////查询ARM的IP、MAC地址//////////////////////////////////////////
	function GetThisIpInfoProc(){
		var GetThisIpInfo = getExeStr("HardwareUtils","GetThisIpInfo","","handleGetThisIpInfo"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(GetThisIpInfo);
	}

	function handleGetThisIpInfo(data){
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			var arr = data.split(",");
			frm.document.getElementById("ip_addr").innerHTML = arr[0];
			frm.document.getElementById("mac_addr").innerHTML = arr[1];
		}
	}
/////////////////////////查询ARM的串口状态//////////////////////////////////////////
	function GetGetComInfoProc(){
		var GetComInfo = getExeStr("HardwareUtils","GetComInfo","","handleGetComInfo"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(GetComInfo);
	}

	function handleGetComInfo(data){
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			var arr = data.split(",");
			var str = "";
			for(var i=0;i<arr.length;i++){
				str += "Com"+(i+1);
				if(arr[i] == "0"){
					str += "已用<br>";
				}else{
					str += "正常<br>";
				}
			}
			frm.document.getElementById("com_state").innerHTML = str;
		}
	}
/////////////////////////查询ARM的运行时间//////////////////////////////////////////

/**
	function SetArmRunTimeProc(t){
		var param = "arm_state_cfg.ini,threshold,arm_run_time,"+t;
		var SetArmRunTime = getExeStr("SoftwareUtils","SetIniCfg",param,"js:;");
		//var  GetArmRunTime = getExeStr("HardwareUtils","GetComInfo","","handleGetArmRunTime"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.Control(SetArmRunTime);
	}
	
	function GetArmRunTimeProc(){
		var GetArmRunTime = getExeStr("Test","GetArmStateCfg","arm_run_time","handleGetArmRunTime");
		//var  GetArmRunTime = getExeStr("HardwareUtils","GetComInfo","","handleGetArmRunTime"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun( GetArmRunTime);
	}

	function handleGetArmRunTime(data){
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			frm.document.getElementById("arm_run_time").innerHTML = formatSeconds(data);
		}

		var t = parseInt(data);
		var t1 = parseInt(check_frequency);

		var tt = t+t1+0;
		//alert(data+":"+t+":"+t1+":"+tt);
		SetArmRunTimeProc(tt);
	}


**/
	
	function GetArmRunTimeProc(){
		var GetArmRunTime = getExeStr("SoftwareUtils","GetSysRunTime","","handleGetArmRunTime");
		//var  GetArmRunTime = getExeStr("HardwareUtils","GetComInfo","","handleGetArmRunTime"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun( GetArmRunTime);
	}

	function handleGetArmRunTime(data){
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			frm.document.getElementById("arm_run_time").innerHTML = formatSeconds(data/1000);
		}
		post(server_listen_url+"?flag=2&data="+data);
	}

/////////////////////////得到ARM后台的路径配置///////////////////////////////////////
var rootUrl;
function getServerRootUrl(){
	getCfgValue("urls","server_root","js:rootUrl=\'%r\';");
}

var updateUrl;
function getUpdateUrl(){
	getCfgValue("urls","server_update_url","js:updateUrl=\'%r\';");
	updateUrl = rootUrl+updateUrl;
}

var pzglUrl;
function getPzglUrl(){
	getCfgValue("urls","server_pzgl_url","js:pzglUrl=\'%r\';");
	pzglUrl = rootUrl+pzglUrl;
}

var server_listen_url;
function getServerListenUrl(){
	getCfgValue("urls","server_listen_url","js:server_listen_url=\'%r\';");
	server_listen_url = rootUrl+server_listen_url;
}
var server_armgl_url;
function getServerArmglUrl(){
	getCfgValue("urls","server_armgl_url","js:server_armgl_url=\'%r\';");
	server_armgl_url = rootUrl+server_armgl_url;
}

var server_kc_url;
function getArmKcUrl(){
	getCfgValue("urls","server_kc_url","js:server_kc_url=\'%r\';");
	server_kc_url = rootUrl+server_kc_url;
}

function GetCheckFrequency(){
	//var GetCheckFrequency = getExeStr("Test","GetArmStateCfg","check_frequency","js:check_frequency=\'%r\';"); //'{"module":"Test","func":"GetArmStateCfg","param":"check_frequency","callback":"js:check_frequency=\'%r\';"}';
	getCfgValue("threshold","check_frequency","js:check_frequency=\'%r\';");
	
}

/////////////////////////查询硬件状态//////////////////////////////////////

var devName = "";
var devState = "";
function GetDetectDetail(data){
	var allDevState = "";

	var arr = data.split("@@");
	if(arr.length>0){
		for(var i=0;i<arr.length-1;i++){
			var str;
			var dev;
			if(i==0){
				var len = arr[i].length;
				str = arr[i].substring(9,len);
			}else{
				str = arr[i];
			}

			dev = str.substring(0,3);
			var cfg = new CfgUtils("arm_comm_code.ini","code",dev,"js:devName=\'%r\';");
			external.Control(cfg.getExeStr());

			var cfg = new CfgUtils("arm_comm_code.ini","code",str,"js:devState=\'%r\';");
			external.Control(cfg.getExeStr());


			allDevState += devState+"<br>";
		}

		return allDevState;
	}
}

function GetDetectProc(){
		if(CheckAppIsRun("iexplore.exe") == "1"){
			return;
		}
		var GetDetect = getExeStr("detect","GetArmDeviceState","","handleGetDetect");
		//var  GetArmRunTime = getExeStr("HardwareUtils","GetComInfo","","handleGetArmRunTime"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun( GetDetect);
	}

function handleGetDetect(data){
		var frm = top.document.frames["armgl"];
		//data = "armjkks##302255@@303003@@307004@@309000@@304000@@314001@@311000@@305001@@armjkjs";
		if(frm != undefined){
			frm.document.getElementById("device_info").innerHTML = GetDetectDetail(data);
		}

		var frm2 = top.document.frames["zdbpjk"];
		if(frm2 != undefined){
			frm2.document.getElementById("device_info").innerHTML = GetDetectDetail(data);
		}

		GetGetComInfoProc();
	}

//////////////////////////执行CMD命令////////////////////////////////////////////////
function RunCmdProc(params){
		var RunCmd = getExeStr("SoftwareUtils","RunCmd",params,"js:;"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(RunCmd);
	}

function showdown(){
	RunCmdProc("shutdown -s -t 10000");
}

function reboot(){
	RunCmdProc("shutdown -r -t 10000");
}

//////////////////////////////查询ARM锁机、关机状态/////////////////////////////////////////
var cmd;
function getServerCmd(){
	post(server_armgl_url,"cmd = req.responseText;handleServerCmd(cmd);");
}

var oldCmd = "";
var oldNotice = "";

//默认不锁机
var oldLock = "1";

function RunCmd(data){
	//alert(data);
	if(data == oldCmd)
		return;

	alert("执行cmd命令："+data);
	oldCmd = data;

}

function SetNotice(data){
	//alert(data);
	if(data == oldNotice)
		return;
	var params = "E:\\\\ftxt.txt@my_txt="+data;
	var Run = getExeStr("SoftwareUtils","WriteMyFile",params,"js:;"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
	external.AsyncRun(Run);
	oldNotice = data;
}

function KillApp(data){
	var Run = getExeStr("SoftwareUtils","KillApp",data,"js:;"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
	external.Control(Run);
}

var isRun = "0";
function CheckAppIsRun(data){
	var Run = getExeStr("SoftwareUtils","CheckAppIsRun",data,"js:isRun=\'%r\';"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
	external.Control(Run);
	return isRun;
}

function WinExec(data){
	var Run = getExeStr("SoftwareUtils","OpenApp",data,"js:;"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
	external.Control(Run);
}
function LockArm(data){
	//alert(data);
	if(data == oldLock)
		return;
	
	
	if (data.split(",")[0] == "0")
	{
		//::MessageBox(NULL,"3","",0);
		KillApp("iexplore.exe");
		KillApp("ArmLaunch.exe");
		KillApp("armFlash.exe");
		if(CheckAppIsRun("LockWindow.exe") == "0"){
			//::MessageBox(NULL,"4","",0);
			WinExec("C:\\\\LockWindow.exe");
		}
	}else{
		//::MessageBox(NULL,"5","",0);
		if (CheckAppIsRun("LockWindow.exe") == "1"){
			KillApp("LockWindow.exe");
			//::MessageBox(NULL,"6","",0);
			if(CheckAppIsRun("ArmLaunch.exe") == "0"){
				//::MessageBox(NULL,"7","",0);
				WinExec("C:\\\\ArmLaunch.exe");
			}
		}
	}

	oldLock = data;

}


function handleServerCmd(){
	if(cmd != undefined&&cmd != ""&&cmd.length>1){
		var arr = cmd.split("@");
		for(var i=0;i<arr.length-1;i++){
			var data = arr[i].split(",");
			switch(data[1]){
				case "SYSTEM_COMMAND":
					RunCmd(data[2]);
					break;
				case "SELF_COMMAND":
					if(data[2] == "NOTICE"){
						SetNotice(data[3]);
					}else if(data[2] == "LOCK"){
						LockArm(data[3]);
					}
					break;

			}
			
		}
	}
	
}
//////////////////////////////查询本终端库存情况/////////////////////////////////////////
var kc;
function getArmKc(){
	post(server_kc_url,"kc = req.responseText;handleArmKc();");
}

function handleArmKc(){
	if(kc != undefined&&kc != ""&&kc.length>1){
		var arr = kc.split("@");
		var str = "";
		for(var i=0;i<arr.length-1;i++){
			var data = arr[i].split(",");
			//alert(arr[i]);
			str += data[0]+"-剩余："+data[1]+"份<br>";
		}
		var frm = top.document.frames["armgl"];
		if(frm != undefined){
			frm.document.getElementById("arm_kc").innerHTML = str;
		}
	}
	
}
