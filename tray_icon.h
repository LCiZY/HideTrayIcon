#pragma once

#include <atlbase.h>
#include <atlconv.h>
#include <CommCtrl.h>
#include <Windows.h>
#include <cstringt.h>
#include <string>
#include <atlstr.h>
#include <vector>

#include "util.h"

// ���ݽ���ID�õ��������� / get process name by process ID
std::string ProcessIdToName(DWORD processId);
// get error string
std::string GetLastErrorAsString();

// �õ����̴��ھ������չ�����̴��ھ�� / get the handle of expanded trayIcon window
HWND FindOverflowTrayWindow();
// �õ����̴��ھ���������������̴��ھ�� / get the handle of taskbar's trayIcon window
HWND FindNormalTrayWindow();

// ������������ͼ��Ŀɼ�״̬ / set the visibility of trayIcons in taskBar and overflow area
// @param hWnd: ͼ�����ڴ��ڵľ�� / the handle of window where the trayIcon is located
// @param process_name_or_tooltips: ���vector��Ԫ�ؿ����ǽ�������������ʾ�����ַ��� / the element of the vector could be a subString of process name or subString of trayIcon's tooltip
// @param visible: true����ʾ��false������ / true: show, false: hide
// @param isHardDelete: Ӳɾ��������visibleΪfalseʱ��Ч / hard delete the trayIcon, only take effect when visible = false
VOID SetTrayIconVisable(HWND hWnd, std::vector<std::string>& process_name_or_tooltips, bool visible, bool isHardDelete = true);
