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
	//输入文本
	void SendKeys(CString msg);
	//得到当前时间
	void GetTime(CString* time);
	//纪录日志
	void Log(CString msg);
	void Log(const TCHAR* msg);
	//打开菜单
	int OpenMenu(HWND hwnd,int subMenu,int item);
	//点击鼠标
	void ClickMouse(int x,int y);
	//点击按钮
	void ClickButton(HWND hwnd);

	void PressKey(int vk);
	//按组合键
	void PressShortKey(int* vk,int count);
	
	//让窗口至顶
	void MakeWinTop(HWND hwnd);

	//将 单字节char* 转换为 宽字节 wchar*
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


// 将 宽字节wchar_t* 转换 单字节char*
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
