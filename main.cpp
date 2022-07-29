#include "util.h"


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )   // 隐藏控制台界面 / hide console 


std::vector<char*> tip_parts;


// argv: 1:延时delay  2:显示隐藏visible:hide/show  3:tooltip list...
int main(int argc, char* argv[]) {
	printf("来自命令行的参数的个数: %d", argc);
	if (argc < 4) return -1;
	//1.解析启动延时 / parse launch delay
	int pre_time = parseInt(argv[1]); if (pre_time == -1) { printf("参数错误");  return -2; }
	printf("\t延时: %d 秒启动", pre_time);
	Sleep(pre_time * 1000);
	
	//2.解析此次操作是隐藏还是显示图标 / whether the operation is show or hide
	bool visible = strcmp(argv[2], "show") == 0 ; // show or hide
	//3.从启动参数中得到要处理的托盘含有的部分tooltip文字 / tooltips that split by space(could be not Integrity)
	for (int i = 3; i < argc; i++) { tip_parts.push_back(argv[i]); }
	
	SetTrayIconVisable(FindOverflowTrayWindow(), tip_parts, visible);
	SetTrayIconVisable(FindNormalTrayWindow(),   tip_parts, visible, true);

	return 0;
}


///////// ---for test---debug in vs---
//int main() {
//	bool visible = false;
//
//	 char c[] = "ShareOnLan";
//	 char c1[] = "Quick";
//	 char c2[] = "Ditto";
//	 char c3[] = "uTools.exe";
//	tip_parts.push_back(c);
//	tip_parts.push_back(c1);
//	tip_parts.push_back(c2);
//	tip_parts.push_back(c3);
//	SetTrayIconVisable(FindOverflowTrayWindow(), tip_parts, visible);
//	SetTrayIconVisable(FindNormalTrayWindow(), tip_parts, visible, false);
//	fflush(stdout);
//	system("pause");
//}
