
// HGuiDlg.h : 头文件
//

#pragma once
#include "json\json.h"
#include "AutoControl.h"
#include "JsDlg.h"
#include <map>
using namespace std;

typedef struct FunInfo_{
	TCHAR* strFunModule;
	TCHAR* strFunName;
	TCHAR* strFunParam;
	TCHAR* strCallback;
	~FunInfo_(){
		delete[] strFunModule; 
		delete[] strFunName;
		delete[] strFunParam;
		delete[] strCallback;
	};
} FunInfo;
typedef BSTR (*IFun)(const BSTR param,HWND hwnd);
#define WM_CALLJS_MESSAGE (WM_USER+100)

// CHGuiDlg 对话框
class CHGuiDlg : public CDHtmlDialog
{
// 构造
public:
	CHGuiDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HGUI_DIALOG, IDH = 0 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement *pElement);
	//HRESULT OnButtonCancel(IHTMLElement *pElement);

	//处理函数
	void Control(BSTR param);
	BSTR Run(FunInfo* funInfo);
	BOOL ParseFunInfo(FunInfo* funInfo,BSTR param);

	//调用js方法
	BOOL CallJScript(const CString strFunc, _variant_t* pVarResult);
	BOOL CallJScript(const CString strFunc,const CString strArg1,_variant_t* pVarResult);
	BOOL CallJScript(const CString strFunc, const CStringArray& paramArray,_variant_t* pVarResult);
	BOOL GetJScript(CComPtr<IDispatch>& spDisp);

	static BOOL bRun;

// 实现
protected:
	HICON m_hIcon;
	BOOL PreTranslateMessage(MSG* pMsg);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL CanAccessExternal();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnCallJScript(WPARAM, LPARAM); 
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_EVENTSINK_MAP()
	void OnNewWindow3(LPDISPATCH* ppDisp,BOOL* Cancel, unsigned long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl);

private:
	TCHAR szCfg[255];
	TCHAR szCurrDir[255];
	TCHAR szLogPath[255];
	//TCHAR szIndex[255];
	TCHAR szInitPage[255];
	CMap<CString, LPCTSTR, CString, LPCTSTR> urlJsMap;

	JsDlg* jsDlg;
	int width;
	int height;
	int title_height;
	int context_menu;
	BOOL isNoTitle;
	CString szTitle;

public:
	static AutoControl ac;
	virtual BOOL IsInvokeAllowed(DISPID /*dispid*/);
	void AsyncRun(BSTR params);
	void SetUrlJsMap(BSTR json);
	static UINT RunProc(LPVOID lParams);
	static UINT ProcSafe(LPVOID lParams);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	void SetNoTitle(BSTR params);
	void Close(BSTR params);
	void ChangeMaxMin(BSTR params);

	static map<string,HMODULE> dllMap;
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ExeScript(CString strJs, VARIANT* pvarRet);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnClose();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg LRESULT OnNcHitTest(CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
