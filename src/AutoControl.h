// AutoControl.h: interface for the AutoControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOCONTROL_H__A5814C11_E9DB_49F8_A791_5138561FA541__INCLUDED_)
#define AFX_AUTOCONTROL_H__A5814C11_E9DB_49F8_A791_5138561FA541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlconv.h>
class AutoControl  
{
public:
	char szLogPath[255];
	BOOL bIsOpenLog;
	HWND GetWindow(LPCTSTR strClassName,LPCTSTR strTitle,int nSecond);
	AutoControl();
	virtual ~AutoControl();
	//�����ı�
	void SendKeys(CString msg);
	//�õ���ǰʱ��
	void GetTime(CString* time);
	//��¼��־
	void Log(CString msg);
	void Log(const TCHAR* msg);
	//�򿪲˵�
	int OpenMenu(HWND hwnd,int subMenu,int item);
	//������
	void ClickMouse(int x,int y);
	//�����ť
	void ClickButton(HWND hwnd);

	void PressKey(int vk);
	//����ϼ�
	void PressShortKey(int* vk,int count);
	
	//�ô�������
	void MakeWinTop(HWND hwnd);

	//�� ���ֽ�char* ת��Ϊ ���ֽ� wchar*
inline wchar_t* AnsiToUnicode( const char* szStr )
{
int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
if (nLen == 0)
{
   return NULL;
}
wchar_t* pResult = new wchar_t[nLen];
MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
return pResult;
}


// �� ���ֽ�wchar_t* ת�� ���ֽ�char*
inline char* UnicodeToAnsi( const wchar_t* szStr )
{
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
	   return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
	return pResult;
}


private:
	void SendAscii(wchar_t data, BOOL shift);
	void SendUnicode(wchar_t data);
};

#endif // !defined(AFX_AUTOCONTROL_H__A5814C11_E9DB_49F8_A791_5138561FA541__INCLUDED_)
