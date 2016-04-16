// AutoControl.cpp: implementation of the AutoControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AutoControl::AutoControl()
{
	memset(szLogPath,0,255);
	bIsOpenLog = FALSE;
}

AutoControl::~AutoControl()
{	

}
HWND preWnd;
//得到窗口句柄
HWND AutoControl::GetWindow(LPCTSTR strClassName, LPCTSTR strTitle,int nMillisecond)
{
	//CString strLog = "查找窗口，窗口类："+CString(strClassName)+"；窗口标题："+strTitle;
	//Log(strLog);
	//获取客户端句柄
	HWND hwnd = NULL;
	int time = 0;
	if(nMillisecond<10){
		nMillisecond = 10;
	}
	int nCount = nMillisecond/10;
	while(1){
		hwnd = ::FindWindow(strClassName,strTitle);
		if(NULL != hwnd){
			if(hwnd != preWnd){
				char text[255] = "";
				CString strFound;
				::GetWindowTextA(hwnd,text,255);
				strFound.Format(L"找到窗口：%0x,%s",hwnd,text);
				Log(strFound);
				preWnd = hwnd;
			}
			
			return hwnd;
		}

		time++;
		if(time>nCount){
			//Log("NULL");
			return NULL;
		}

		Sleep(10);
	}

}


void AutoControl::SendAscii(wchar_t data, BOOL shift)
{
  INPUT input[2];
  memset(input, 0, 2 * sizeof(INPUT));
 
  if (shift)
  {
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = VK_SHIFT;
    SendInput(1, input, sizeof(INPUT));
  }
  input[0].type = INPUT_KEYBOARD;
  input[0].ki.wVk = data;
  input[1].type = INPUT_KEYBOARD;
  input[1].ki.wVk = data;
  input[1].ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(2, input, sizeof(INPUT));
  if (shift)
  {
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = VK_SHIFT;
    input[0].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, input, sizeof(INPUT));  
  }
}

void AutoControl::SendUnicode(wchar_t data)
{
  INPUT input[2];
  memset(input, 0, 2 * sizeof(INPUT));
 
  input[0].type = INPUT_KEYBOARD;
  input[0].ki.wVk = 0;
  input[0].ki.wScan = data;
  input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;
 
  input[1].type = INPUT_KEYBOARD;
  input[1].ki.wVk = 0;
  input[1].ki.wScan = data;
  input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;
 
  SendInput(2, input, sizeof(INPUT));
}

//为方便使用，下面这个函数包装了前两个函数。

void AutoControl::SendKeys(CString msg)
{
  CString strLog = L"发送文本："+CString(msg);
  Log(strLog);
  short vk;
  BOOL shift;
  USES_CONVERSION;
  wchar_t* data = T2W(msg.GetBuffer(0));
  int len = wcslen(data);
  for(int i=0;i<len;i++)
  {
    if (data[i]>=0 && data[i]<256) //ascii字符
    {
      vk = VkKeyScanW(data[i]);
      if (vk == -1)
      {
        SendUnicode(data[i]);
      }
      else
      {
        if (vk < 0)
        {
          vk = ~vk + 0x1;
        }
       
        shift = vk >> 8 & 0x1;
       
        if (GetKeyState(VK_CAPITAL) & 0x1)
        {
          if (data[i]>='a' && data[i]<='z' || data[i]>='A' && data[i]<='Z')
          {
            shift = !shift;
          }
        }
        SendAscii(vk & 0xFF, shift);
      }
    }
    else //unicode字符
    {
      SendUnicode(data[i]);
    }
  }
}

void AutoControl::GetTime(CString* time){
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	time->Format(L"%4d-%02d-%02d %02d:%02d:%02d",
				sys.wYear,
				sys.wMonth,
				sys.wDay,
				sys.wHour,
				sys.wMinute,
				sys.wSecond);
}

void AutoControl::Log(CString msg){
	Log(msg.GetBuffer(msg.GetLength()));
}

void AutoControl::Log(const TCHAR* msg){
	//::MessageBoxA(NULL,"","1",0);
	if(bIsOpenLog){
		char log[255] = "";
		//::MessageBoxA(NULL,"","2",0);
		if(strlen(szLogPath)<1)
			strcat(log,"D:\\HGui.log");
		else{
			strcat(log,szLogPath);
			strcat(log,"\\HGui.log");
		}
		//::MessageBoxA(NULL,"","3",0);
		FILE* file = fopen(log,"a");
		CString time;
		GetTime(&time);
		CString str;
		str.Format(L"%s %s\n",(LPCTSTR)time,msg);
		char* tmp = UnicodeToAnsi(str.GetBuffer());
		fprintf(file,"%s",tmp);
		delete[] tmp;
		fclose(file);
	}
}
//打开菜单
int AutoControl::OpenMenu(HWND hwnd,int subMenu,int item){
	
	//查找菜单
	HMENU mainMenu = ::GetMenu(hwnd);
	HMENU menuFile = ::GetSubMenu(mainMenu,subMenu);
	int nFpgl = ::GetMenuItemID(menuFile,item);
	::PostMessage(hwnd,WM_COMMAND,nFpgl,nFpgl);
	//测试
	//Sleep(2000);
	return 1;
}

//click mouse
void AutoControl::ClickMouse(int x,int y){
	CString msg = L"";
	msg.Format(L"点击鼠标，位置：%d,%d",x,y);
	Log(msg);
	::SetCursorPos(x,y);
	//测试，暂时屏蔽
	//::mouse_event(MOUSEEVENTF_LEFTDOWN,x,y,0,::GetMessageExtraInfo());
	//::mouse_event(MOUSEEVENTF_LEFTUP,x,y,0,::GetMessageExtraInfo());
	::mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

}

// click button 
void AutoControl::ClickButton(HWND hwnd){
	// UINT nCtrlID = ::GetDlgCtrlID(hwnd);  
	//::PostMessage(hwnd, WM_COMMAND, (WPARAM)(BN_CLICKED << 16 | nCtrlID), (LPARAM)hwnd);  
	::PostMessage(hwnd,WM_MOUSEMOVE, MK_LBUTTON, MAKELONG(0,0) );     
	::PostMessage(hwnd,WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(0,0));        
	::PostMessage(hwnd,WM_LBUTTONUP,MK_LBUTTON,MAKELPARAM(0,0)); 
	/*
	::BringWindowToTop(hwnd);
	RECT rect;
	::GetWindowRect(hwnd,&rect);
	::ClickMouse((rect.right - rect.left)/2,(rect.bottom - rect.top)/2);
	*/
	CString log;
	log.Format(L"%0x",hwnd);
	TCHAR text[255] = L"";
	::GetWindowText(hwnd,text,255);
	Log(L"点击按钮："+log+L" ,"+text);
}

//按键
void AutoControl::PressKey(int vk){
	keybd_event(vk,0,0,0);
	keybd_event(vk,0,KEYEVENTF_KEYUP,0);

	CString log;
	log.Format(L"%d",vk);
	Log(L"按键："+log);
}

//按组合键
void AutoControl::PressShortKey(int* vk,int count){
	for(int i=0;i<count;i++){
		keybd_event(vk[i],0,0,0);
	}
	
	for(int j=0;j<count;j++){
		keybd_event(vk[j],0,KEYEVENTF_KEYUP,0);
	}
}

//让窗口至顶
void AutoControl::MakeWinTop(HWND hwnd){
	::ShowWindow(hwnd,SW_SHOWNORMAL);
	::SetForegroundWindow(hwnd);
}