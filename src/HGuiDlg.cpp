
// HGuiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HGui.h"
#include "HGuiDlg.h"
#include "afxdialogex.h"
#include <string>

using namespace std;


// CHGuiDlg 对话框

BEGIN_DHTML_EVENT_MAP(CHGuiDlg)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CHGuiDlg, CDHtmlDialog)
    // example:
    DISP_FUNCTION(CHGuiDlg,"SetUrlJsMap",SetUrlJsMap,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CHGuiDlg,"AsyncRun",AsyncRun,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CHGuiDlg,"Control",Control,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CHGuiDlg,"Run",Control,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CHGuiDlg,"SetNoTitle",SetNoTitle,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CHGuiDlg,"Close",Close,VT_NULL,VTS_BSTR)
	DISP_FUNCTION(CHGuiDlg,"ChangeMaxMin",ChangeMaxMin,VT_NULL,VTS_BSTR)
	
    //                                                                        ^return,        ^parameters type list
    //每个方法都需要在这里添加映射
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CHGuiDlg, CDHtmlDialog)
ON_EVENT(CHGuiDlg, AFX_IDC_BROWSER,273,OnNewWindow3, VTS_PDISPATCH VTS_PBOOL VTS_I4 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

BOOL CHGuiDlg::CanAccessExternal(){
	return TRUE;
}

map<string,HMODULE> CHGuiDlg::dllMap;
AutoControl CHGuiDlg::ac;
CHGuiDlg::CHGuiDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CHGuiDlg::IDD, CHGuiDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//让CDHtmlDialog对象自身支持自动化
	EnableAutomation(); 
	EnableActiveAccessibility();
	EnableAggregation();
	EnableConnections();
	EnableTypeLib();

	szTitle = "";
	isNoTitle = FALSE;

	memset(szCfg,0,255);
	memset(szCurrDir,0,255);
	memset(szLogPath,0,255);

	TCHAR szTmp[255] = _T("");
	memset(szInitPage,0,255);

	::GetCurrentDirectory(255,szCurrDir);
	StrCat(szCfg,szCurrDir);
	StrCat(szCfg,_T("\\set.ini"));
	::GetPrivateProfileString(_T("set"),_T("init_page"),_T("index.html"),szTmp,255,szCfg);
	StrCat(szInitPage,szCurrDir);
	StrCat(szInitPage,szTmp);

	//得到程序标题
	TCHAR tmp1[255] = L"";
	::GetPrivateProfileString(_T("set"),_T("app_title"),_T("HGui"),tmp1,255,szCfg);
	szTitle.Format(L"%s",tmp1);
	
	width = GetPrivateProfileInt(_T("set"),_T("width"),0,szCfg);
	height = GetPrivateProfileInt(_T("set"),_T("height"),0,szCfg);

	title_height = GetPrivateProfileInt(_T("set"),_T("title_height"),0,szCfg);

	context_menu = GetPrivateProfileInt(_T("set"),_T("context_menu"),0,szCfg);

	int nLogOpen = GetPrivateProfileInt(_T("set"),_T("log_open"),0,szCfg);
	if(nLogOpen == 1){
		::GetPrivateProfileString(_T("set"),_T("log_path"),_T("C:\\"),szLogPath,255,szCfg);
		//AfxMessageBox(szLogPath);
		strcat(ac.szLogPath,ac.UnicodeToAnsi(szLogPath));
		ac.bIsOpenLog = TRUE;
	}
	///**
	//初始化地址js映射
	TCHAR szJs[255] = L"";
	StrCat(szJs,szCurrDir);
	StrCat(szJs,L"\\js\\urls_map.json");
	//**/
	CStdioFile file;
	CString strJson = L"";
	if(file.Open(szJs,CFile::modeRead)){
		CString tmp = L"";
		while(file.ReadString(tmp)){
			strJson += tmp;
		}
	}
	SetUrlJsMap(strJson.AllocSysString());

	int is_single= GetPrivateProfileInt(_T("set"),_T("is_single"),1,szCfg);
	//**/
	//单例模式
	if(is_single == 1){
		HWND hwnd = ::FindWindow(L"#32770",szTitle.GetBuffer(0));
		if(hwnd != NULL){

			//::SetFocus(hwnd);
			//::SetForegroundWindow(hwnd);
		   // ::BringWindowToTop(hwnd);
			//::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

			::ShowWindow(hwnd,SW_MAXIMIZE);
			::SetForegroundWindow(hwnd);
			PostQuitMessage(0);
		}
	}
}

void CHGuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHGuiDlg, CDHtmlDialog)
	ON_MESSAGE(WM_CALLJS_MESSAGE, OnCallJScript) 
//	ON_WM_KEYDOWN()
ON_WM_PARENTNOTIFY()
ON_WM_CLOSE()
//ON_WM_LBUTTONDOWN()
//ON_WM_NCHITTEST()
//ON_WM_LBUTTONDOWN()
ON_WM_SIZE()
END_MESSAGE_MAP()


// CHGuiDlg 消息处理程序

UINT CHGuiDlg::ProcSafe(LPVOID lParams){
	CHGuiDlg* params = (CHGuiDlg*)lParams;
	HWND hwndSafe;
	HWND btnOk;
	while(bRun){
		hwndSafe = ::FindWindowA("#32770","Internet Explorer");
		if(hwndSafe){
			params->ac.Log(L"出现安全验证窗口");
			btnOk = ::GetDlgItem(hwndSafe,0x1);
			params->ac.ClickButton(btnOk);
		}
		Sleep(100);
	}
	return 0;
}

BOOL CHGuiDlg::bRun = TRUE;

BOOL CHGuiDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	SetExternalDispatch(GetIDispatch(TRUE)); //将浏览器控件的扩展接口设置为对话框自身的IDispatch

	int has_scrollbar = GetPrivateProfileInt(_T("set"),_T("has_scrollbar"),0,szCfg);
	if(has_scrollbar == 1){
		SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR);
	}
	//SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR);
	//SetWindowLong(GetSafeHwnd(),   GWL_STYLE,   GetWindowLong(GetSafeHwnd(),   GWL_STYLE)   -   WS_CAPTION);
	//TCHAR* cmd = GetCommandLine();
	SetWindowText(szTitle);
	TCHAR cmd[255] = L"";
	::GetPrivateProfileString(_T("set"),_T("show_cfg"),_T("normal"),cmd,255,szCfg);
	CString str;
	str.Format(L"%s",cmd);
	
    //atexit(ExitCheckMomery);
	int has_js_msg = GetPrivateProfileInt(_T("set"),_T("has_js_msg"),1,szCfg);
	if(has_js_msg == 0){
		m_pBrowserApp->put_Silent(VARIANT_TRUE);
	}

    //m_pBrowserApp->put_Silent(VARIANT_TRUE);  
	if(width > 0){
		RECT rect;
		GetWindowRect(&rect);
		MoveWindow(rect.left,rect.top,width,height,TRUE);
	}
	ChangeMaxMin(str.AllocSysString());

	//跳转到主页
	this->Navigate(szInitPage);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//自动处理验证安全性窗口
	//AfxBeginThread(ProcSafe,this,0,0,NULL);
	jsDlg = new JsDlg(this);
	jsDlg->mainDlg = this;
	jsDlg->Create(IDD_DIALOG1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHGuiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHGuiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CHGuiDlg::CallJScript(const CString strFunc, _variant_t* pVarResult)
{
    CStringArray paramArray;
    return CallJScript(strFunc,paramArray,pVarResult);
}

BOOL CHGuiDlg::CallJScript(const CString strFunc,const CString strArg1,_variant_t* pVarResult)
{
    CStringArray paramArray;
    paramArray.Add(strArg1);
    return CallJScript(strFunc,paramArray,pVarResult);
}

LRESULT CallJsFunc(IDispatch* pDispScript, LPOLESTR lpsName, DISPPARAMS* pParams, CComVariant* pvtRet)
{
    ATLASSERT(pDispScript);
     
    //根据函数名字，取得函数序号(DISPID)
    DISPID did = 0;
    HRESULT hr = pDispScript->GetIDsOfNames(IID_NULL, //riid为保留参数，必须设置为IID_NULL
        &lpsName, //rgszNames 字符串数组，指定了成员的函数名及其参数
        1, //标识了名字的个数
        LOCALE_USER_DEFAULT, //指定本地化标识
        &did); //返回数组，每个对应rgszNames的元素
    if (FAILED(hr))
        return hr;
     
     
    // 根据函数序号，解释执行函数功能
    if (pvtRet)
        return pDispScript->Invoke(did, //方法或属性的DISPID
        IID_NULL, //riid为保留参数，必须设置为IID_NULL
        LOCALE_USER_DEFAULT, //指定本地化标识
                       DISPATCH_METHOD, //DISPATCH_METHOD，DISPATCH_PROPERTYGET，

						//DISPATCH_PROPERTYPUT，或DISPATCH_PROPERTYPUTREF
                       pParams, //包含的属性或方法的参数
                       pvtRet, //调用的结果
                       NULL, //异常信息
                       NULL); //产生错误的参数索引
     
    CComVariant vtResult;
    return pDispScript->Invoke(did, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pParams, 

&vtResult, NULL, NULL);
}

BOOL CHGuiDlg::CallJScript(const CString strFunc, const CStringArray& paramArray,_variant_t* pVarResult)
{

	//getscriptdispatch
	CComPtr<IDispatch> spScript;
		if(!GetJScript(spScript))
		{
			//AfxMessageBox(_T("函数GetJScrip调用失败！"));
			return FALSE;
		}

	//createDISPPARAMS
	const int arraySize=paramArray.GetSize();
	CComVariant variant;
	if(arraySize==0)
	{
	DISPPARAMS dispparams={0};
	TCHAR* func = strFunc.AllocSysString() ;
	CallJsFunc(spScript,func,&dispparams,&variant);
	}
	else
	{
	/*
	cArgs:参数个数;
	rgvarg:参数数组;
	cNamedArgs:命名参数个数;
	rgdispidNamedArgs:命名参数的调度号列表*/
	DISPPARAMS dispparams;
	memset(&dispparams,0,sizeof dispparams);
	dispparams.cArgs=arraySize;
	dispparams.rgvarg=new VARIANT[dispparams.cArgs];

	for(int i=0;i<arraySize;i++)
	{
	CComBSTR bstr=paramArray.GetAt(arraySize-1-i);//backreading
	bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
	dispparams.rgvarg[i].vt=VT_BSTR;
	}
	dispparams.cNamedArgs=0;

	CallJsFunc(spScript,strFunc.AllocSysString(),&dispparams,&variant);

	if(dispparams.rgvarg)
	{
	for(int i=0;i<arraySize;i++)
	{
	::SysFreeString(dispparams.rgvarg[i].bstrVal);
	}
	delete[]dispparams.rgvarg;
	dispparams.rgvarg=NULL;
	}
	}
	return TRUE;

	/**
	try{
		 CComPtr<IDispatch> spScript;
		if(!GetJScript(spScript))
		{
			AfxMessageBox(_T("函数GetJScrip调用失败！"));
			return FALSE;
		}
		CComBSTR bstrFunc(strFunc);
		DISPID dispid = NULL;
		HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrFunc,1,
			LOCALE_SYSTEM_DEFAULT,&dispid);
		if(FAILED(hr))
		{
			//MessageBox(GetSystemErrorMessage(hr));
			return FALSE;
		}

		INT_PTR arraySize = paramArray.GetSize();

		DISPPARAMS dispparams;
		memset(&dispparams, 0, sizeof dispparams);
		dispparams.cArgs = (UINT)arraySize;
		dispparams.rgvarg = new VARIANT[dispparams.cArgs];

		for( int i = 0; i < arraySize; i++)
		{
			CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
			bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
			dispparams.rgvarg[i].vt = VT_BSTR;
		}
		dispparams.cNamedArgs = 0;

		EXCEPINFO excepInfo;
		memset(&excepInfo, 0, sizeof excepInfo);
		_variant_t vaResult;
		UINT nArgErr = (UINT)-1;  // initialize to invalid arg

		hr = spScript->Invoke(dispid,IID_NULL,0,
			DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

		delete [] dispparams.rgvarg;
		if(FAILED(hr))
		{
			//MessageBox(GetSystemErrorMessage(hr));
			return FALSE;
		}

		if(pVarResult)
		{
			*pVarResult = vaResult;
		}
		return TRUE;
	}catch(...){
		return FALSE;
	}
	**/
}

BOOL CHGuiDlg::GetJScript(CComPtr<IDispatch>& spDisp)
{
    if( m_spHtmlDoc==NULL )
        return FALSE;

    HRESULT hr = m_spHtmlDoc->get_Script(&spDisp);
    ATLASSERT(SUCCEEDED(hr));
    return SUCCEEDED(hr);
}

typedef struct RunProcParams_{
	CHGuiDlg* parent;
	FunInfo* funInfo;
} RunProcParams;

typedef struct CallJsData_{
	CString strFunName;
	CStringArray* pArray;
} CallJsData;


void CHGuiDlg::Control(BSTR param)
{
	FunInfo funInfo;
	if(ParseFunInfo(&funInfo,param)){
		ac.Log(L"执行 Control 方法");
		BSTR szRet;
		TCHAR pluginPath[255] = L"";
		StrCat(pluginPath,szCurrDir);
		StrCat(pluginPath,L"\\plugins\\");
		StrCat(pluginPath,funInfo.strFunModule);
		StrCat(pluginPath,L".dll");

		char* tmp = ac.UnicodeToAnsi(pluginPath);
		string dll;
		dll.assign(tmp);

		HMODULE hModule= NULL;
		map<string,HMODULE>::iterator i;
		if((i=dllMap.find(dll) ) != dllMap.end()){
			hModule = i->second;
		}else{
			hModule = ::LoadLibraryW(pluginPath);
		}

		if(hModule){
			ac.Log(L"加载DLL："+CString(pluginPath)+L" 成功");
			char* tmp = ac.UnicodeToAnsi(funInfo.strFunName);
			IFun fun = (IFun)::GetProcAddress(hModule,tmp);
			delete[] tmp;
			tmp = NULL;
			if(fun){
				ac.Log(L"在DLL："+CString(pluginPath)+L" 中，查找："+funInfo.strFunName+L" 函数成功");
				ac.Log(L"调用函数："+CString(funInfo.strFunName)+L"；参数："+funInfo.strFunParam);
				szRet = fun(funInfo.strFunParam,GetSafeHwnd());
				ac.Log(L"执行："+CString(funInfo.strFunName)+L" 函数成功，返回值："+szRet);
				 
			}else{
				ac.Log(L"在DLL："+CString(pluginPath)+L" 中，查找："+funInfo.strFunName+L" 函数失败");
					szRet = L"{\"result\":\"error\",\"retVaule\":\"\",\"msg\":\"can not find function\"}";
				}
		}else{
			ac.Log(L"加载DLL："+CString(pluginPath)+L" 失败");
			szRet =  L"{\"result\":\"error\",\"retVaule\":\"\",\"msg\":\"can not find module\"}";
		}

		CStringArray* arr = new CStringArray();
		arr->Add(szRet);
		CallJsData* data = new CallJsData();
		data->pArray = arr;
		data->strFunName = funInfo.strCallback;
		SendMessage(WM_CALLJS_MESSAGE,0,(LPARAM)data);
		//FreeLibrary(hModule);
		hModule = NULL;
	}
}


BSTR CHGuiDlg::Run(FunInfo* funInfo){
	ac.Log(L"执行 Run 方法");
	RunProcParams* params = new RunProcParams();
	params->parent = this;
	params->funInfo = funInfo;
	AfxBeginThread(RunProc,(LPVOID)params,0,0,NULL);
	return L"1";
}

BOOL CHGuiDlg::ParseFunInfo(FunInfo* funInfo,BSTR param){
	Json::Value value;
	Json::Reader reader;
	char* tmp = ac.UnicodeToAnsi(param);
	if (!reader.parse(tmp,value))
	{
		delete[] tmp;
		tmp = NULL;
		return false;
	}
	delete[] tmp;
	tmp = NULL;
	funInfo->strFunModule = ac.AnsiToUnicode(value["module"].asCString());
	//AfxMessageBox(funInfo->strFunModule);
	funInfo->strFunName = ac.AnsiToUnicode(value["func"].asCString());
	funInfo->strFunParam = ac.AnsiToUnicode(value["param"].asCString());
	funInfo->strCallback = ac.AnsiToUnicode(value["callback"].asCString());
	return true;
}

BOOL CHGuiDlg::IsInvokeAllowed(DISPID /*dispid*/)
{
	// TODO: 在此添加专用代码和/或调用基类

	return TRUE;
}

//异步处理方法
void CHGuiDlg::AsyncRun(BSTR params)
{
	CString log = L"执行 AsyncRun 方法，参数："+CString(params);
	ac.Log(log.GetBuffer(0));
	FunInfo* funInfo = new FunInfo();
	if(ParseFunInfo(funInfo,params)){
		Run(funInfo);
	}
}


UINT CHGuiDlg::RunProc(LPVOID lParams)
{
	RunProcParams* params = (RunProcParams*)lParams;
	params->parent->ac.Log(L"执行 RunProc 方法");
	BSTR szRet;
	TCHAR pluginPath[255] = L"";
	StrCat(pluginPath,params->parent->szCurrDir);
	StrCat(pluginPath,L"\\plugins\\");
	StrCat(pluginPath,params->funInfo->strFunModule);
	StrCat(pluginPath,L".dll");
	char* tmp = ac.UnicodeToAnsi(pluginPath);
	string dll;
	dll.assign(tmp);
	delete[] tmp;
	tmp = NULL;
	HMODULE hModule= NULL;
	map<string,HMODULE>::iterator i;
	if((i=dllMap.find(dll) ) != dllMap.end()){
		hModule = i->second;
	}else{
		hModule = ::LoadLibraryW(pluginPath);
	}
	
	if(hModule){
		//params->parent->ac.Log(L"加载DLL："+CString(pluginPath)+L" 成功");
		char* tmp = ac.UnicodeToAnsi(params->funInfo->strFunName);
		//::MessageBoxA(NULL,tmp,"",0);
		IFun fun = (IFun)::GetProcAddress(hModule,tmp);
		delete[] tmp;
		tmp = NULL;
		if(fun){
			//params->parent->ac.Log(L"在DLL："+CString(pluginPath)+L" 中，查找："+params->funInfo->strFunName+L" 函数成功");
			//params->parent->ac.Log(L"调用函数："+params->funInfo->strFunName+L"；参数："+params->funInfo->strFunParam);
			szRet = fun(params->funInfo->strFunParam,params->parent->GetSafeHwnd());
			params->parent->ac.Log(L"执行："+CString(params->funInfo->strFunName)+L" 函数成功，返回值："+szRet);
		}else{
			params->parent->ac.Log(L"在DLL："+CString(pluginPath)+L" 中，查找："+params->funInfo->strFunName+L" 函数失败");
				szRet = L"{\"result\":\"error\",\"retVaule\":\"\",\"msg\":\"can not find function\"}";
			}
	}else{
		params->parent->ac.Log(L"加载DLL："+CString(pluginPath)+L" 失败");
		szRet =  L"{\"result\":\"error\",\"retVaule\":\"\",\"msg\":\"can not find module\"}";
	}
	
	CStringArray* arr = new CStringArray();
	arr->Add(szRet);
	CallJsData* data = new CallJsData();
	data->pArray = arr;
	data->strFunName = params->funInfo->strCallback;
	params->parent->SendMessage(WM_CALLJS_MESSAGE,0,(LPARAM)data);

	delete params->funInfo;
	params->funInfo = NULL;
	delete params;
	params = NULL;
	 
	//FreeLibrary(hModule);
	hModule = NULL;

	return 0;
}

LRESULT CHGuiDlg::OnCallJScript(WPARAM wParam, LPARAM lParam){
	CallJsData* data = (CallJsData*)lParam;
	_variant_t vRet;

	CString js;
	js.Format(L"%s",data->strFunName);
	int pos;
	if((pos = js.Find(L"js:"))>-1){
		js = js.Right(js.GetLength()-3);
		js.Replace(L"%r",data->pArray->GetAt(0));
		//AfxMessageBox(js);
		this->ExeScript(js,&vRet);

		delete data->pArray;
		data->pArray = NULL;
		delete data;
		data = NULL;
		return 0;

	}else{
		try{
			CString str = data->pArray->GetAt(0);
			str.Replace(L"\"",L"\\\"");
			str.Replace(L"\n",L"");
			js = data->strFunName+L"(\""+str+L"\");";
			this->ExeScript(js,&vRet);

			/**
			if(CallJScript(data->strFunName,*data->pArray,&vRet)){
				BSTR szRet = vRet.bstrVal;
				ac.Log(L"执行JS函数："+data->strFunName+" 成功");
			}else{
				ac.Log(L"执行JS函数："+data->strFunName+" 失败");
			}
			**/

			delete data->pArray;
			data->pArray = NULL;
			delete data;
			data = NULL;
			return 0;
		}catch(...){
			delete data->pArray;
			data->pArray = NULL;
			delete data;
			data = NULL;
			return 0;
		}
		
	}

	
}

void CHGuiDlg::SetUrlJsMap(BSTR json){
	Json::Value value;
	Json::Reader reader;
	char* tmp = ac.UnicodeToAnsi(json);
	if (!reader.parse(tmp,value))
	{
		delete[] tmp;
		tmp = NULL;
		return;
	}
	delete[] tmp;
	tmp = NULL;
	Json::Value data = value["data"];
	Json::Value::iterator begin = data.begin();
	Json::Value::iterator end = data.end();
	for(;begin != end;begin++){
		CString url = ac.AnsiToUnicode((*begin)["url"].asCString());
		CString js = ac.AnsiToUnicode((*begin)["js"].asCString());
		//MessageBoxA(NULL,(*begin)["url"].asCString(),"",0);
		urlJsMap[url] = js;
	//	AfxMessageBox(url);
	//	AfxMessageBox(js);
	}

	return;
}

void CHGuiDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	CComPtr<IHTMLDocument2> pHtmlDoc;
	CString strUrl;
	strUrl.Format(L"%s",szUrl);
	
    IHTMLDocument2   *objDocument=NULL;
    IHTMLWindow2* pIHTMLWindow = NULL;
	this->GetDHtmlDocument(&objDocument);
    if(objDocument)
    {
		int len = urlJsMap.GetSize();
		POSITION pos = urlJsMap.GetStartPosition();
		CString strKey;
		CString strValue;
		while(pos)
		{
			urlJsMap.GetNextAssoc(pos, strKey, strValue);
			if(strUrl.Find(strKey)>-1){
				objDocument->get_parentWindow(&pIHTMLWindow);
				if(pIHTMLWindow)
				{
					TCHAR jsFile[255] = L"";
					StrCat(jsFile,szCurrDir);
					StrCat(jsFile,L"\\js\\");
					StrCat(jsFile,strValue.GetBuffer(0));
					char* tmp = ac.UnicodeToAnsi(jsFile);
					FILE* f = fopen(tmp,"r");
					delete[] tmp;
					tmp = NULL;
					if(f == NULL){
						return;
					}
					CString strJs = L"";
					CString strTmp = L"";
					char buff[256] = "";
					while(fgets(buff,256,f) != NULL){
						strTmp.Format(L"%s",ac.AnsiToUnicode(buff));
						strJs += strTmp;
						memset(buff,0,256);
					}
					fclose(f);
					VARIANT pvarRet;
					pIHTMLWindow->execScript(CComBSTR(strJs), CComBSTR("JavaScript"), &pvarRet);
					ac.Log(L"执行JS文件："+CString(jsFile));
					pIHTMLWindow->Release();
				}
			}
		}
		objDocument->Release();
    }        
	

}


