#pragma once


// JsDlg 对话框
class CHGuiDlg;
class JsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(JsDlg)

public:
	JsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~JsDlg();
	CHGuiDlg* mainDlg;
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

	CFont m_fontabouttest;
	CBrush m_brushblue;//误码信息背景色画刷


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CEdit m_js;
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedExe();
	CEdit m_file_path;
	afx_msg void OnBnClickedOpenJs();
	afx_msg void OnBnClickedLoadJs();
	afx_msg void OnBnClickedClear();
};
