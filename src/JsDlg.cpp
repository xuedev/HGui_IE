// JsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HGui.h"
#include "JsDlg.h"
#include "afxdialogex.h"
#include "HGuiDlg.h"

// JsDlg �Ի���

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


// JsDlg ��Ϣ�������

BOOL JsDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	
	CFont* ptf=m_js.GetFont(); // �õ�ԭ��������  
  
	LOGFONT lf;   
  
	ptf->GetLogFont(&lf);   
	lf.lfWeight = 150;
	lf.lfHeight = 16; // �ı�����߶�   
	WCHAR* str = L"Courier New";
	lstrcpy (lf.lfFaceName,str); // �ı���������   
  
	m_fontabouttest.CreateFontIndirect(&lf);   
  
	m_js.SetFont(&m_fontabouttest); // ����������
	m_js.SetFocus();
	return TRUE;
}

void JsDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
	CString sNewString;
	m_js.GetWindowTextW(sNewString);
	sNewString +="\r\n";
	m_js.SetSel(-1, -1);      //�Զ�����
	m_js.SetWindowTextW(sNewString);
	m_js.SetFocus();
}


//HBRUSH JsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//	if(pWnd->GetDlgCtrlID()==IDC_EDIT1)
//	 {
//	  pDC->SetBkMode(TRANSPARENT);//����Ϊ͸��
//	  pDC->SetTextColor(RGB(10,35,7));//�����ı���ɫ
//	  return m_brushblue;//�༭�򱳾�
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
			AfxMessageBox(L"�����ļ�ʧ�ܣ���ѡ����ȷ���ļ���");
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
		AfxMessageBox(L"��ѡ��Ҫ���ص��ļ���");
		return;
	}
	m_file_path.SetWindowTextW(filePath);
	CString text = L"";
	if(!GetFileText(filePath,&text)){
		AfxMessageBox(L"�����ļ�ʧ�ܣ���ѡ����ȷ���ļ���");
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
