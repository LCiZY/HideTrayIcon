#pragma once

#include <atlbase.h>
#include <atlconv.h>
#include <CommCtrl.h>
#include <Windows.h>
#include <cstringt.h>
#include <string>
#include <atlstr.h>
#include <vector>




bool contain(char *s, char *p);
int parseInt(char* s);

// 根据进程ID得到进程名称 / get process name by process ID
std::string ProcessIdToName(DWORD processId);
// get error string
std::string GetLastErrorAsString();

// 得到托盘窗口句柄，拓展的托盘窗口句柄 / get the handle of expanded trayIcon window
HWND FindOverflowTrayWindow();
// 得到托盘窗口句柄，任务栏的托盘窗口句柄 / get the handle of taskbar's trayIcon window
HWND FindNormalTrayWindow();

// 设置任务栏中图标的可见状态
// @param hWnd: 图标所在窗口的句柄 / the handle of window where the trayIcon is located
// @param process_name_or_tooltips: 这个vector的元素可以是进程名或托盘提示的子字符串 / the element of the vector could be a subString of process name or subString of trayIcon's tooltip
// @param visible: true是显示，false是隐藏 / true: show, false: hide
// @param isHardDelete: 硬删除，仅在visible为false时生效 / hard delete the trayIcon, only take effect when visible = false
VOID SetTrayIconVisable(HWND hWnd, std::vector<char*>& process_name_or_tooltips, bool visible, bool isHardDelete = false);



