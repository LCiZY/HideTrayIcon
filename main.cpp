#include <atlbase.h>
#include <atlconv.h>
#include <CommCtrl.h>
#include <Windows.h>
#include <iostream>
#include <cstringt.h>
#include <string>
#include <atlstr.h>
#include <vector>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

std::vector<char*> tip_parts;

bool contain( char *s, char *p);
int parseInt(char* s);
HWND FindTrayWindow();
HWND FindOverflowTrayWindow();
VOID DeleteTrayIcon(HWND hWnd);


int main(int argc, char* argv[]) {
	std::cout << "参数个数："<<argc;
	if (argc == 1 || argc == 2) return -1;
	//启动延时
	int pre_time = -1;
	pre_time = parseInt(argv[1]); if (pre_time == -1) { std::cout << "参数错误 ";  return -1; } 
	std::cout << "延时 " << pre_time << "秒 "; Sleep(pre_time * 1000); 

	//从启动参数中得到要处理的托盘含有的部分tooltip文字
	for (int i = 2; i < argc; i++) { tip_parts.push_back(argv[i]); }

	DeleteTrayIcon(FindOverflowTrayWindow());
	//system("pause");
	return 0;
}

struct TBBUTTON_1
{
	 int iBitmap;
	 int idCommand;
	 byte fsState;
	 byte fsStyle;
	//      [ MarshalAs( UnmanagedType.ByValArray, SizeConst=2 ) ]
	//      public byte[] bReserved;
	 byte bReserved1;
	 byte bReserved2;
	// public UInt32 dwData;
	 long long dwData;
	 int* iString;
};

//任务栏上的托盘图标
HWND FindTrayWindow()
{
	HWND hWnd = NULL;
	HWND hWndPage = NULL;  // 查找Shell_TrayWnd窗口
	hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if (hWnd != NULL) {    // 查找TrayNotifyWnd窗口
		hWnd = ::FindWindowEx(hWnd, NULL, _T("TrayNotifyWnd"), NULL);
		if (hWnd != NULL) {      // 查找SysPager窗口.
			hWndPage = ::FindWindowEx(hWnd, NULL, _T("SysPager"), NULL);
			if (hWndPage != NULL)        // 查找ToobarWindow32窗口.
				hWnd = ::FindWindowEx(hWndPage, NULL, _T("ToolbarWindow32"), NULL);
			else
				// Win2000没有SysPager窗口，可直接查找ToobarWindow32窗口.
				hWnd = ::FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);
		}
	}
	return hWnd;
}

//点击“显示隐藏的图标”后出现的托盘小窗体里的托盘图标
HWND FindOverflowTrayWindow()
{
	HWND hWnd = NULL;  // 查找NotifyIconOverflowWindow窗口
	hWnd = ::FindWindow(_T("NotifyIconOverflowWindow"), NULL);
	if (hWnd != NULL)    // 查找ToobarWindow32窗口
		hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);

	return hWnd;
}

