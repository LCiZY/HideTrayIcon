#include"util.h"

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

HWND FindOverflowTrayWindow() // ͼ���������Ĵ��ھ��
{
	HWND hWnd = ::FindWindow(_T("NotifyIconOverflowWindow"), NULL);  // ����NotifyIconOverflowWindow����
	if (hWnd != NULL)    // ����ToobarWindow32����
		hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);

	return hWnd;
}

HWND FindNormalTrayWindow()  //�������Ĵ��ھ��
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

VOID SetTrayIconVisable(HWND hWnd, std::vector<char*>& tip_parts ,bool visible, bool isHardDelete)
{
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
	TCHAR *pszApplicationName;

	// ��ѯָ����������ͼ������ÿ��ͼ���Ӧһ����ť
	dwButtonCount = (DWORD)::SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);
	printf("\n==================== \nSetTrayIconVisable Start.\n�����%x ����ͼ������%d,", hWnd, dwButtonCount);
	if (dwButtonCount == 0)
		return;

	// ��ȡ�������ڵ��߳�
	if ((::GetWindowThreadProcessId(hWnd, &dwProcessID) != 0)
		&& (dwProcessID != 0)) {
		hProcess = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
			FALSE, dwProcessID);
		if (hProcess != NULL) {
			pTB = ::VirtualAllocEx(hProcess, NULL, sizeof(TBBUTTON64), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (pTB != NULL) {
				// ��������ͼ�겢ƥ��Ŀ����Ϣ���Ӷ��ҵ�Ŀ��ͼ�겢ɾ��֮ 
				std::cout << "��ʼ��������ͼ��\n";
				for (DWORD i = 0; i < dwButtonCount; i++) {
					printf("\n====================\n[ͼ��%d]: ", i + 1);
					bool flag1 = (SendMessage(hWnd, TB_GETBUTTON, i, (LPARAM)pTB) == TRUE);
					bool flag2 = (::ReadProcessMemory(hProcess, pTB, &tbButton, sizeof(TBBUTTON64), NULL) != 0);
					printf("iBitmap:%d\tidCommand:%d\tfsState:%d\tfsStyle:%d\tbReserved1:%d\tdwData:%I64d\tiString:%d\s",
						tbButton.iBitmap , tbButton.idCommand , tbButton.fsState , tbButton.fsStyle , tbButton.bReserved1 , tbButton.dwData , tbButton.iString );
					bool flag3 = (::ReadProcessMemory(hProcess, LPVOID(tbButton.dwData), &td, sizeof(TRAYDATA), NULL) != 0);
					if (GetLastError() != 0)  printf("\tERROR��%d", GetLastError());
					printf("\tuid:%d\thwnd:%x\thIcon:%x\tflag1:%i,flag2:%i,flag3:%i", td.uID, td.hWnd, td.hIcon, flag1, flag2, flag3);
					if (flag1 && flag2 && flag3) {

						TCHAR szTips[1024];
						::ReadProcessMemory(hProcess, (LPVOID)tbButton.iString, szTips, 1024, NULL);
						//szTips��Unicode�ַ�������Ҫת��
						USES_CONVERSION;
						CString csTips = W2A((WCHAR*)(szTips));
						std::string tip = csTips.GetBuffer(0);
						printf("\ttooltip_text:[%s]", tip.c_str());

						DWORD trayIconProcessId = 0;
						GetWindowThreadProcessId(td.hWnd, &trayIconProcessId);
						std::string pro_name = ProcessIdToName(trayIconProcessId);
						printf("\tprocess_name:[%s]", pro_name.c_str());

						bool existFlag = false;
						for (int i = 0; i < tip_parts.size(); i++){
							if (contain(const_cast<char*>(tip.c_str()), tip_parts.at(i)) || 
								contain(const_cast<char*>(pro_name.c_str()), tip_parts.at(i))) {
								existFlag = true; 
								break; 
							}
						}

						// ��� ����ͼ���szTip �� ����ͼ������������ �����ض�����Ϣ����ͼ���������׼�������ͼ�꣬�ҵ��� ɾ��/���� ��
						std::cout << "\tis target:" << existFlag ;
						if (existFlag){
							if (isHardDelete && !visible) { //Ӳɾ�������ɻָ�
								nid.cbSize = NOTIFYICONDATA_V2_SIZE;
								nid.uID = td.uID;
								nid.hWnd = td.hWnd;
								nid.hIcon = td.hIcon;
								nid.uCallbackMessage = td.uCallbackMessage;
								nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
								::Shell_NotifyIcon(NIM_DELETE, &nid);
							}
							::SendMessage(hWnd, TB_HIDEBUTTON, tbButton.idCommand, MAKELONG(!visible, 0)); // MAKELONG(true, 0) is hide, otherwise is show
							::SendMessage(hWnd, TB_AUTOSIZE, 0, 0);
							printf("\t%s����ͼ�꣺ִ�гɹ�", visible?"��ʾ":"����");
						}
					}
					printf("\n");
				}
				::VirtualFreeEx(hProcess, pTB, sizeof(TBBUTTON64), MEM_FREE);
			}
			::CloseHandle(hProcess);
		}
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



bool contain(char *s, char *p) {
	int len1 = strlen(s);
	int len2 = strlen(p);
	if (len1 < len2) return false;

	int index1 = 0;
	int index2 = 0;

	while (index1 != len1 && index2 != len2)
	{
		if (s[index1] == p[index2]) {
			index1++; index2++;
			if (index2 == len2) return true;
		}
		else {
			index1++;
			index2 = 0;
		}
	}
	return false;
}


int parseInt(char* s) {

	int len = strlen(s);
	int result = 0;
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= 48 && s[i] <= 57) result += ((s[i] - 48) * pow(10, len - i - 1));
		else return -1;
	}
	return result;

}