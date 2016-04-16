// JsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HGui.h"
#include "JsDlg.h"
#include "afxdialogex.h"
#include "HGuiDlg.h"

// JsDlg 对话框

IMPLEMENT_DYNAMIC(JsDlg, CDialogEx)

JsDlg::JsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(JsDlg::IDD, pParent)
{
	m_fontabouttest.CreatePointFont(200,L"Courier New");
	m_brushblue.CreateSolidBrush(RGB(61,89,171));
}

JsDlg::~JsDlg()
{
}

void JsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_js);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_file_path);
}


BEGIN_MESSAGE_MAP(JsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JsDlg::OnBnClickedOk)
//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_EXE, &JsDlg::OnBnClickedExe)
	ON_BN_CLICKED(IDC_OPEN_JS, &JsDlg::OnBnClickedOpenJs)
	ON_BN_CLICKED(IDC_LOAD_JS, &JsDlg::OnBnClickedLoadJs)
	ON_BN_CLICKED(IDC_CLEAR, &JsDlg::OnBnClickedClear)
END_MESSAGE_MAP()


// JsDlg 消息处理程序

BOOL JsDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	
	CFont* ptf=m_js.GetFont(); // 得到原来的字体  
  
	LOGFONT lf;   
  
	ptf->GetLogFont(&lf);   
	lf.lfWeight = 150;
	lf.lfHeight = 16; // 改变字体高度   
	WCHAR* str = L"Courier New";
	lstrcpy (lf.lfFaceName,str); // 改变字体名称   
  
	m_fontabouttest.CreateFontIndirect(&lf);   
  
	m_js.SetFont(&m_fontabouttest); // 设置新字体
	m_js.SetFocus();
	return TRUE;
}

void JsDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
	CString sNewString;
	m_js.GetWindowTextW(sNewString);
	sNewString +="\r\n";
	m_js.SetSel(-1, -1);      //自动滚屏
	m_js.SetWindowTextW(sNewString);
	m_js.SetFocus();
}


//HBRUSH JsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//	if(pWnd->GetDlgCtrlID()==IDC_EDIT1)
//	 {
//	  pDC->SetBkMode(TRANSPARENT);//设置为透明
//	  pDC->SetTextColor(RGB(10,35,7));//设置文本颜色
//	  return m_brushblue;//编辑框背景
//	 }
//	return hbr;
//}


void JsDlg::OnBnClickedExe()
{
	CString strJs;
	m_js.GetWindowTextW(strJs);
	VARIANT variant;
	mainDlg->ExeScript(strJs,&variant);
}

BOOL GetFileTextT(const CString fileName,CString* ret){
	CStdioFile file;
	BOOL nOpen = file.Open(fileName,CFile::modeRead);
	if(!nOpen)
		return nOpen;
	CString buf = L"";
	while(file.ReadString(buf)){
		*ret +=buf;
		*ret += "\r\n";
	}
}

BOOL GetFileText(const CString fileName,CString* ret){
	char* tmp = CHGuiDlg::ac.UnicodeToAnsi(fileName);
	FILE* f = fopen(tmp,"r");
	delete[] tmp;
	if(f == NULL){
		return FALSE;
	}
	CString strTmp = L"";
	char buff[256] = "";
	while(fgets(buff,256,f) != NULL){
		strTmp.Format(L"%s\r\n",CHGuiDlg::ac.AnsiToUnicode(buff));
		*ret += strTmp;
		memset(buff,0,256);
	}
}					
void JsDlg::OnBnClickedOpenJs()
{
	CFileDialog dlg(TRUE,0,0,6UL,0,this,0,1);
	int nRet = dlg.DoModal();
	if(nRet = IDOK){
		CString filePath = dlg.GetPathName();
		m_file_path.SetWindowTextW(filePath);
		CString text = L"";
		if(!GetFileText(filePath,&text)){
			AfxMessageBox(L"加载文件失败，请选择正确的文件！");
			return;
		}
		m_js.SetWindowTextW(text);
	}
}


void JsDlg::OnBnClickedLoadJs()
{
	CString filePath;
	m_file_path.GetWindowTextW(filePath);
	if(filePath.GetLength()<1){
		AfxMessageBox(L"请选择要加载的文件！");
		return;
	}
	m_file_path.SetWindowTextW(filePath);
	CString text = L"";
	if(!GetFileText(filePath,&text)){
		AfxMessageBox(L"加载文件失败，请选择正确的文件！");
		return;
	}
	m_js.SetWindowTextW(text);
	m_js.SetFocus();
}


void JsDlg::OnBnClickedClear()
{
	m_js.SetWindowTextW(L"");
	m_js.SetFocus();
}