void CHGuiDlg::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);
}

void CHGuiDlg::SetNoTitle(BSTR params)
{
	//SetWindowLong(GetSafeHwnd(),   GWL_STYLE,   GetWindowLong(GetSafeHwnd(),   GWL_STYLE)   -   WS_CAPTION);
	szTitle = L"";
	szTitle.Format(L"%s",params);
	::WritePrivateProfileString(_T("set"),_T("app_title"),szTitle,szCfg);
	SetWindowText(params);
}

void CHGuiDlg::Close(BSTR params)
{
	//SendMessage(WM_CLOSE,0,0);
	DestroyWindow();
}
void CHGuiDlg::ChangeMaxMin(BSTR params)
{
	//SendMessage(WM_CLOSE,0,0);
	CString str;
	str.Format(L"%s",params);
	if(str.Find(L"no_title")>-1){
		isNoTitle = true;
		//SetWindowLong(GetSafeHwnd(),   GWL_STYLE,   GetWindowLong(GetSafeHwnd(),   GWL_STYLE)   -   WS_CAPTION);
		DWORD dwStyle = GetStyle();//获取旧样式  
		DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;  
		dwNewStyle&=dwStyle;//按位与将旧样式去掉  
		SetWindowLong(m_hWnd,GWL_STYLE,dwNewStyle);//设置成新的样式   
		DWORD dwExStyle = GetExStyle();//获取旧扩展样式  
		DWORD dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR;  
		dwNewExStyle&=dwExStyle;//按位与将旧扩展样式去掉  
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwNewExStyle);//设置新的扩展样式   
		//::SetWindowPos(this->GetSafeHwnd(),HWND_BOTTOM,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);//窗口位置和大小保持原来不变！  
		SetWindowPos( & wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE ); 
	}
	if(str.Find(L"max")>-1){
		ShowWindow(SW_MAXIMIZE);
	}else if(str.Find(L"min")>-1){
		ShowWindow(SW_MINIMIZE);
	}else if(str.Find(L"restore")>-1){
		ShowWindow(SW_NORMAL);
	}
	
}

