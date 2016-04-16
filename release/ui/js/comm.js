///**
window.alert = function(msg){
			$.messager.show({
					title:'提示',
					msg:msg
				});
		}
//**/
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

function CloseAllTab(){
	var tabs = $('#tt').tabs("tabs");
	$.each(tabs,function(){
		try{
			var tab = this;
			var opt = tab.panel("options");
			var currTitle = opt.title;
			$('#tt').tabs('close',currTitle);
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

///////////////////////////////测试代码段///////////////////////////////////////
var varTest;
function TestProc(){
	var Test = getExeStr("Test","Test","Test","js:varTest=\'%r\';"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Test);
		alert(varTest);
}


//////////////////////////查询系统核心参数//////////////////////////////////////////

	function getSysCoreInfo() {
				var ret = external.AsyncRun(queryArmStateInfo);
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
function changeToService(){
	var Run = getExeStr("SoftwareUtils","ChangeToService","","js:;"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
	external.Control(Run);
}


function RunPhsp(){
	WinExec("D:\\\\ArmAdmin\\\\plugins\\\\CapVideo.exe");
}

function RunTest(){
	WinExec("C:\\\\inspur\\\\tools\\\\ARM部品测试程序.exe");
}
function ClearSys(){
	WinExec(".\\\\清理.bat");
}
var currVolume=0;
function handleCurrVolume(data){
		currVolume = data-0;
		var frm = top.document.frames["shortcut"];
		//data = "armjkks##302255@@303003@@307004@@309000@@304000@@314001@@311000@@305001@@armjkjs";
		if(frm != undefined){
			frm.document.getElementById("volumeValue").innerHTML = "当前音量："+currVolume+"";
		}
}

//音量设置回调函数
function handleVolumeValue(data){
	top.document.frames["testVolume"].document.getElementById('value1').value = data;
}

////////////////////数据组件//////////////////////////

var maxValue;
function queryMaxValue(db_file,table){
	var query =   '{"module":"DataAccess","func":"QueryData","param":"'+db_file+'@select max(id) as max from '+table+'","callback":"handlQueryMaxValue"}';
	external.Run(query);
}
function handlQueryMaxValue(data){
	var json = eval("("+data+")");
	maxValue = json[0].max;
}
function queryData(db_file,sql,callback){
	var query =   '{"module":"DataAccess","func":"QueryData","param":"'+db_file+'@'+sql+'","callback":"'+callback+'"}';
	external.AsyncRun(query);
}
function insertData(db_file,data,callback){
	var insert =   '{"module":"DataAccess","func":"InsertData","param":"'+db_file+'@'+data+'","callback":"'+callback+'"}';
	external.Run(insert);
}
function exeSql(db_file,sql,callback){
	var exeSql =   '{"module":"DataAccess","func":"ExecuteSql","param":"'+db_file+'@'+sql+'","callback":"'+callback+'"}';
	external.Run(exeSql);
}

//菜单回调函数
//////////////////////////////////////////////////////
function handleQueryMenu(data){
	try{
		var json = eval("("+data+")");
		for(var i=0;i<json.length;i++){
			var menu_parent = json[i].menu_parent;
			var menu_id = json[i].id;
			var menu_name = json[i].menu_name;
			if(menu_parent == "0"){
				top.addMainMenu(menu_id,menu_name);
			}
		}
		
		for(var i=0;i<json.length;i++){
			var menu_parent = json[i].menu_parent;
			var menu_id = json[i].id;
			var menu_name = json[i].menu_name;
			var menu_url = json[i].menu_url;
			if(menu_parent != "0"){
				top.addMenu(menu_parent,menu_name,menu_url,menu_id);
			}
		}

		$('#menus').accordion();

	}catch(e){
		alert(data);
		return;
	
	}
}

function handleQueryMenu_Update(data){
	try{
		var frm = top.document.frames["cdsz"];
		if(frm != undefined){
			var json = eval("("+data+")");
			for(var i=0;i<json.length;i++){
				var menu_parent = json[i].menu_parent;
				var menu_id = json[i].id;
				var menu_name = json[i].menu_name;
				var position = json[i].position;
				var menu_url = json[i].menu_url;
				if(menu_parent == "0"){
					frm.setMainMenu(menu_id,menu_name,menu_url,position);
				}
			}
			
			for(var i=0;i<json.length;i++){
				var menu_parent = json[i].menu_parent;
				var menu_id = json[i].id;
				var menu_name = json[i].menu_name;
				var menu_url = json[i].menu_url;
				var position = json[i].position;
				if(menu_parent != "0"){
					frm.setMenu(menu_parent,menu_name,menu_url,menu_id,position);
				}
			}

			frm.applyUI();
		}
	}catch(e){
		alert(data);
		return;
	
	}
}

