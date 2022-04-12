#pragma once

#include <atlbase.h>
#include <atlconv.h>
#include <CommCtrl.h>
#include <Windows.h>
#include <iostream>
#include <cstringt.h>
#include <string>
#include <atlstr.h>
#include <vector>




bool contain(char *s, char *p);
int parseInt(char* s);


// 得到托盘窗口句柄
HWND FindOverflowTrayWindow();
HWND FindNormalTrayWindow();
// 设置任务栏中图标的可见状态
VOID SetTrayIconVisable(HWND hWnd, std::vector<char*>& tip_parts, bool visable, bool isHardDelete = false);



