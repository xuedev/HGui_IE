#pragma once


// JsDlg �Ի���
class CHGuiDlg;
class JsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(JsDlg)

public:
	JsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~JsDlg();
	CHGuiDlg* mainDlg;
// �Ի�������
	enum { IDD = IDD_DIALOG1 };

	CFont m_fontabouttest;
	CBrush m_brushblue;//������Ϣ����ɫ��ˢ


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
