#include "util.h"


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )   // 隐藏控制台界面


std::vector<char*> tip_parts;


/// argv: 1:延时delay  2:显示隐藏visable:hide/show  3:tooltip list...
int main(int argc, char* argv[]) {
	std::cout << "参数个数:"<<argc;
	if (argc < 4) return -1;
	//1.解析启动延时 / parse launch delay
	int pre_time = parseInt(argv[1]); if (pre_time == -1) { std::cout << "参数错误 ";  return -1; }
	std::cout << "\t延时:" << pre_time << "秒 "; Sleep(pre_time * 1000); 
	//2.解析此次操作是隐藏还是显示图标 / whether the operation is show or hide
	bool visable = strcmp(argv[2], "show") == 0 ; // show or hide
	//3.从启动参数中得到要处理的托盘含有的部分tooltip文字 / tooltips that split by space(could be not Integrity)
	for (int i = 2; i < argc; i++) { tip_parts.push_back(argv[i]); }
	
	SetTrayIconVisable(FindOverflowTrayWindow(), tip_parts, visable);
	return 0;
}


///// for test
//int main() {
//	 char c[] = "ShareOnLan";
//	 char c1[] = "Quick";
//	 char c2[] = "Ditto";
//	tip_parts.push_back(c);
//	tip_parts.push_back(c1);
//	tip_parts.push_back(c2);
//	SetTrayIconVisable(FindOverflowTrayWindow(), tip_parts, true);
//	fflush(stdout);
//	system("pause");
//}
