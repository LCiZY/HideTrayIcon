#include"tray_icon.h"

struct TBBUTTON64
{
	int iBitmap;
	int idCommand;
	byte fsState;
	byte fsStyle;
	byte bReserved1;
	byte bReserved2;
	long long dwData;
	int* iString;
};

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

HWND FindOverflowTrayWindow() // 图标收纳区的窗口句柄
{
	HWND hWnd = ::FindWindow(_T("NotifyIconOverflowWindow"), NULL);  // 查找NotifyIconOverflowWindow窗口
	if (hWnd != NULL)    // 查找ToobarWindow32窗口
		hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);

	return hWnd;
}

HWND FindNormalTrayWindow()  //任务栏的窗口句柄
{
	// find system tray window
	HWND trayWnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (trayWnd) {
		trayWnd = FindWindowEx(trayWnd, NULL, _T("TrayNotifyWnd"), NULL);
		if (trayWnd) {
			trayWnd = FindWindowEx(trayWnd, NULL, _T("SysPager"), NULL);
			if (trayWnd) {
				trayWnd = FindWindowEx(trayWnd, NULL, _T("ToolbarWindow32"), NULL);
			}
		}
	}
	return trayWnd;
}

VOID SetTrayIconVisable(HWND hWnd, std::vector<std::string>& process_name_or_tooltips, bool visible, bool isHardDelete)
{
	printf("\n\n====================\n");
	printf("start to %s TrayIcon.\n", visible ? "show" : "hide");
	printf("hWnd: %x, visible: %i, isHardDelete: %i, length of process_name_or_tooltips: %d \n", hWnd, visible, isHardDelete, process_name_or_tooltips.size());

	if (hWnd == NULL)
		return;

	DWORD dwProcessID = 0;
	DWORD dwButtonCount = 0;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	TBBUTTON64 tbButton;
	LPVOID pTB;
	TRAYDATA td;
	NOTIFYICONDATA nid;
	TCHAR szSynTPEnhPath[MAX_PATH] = { 0 };


	// 查询指定窗口所含图标数，每个图标对应一个按钮 // get the number of trayIcon in the window specified by hWnd
	dwButtonCount = (DWORD)::SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);
	printf("handle: %x, num_of_tray_icon: %d\n", hWnd, dwButtonCount);
	if (dwButtonCount == 0)
		return;

	// 获取窗口所在的进程ID // get the processID to which the hWnd belongs
	if ((::GetWindowThreadProcessId(hWnd, &dwProcessID) != 0)
		&& (dwProcessID != 0)) {
		hProcess = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
			FALSE, dwProcessID);
		if (hProcess != NULL) {
			pTB = ::VirtualAllocEx(hProcess, NULL, sizeof(TBBUTTON64), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (pTB != NULL) {
				// 遍历所有图标并匹配目标信息，从而找到目标图标并发送 显示/隐藏 消息  // traverse all trayIcons to find the target and send msg to show/hide it
				printf("开始遍历所有图标(Begin traversal)\n");
				for (DWORD i = 0; i < dwButtonCount; i++) {

					printf("\n--------------------\n ");
					printf("[Icon_%d]\n", i + 1);

					bool flag1 = (SendMessage(hWnd, TB_GETBUTTON, i, (LPARAM)pTB) == TRUE);
					bool flag2 = (::ReadProcessMemory(hProcess, pTB, &tbButton, sizeof(TBBUTTON64), NULL) != 0);
					bool flag3 = (::ReadProcessMemory(hProcess, LPVOID(tbButton.dwData), &td, sizeof(TRAYDATA), NULL) != 0);

					printf("iBitmap:%d \t idCommand:%d \t fsState:%d \t fsStyle:%d \t bReserved1:%d \t dwData:%I64d \t iString:%d \t ",
						tbButton.iBitmap, tbButton.idCommand, tbButton.fsState, tbButton.fsStyle, tbButton.bReserved1, tbButton.dwData, tbButton.iString);
					printf("uid:%d \t hwnd:%x \t hIcon:%x \t flag1:%i, flag2:%i, flag3:%i", td.uID, td.hWnd, td.hIcon, flag1, flag2, flag3);
					if (GetLastError() != 0) { printf("\nLastError: %s", GetLastErrorAsString().c_str()); }
					else { printf("\n"); }

					if (flag1 && flag2 && flag3) {

						TCHAR szTips[1024];
						::ReadProcessMemory(hProcess, (LPVOID)tbButton.iString, szTips, 1024, NULL);
						//szTips是Unicode字符串，需要转换 // szTips is compose of unicode, convert to CString
						USES_CONVERSION;
						CString csTips = W2A((WCHAR*)(szTips));
						std::string tip = csTips.GetBuffer(0);
						printf("tooltipText:[%s]", tip.c_str());

						DWORD trayIconProcessId = 0;
						GetWindowThreadProcessId(td.hWnd, &trayIconProcessId);
						std::string pro_name = ProcessIdToName(trayIconProcessId);
						printf("\nprocessName:[%s]", pro_name.c_str());

						bool existFlag = false;
						for (int i = 0; i < process_name_or_tooltips.size(); i++) {
							if (contain(tip.c_str(), process_name_or_tooltips.at(i).c_str()) ||
								contain(pro_name.c_str(), process_name_or_tooltips.at(i).c_str())) {
								existFlag = true;
								break;
							}
						}

						// 如果 托盘图标的szTip 或 托盘图标所属进程名 包含特定的信息，该图标就是我们准备清除的图标，找到并 删除/隐藏 它 
						// if there is user specified msg contained in the tooltip of trayIcon and process path, then the trayIcon is a target
						printf("\nis target: %s", existFlag ? "true" : "false");
						if (existFlag) {
							bool executeResult;
							if (isHardDelete && !visible) { //硬删除，不可恢复 // hard delete action, can not recover it
								// for taskbar's icon
								nid.cbSize = NOTIFYICONDATA_V2_SIZE;
								nid.uID = td.uID;
								nid.hWnd = td.hWnd;
								nid.hIcon = td.hIcon;
								nid.uCallbackMessage = td.uCallbackMessage;
								nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
								executeResult = ::Shell_NotifyIcon(NIM_DELETE, &nid);
								// for overflow area
								::SendMessage(hWnd, TB_DELETEBUTTON, i, 0);
							} else {
								executeResult = ::SendMessage(hWnd, TB_HIDEBUTTON, tbButton.idCommand, MAKELONG(!visible, 0)); // MAKELONG(true, 0) is hide, otherwise is show
							}
							// send msg to hWnd
							::SendMessage(hWnd, TB_AUTOSIZE, 0, 0);
							printf(",\t%s trayIcon: %s", visible ? "show" : "hide", executeResult ? "succeed" : "failed");
						}
					} else {
						printf("\n Read trayIcon info failed!!! \n");
					}
					printf("\n");
				}
				::VirtualFreeEx(hProcess, pTB, sizeof(TBBUTTON64), MEM_FREE);
			}
			else {
				printf("\n VirtualAllocEx allocate memory to TBBUTTON64 failed, processID: %d, process: %d %x(hex) \n", dwProcessID, hProcess, hProcess);
			}
			::CloseHandle(hProcess);
		}
		else {
			printf("\n OpenProcess failed, processID: %d \n", dwProcessID);
		}
	}
	else {
		printf("\n GetWindowThreadProcessId failed, hWnd: %x \n", hWnd);
	}
}


std::string ProcessIdToName(DWORD processId)
{
	std::string ret;
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_LIMITED_INFORMATION,
		FALSE,
		processId /* This is the PID, you can find one from windows task manager */
	);
	if (handle)
	{
		DWORD buffSize = 1024;
		CHAR buffer[1024];
		if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
		{
			ret = buffer;
		}
		else
		{
			printf("Error GetModuleBaseNameA : %lu", GetLastError());
		}
		CloseHandle(handle);
	}
	else
	{
		printf("Error OpenProcess : %lu", GetLastError());
	}
	return ret;
}

std::string GetLastErrorAsString()
{
	//Get the error message ID, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message;
}