HRESULT CHGuiDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	//OnOK();
	return S_OK;
}
void CHGuiDlg::OnOK(){
	return;
}

void CHGuiDlg::OnDestroy(){
	bRun = FALSE;
	jsDlg->DestroyWindow();
	delete jsDlg;
	jsDlg = NULL;
}

BOOL CHGuiDlg::PreTranslateMessage(MSG* pMsg){
	if(pMsg->message == WM_KEYDOWN) 
  {
    switch (pMsg->wParam) 
   { 
		case VK_F2: 
			jsDlg->ShowWindow(SW_SHOW);
		   break; 
		case VK_ESCAPE:
			return TRUE;
			break;
    } 
  } 
	//屏蔽右键
	if(context_menu == 0){
		if(WM_RBUTTONDOWN == pMsg->message)
		{
		   // AfxMessageBox(_T("Right Menu!"));
			return TRUE;
		}
	  if(WM_RBUTTONUP == pMsg->message)
		{
		   // AfxMessageBox(_T("Right Menu!"));
			return TRUE;
		}
	  if(WM_RBUTTONDBLCLK == pMsg->message)
		{
		   // AfxMessageBox(_T("Right Menu!"));
			return TRUE;
		}
	}
	//在不是标题栏的地方能拖动
	if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam & MK_LBUTTON)
	{
		if(HIWORD(pMsg->lParam)<title_height){
			SendMessage(WM_NCLBUTTONDOWN, HTCAPTION,  pMsg->lParam);
			return TRUE;
		}
			
	}

  return CDHtmlDialog::PreTranslateMessage(pMsg); 
}
//void CHGuiDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	AfxMessageBox(L"2");
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if(nChar == VK_MENU){
//		AfxMessageBox(L"11");
//	}
//
//	CDHtmlDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}
IHTMLDocument2   *objDocument=NULL;
IHTMLWindow2* pIHTMLWindow = NULL;
void CHGuiDlg::ExeScript(CString strJs, VARIANT* pvarRet)
{
	try{
		if(pIHTMLWindow == NULL){
			this->GetDHtmlDocument(&objDocument);
			objDocument->get_parentWindow(&pIHTMLWindow);
		}
		if(pIHTMLWindow){
			pIHTMLWindow->execScript(CComBSTR(strJs), CComBSTR("JavaScript"), pvarRet);
			ac.Log(L"执行JS："+strJs);
		}
	}catch(...){
		ac.Log(L"执行JS出现异常");
	}
	    
}

void CHGuiDlg::OnNewWindow3(LPDISPATCH* ppDisp,BOOL* Cancel, 
	unsigned long dwFlags, LPCTSTR bstrUrlContext,
	LPCTSTR bstrUrl)
	{
		*Cancel = TRUE;//禁止跳转
		CHGuiDlg* dlg = new CHGuiDlg(this);
		dlg->Create(IDD_HGUI_DIALOG);
		dlg->Navigate(bstrUrl);
		dlg->ShowWindow(SW_SHOW);
	
}

void CHGuiDlg::OnParentNotify(UINT message, LPARAM lParam)
{
	CDHtmlDialog::OnParentNotify(message, lParam);
	if(((HWND)lParam == m_wndBrowser)){
		if (LOWORD(message) == WM_DESTROY)
		{
			this->DestroyWindow();
		}
	}
	

	// TODO: 在此处添加消息处理程序代码
}


void CHGuiDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//
	//ShowWindow(SW_MINIMIZE);
	//ShowWindow(SW_HIDE);
	CDHtmlDialog::OnClose();
}


//void CHGuiDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	//发送WM_NCLBUTTONDOWN消息，使Windows认为鼠标在标题条上
//    PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
//	//CDHtmlDialog::OnLButtonDown(nFlags, point);
//}


//LRESULT CHGuiDlg::OnNcHitTest(CPoint point)
//{
//    CRect rect;
//    GetClientRect(&rect);
//    ScreenToClient(&point);
//    if ((point.x >rect.Width()  -6) && (point.y >rect.Height()  - 6))
//        return HTBOTTOMRIGHT;
//    else if((point.x > rect.Width() -6) && (point.y <= rect.Height() - 6))
//        return HTRIGHT;
//    else if((point.x <= rect.Width() -6) && (point.y > rect.Height() - 6))
//        return HTBOTTOM;
//    else
//        return HTCAPTION;
//}



void CHGuiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);
	if(isNoTitle){
		CRgn m_rgn;
		CRect rc;
		GetWindowRect(&rc);
		rc-=rc.TopLeft();
		m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);
		SetWindowRgn(m_rgn,TRUE);
	}
	// TODO: 在此处添加消息处理程序代码
}