VOID DeleteTrayIcon(HWND hWnd)
{
	if (hWnd == NULL)
		return;

	struct TRAYDATA
	{
		HWND hWnd;
		UINT uID;
		UINT uCallbackMessage;
		DWORD Reserved1[2];
		HICON hIcon;
		DWORD Reserved2[3];
		TCHAR szExePath[MAX_PATH];
		TCHAR szTip[128];
	};


	DWORD dwProcessID = 0;
	DWORD dwButtonCount = 0;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	TBBUTTON_1 tbButton;
	LPVOID pTB;
	TRAYDATA td;
	NOTIFYICONDATA nid;
	TCHAR szSynTPEnhPath[MAX_PATH] = { 0 };
	TCHAR *pszApplicationName;

	// 查询指定窗口所含图标数，每个图标对应一个按钮
	dwButtonCount = (DWORD)::SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);
	std::cout <<"所含图标数："<< dwButtonCount<<"\n";
	if (dwButtonCount == 0)
		return;

	// 获取窗口所在的线程
	if ((::GetWindowThreadProcessId(hWnd, &dwProcessID) != 0)
		&& (dwProcessID != 0)) {
		hProcess = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
			FALSE, dwProcessID);
		if (hProcess != NULL) {
			pTB = ::VirtualAllocEx(hProcess, NULL, sizeof(TBBUTTON_1), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (pTB != NULL) {
				// 遍历所有图标并匹配目标信息，从而找到目标图标并删除之 
				std::cout << "开始遍历所有图标\n";
				for (DWORD i = 0; i < dwButtonCount; i++) {
					std::cout << "[图标" << i+1 << "]  : ";
					bool flag1 = (SendMessage(hWnd, TB_GETBUTTON, i, (LPARAM)pTB) == TRUE);
					bool flag2 = (::ReadProcessMemory(hProcess, pTB, &tbButton, sizeof(TBBUTTON_1), NULL) != 0);
					std::cout << tbButton.iBitmap << " " << tbButton.idCommand << " " << tbButton.fsState << " " << tbButton.fsStyle << " " << tbButton.bReserved1 << " " << tbButton.dwData << " " << tbButton.iString << " ***";
					bool flag3 = (::ReadProcessMemory(hProcess, LPVOID(tbButton.dwData), &td, sizeof(TRAYDATA), NULL) != 0);

					std::cout <<"ERROR:"<< GetLastError();
					std::cout << "  uid:" << td.uID << "  hwnd:" << td.hWnd << " hIcon:" << td.hIcon;
					if (flag1&&flag2 && flag3) {
						TCHAR szTips[1024];
						::ReadProcessMemory(hProcess, (LPVOID)tbButton.iString, szTips, 1024, NULL);
						//szTips是Unicode字符串，需要转换
						USES_CONVERSION;
						CString csTips = W2A((WCHAR*)(szTips));
						std::string tip = csTips.GetBuffer(0);
						std::cout <<" tip:"<< tip<<" "; 
						bool existFlag = false;
						for (int i = 0; i < tip_parts.size(); i++)
						{
							if (contain(const_cast<char*>(tip.c_str()), tip_parts.at(i))) { existFlag = true; break; }
						}
						// 如果托盘图标的szTip包含特定的信息，该图标就是我们准备清除的图标，找到并删除它
						std::cout << " ifExist:" << existFlag<<" ";
						if(existFlag)
						{
							nid.cbSize = NOTIFYICONDATA_V2_SIZE;
							nid.uID = td.uID;
							nid.hWnd = td.hWnd;
							nid.hIcon = td.hIcon;
							nid.uCallbackMessage = td.uCallbackMessage;
							nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
							::Shell_NotifyIcon(NIM_DELETE, &nid);
							::SendMessage(hWnd, TB_HIDEBUTTON, tbButton.idCommand, MAKELONG(true, 0));
							std::cout << "隐藏托盘图标：执行成功";
						}
					}
					std::cout <<" flags:"<< flag1 << " " << flag2 << " " << flag3;

					std::cout << std::endl;
				}
				::VirtualFreeEx(hProcess, pTB, sizeof(TBBUTTON_1), MEM_FREE);
			}
			::CloseHandle(hProcess);
		}
	}
}


bool contain(char *s, char *p) {
	int len1 = strlen(s);
	int len2 = strlen(p);
	if (len1 < len2) return false;

	int index1 = 0;
	int index2 = 0;

	while (index1!=len1 && index2!=len2)
	{
		if (s[index1] == p[index2]) {
			index1++; index2++;
			if (index2 == len2) return true;
		}
		else{
			index1++;
			index2 = 0;
		}
	}
	return false;
}


int parseInt(char* s) {

	std::cout << " int：" << s;

	int len = strlen(s);
	int result = 0;
	for (int i = 0; i < len; i++)
	{
		if(s[i]>=48&&s[i]<=57) result += ( (s[i]-48 ) * pow(10, len - i - 1));
		else return -1;
	}
	return result;

}