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


// �õ����̴��ھ��
HWND FindOverflowTrayWindow();
HWND FindNormalTrayWindow();
// ������������ͼ��Ŀɼ�״̬
VOID SetTrayIconVisable(HWND hWnd, std::vector<char*>& tip_parts, bool visable, bool isHardDelete = false);



