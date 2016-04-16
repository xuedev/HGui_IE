
function setLogFlag(flag){
		var Run = getExeStr("CommCom","SetLogFlag",flag,""); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Run);
	}
function handleOpenComRet(data){
	//alert(data);
	if(data == "1,001"){
		alert("打开串口成功");
	}else{
		alert("打开串口失败");
	}
}
function openCom(port,baud){
		var Run = getExeStr("CommCom","OpenCom",port+","+baud,"handleOpenComRet"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Run);
	}

function handleCloseComRet(data){
	if(data == "1,001"){
		alert("关闭串口成功");
	}else{
		alert("关闭串口失败");
	}
}
function closeCom(port){
		var Run = getExeStr("CommCom","CloseCom",port,"handleCloseComRet"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Run);
	}

function handleSendComData(data){
	//alert(data);
}
function sendComData(port,data){
		var Run = getExeStr("CommCom","SendHexData",port+","+data,"handleSendComData"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Run);
	}

function handleReadHexData(data){
	var frm = top.document.frames["testCom"];
	if(frm != undefined){
		var html = frm.document.getElementById("recv").innerHTML;
		frm.document.getElementById("recv").innerHTML = html+data;
	}
}
function readHexData(port,len){
		var Run = getExeStr("CommCom","ReadHexData",port+","+len,"handleReadHexData"); //'{"module":"HardwareUtils","func":"GetArmPrinterState","param":"","callback":"js:printer_code = \'%r\';"}';
		external.AsyncRun(Run);
	}

