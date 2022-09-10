#include "tray_icon.h"
#include "consts.h"
#include "cmdline.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )   // 隐藏控制台界面 / hide console 


using namespace std;

int main(int argc, char* argv[]) {

	///// cmdline https://github.com/tanakh/cmdline
	cmdline::parser psr;
	psr.add<string>("icon", 'i', "list of trayIcons, separated by space", true, "");
	psr.add<int>("delay", 'd', "start-up delay", false, 0);
	psr.add<string>("action", 'a', "show or hide", false, "hide", cmdline::oneof<string>("show", "hide"));
	psr.add("recoverable", 'r', "whether the icon can be restored to display");
	psr.add<string>("help", 'h', specification, false, "");

	psr.parse_check(argc, argv);

	string iconStr = psr.get<string>("icon");
	string action = psr.get<string>("action");
	int delay = psr.get<int>("delay");
	bool recoverable = psr.exist("recoverable");

	printf("\naction:[%s]\ndelay:[%d]\nrecoverable:[%s]\nicons: [%s]\n", action.c_str(), delay, recoverable ? "true" : "false", iconStr.c_str());
	
	if (iconStr == ""){
		printf("\nerror: invalid argv --icon is empty");
		return -1;
	}

	vector<string> sourceIcons = split(iconStr, ' ');
	vector<string> icons;
	for (int i = 0; i < sourceIcons.size(); i++)
	{
		string sourceIcon = sourceIcons.at(i);
		string newIcon = sourceIcon;
		trim(newIcon);
		replaceAll(newIcon, "{dquotation}", "\"");
		replaceAll(newIcon, "{squotation}", "'");
		replaceAll(newIcon, "{space}", " ");
		if (newIcon == "") continue;
		icons.push_back(newIcon);

		printf("\nicon:[%s]\tsource:[%s]", newIcon.c_str(), sourceIcon.c_str());
	}

	bool visible = action == "show";

	printf("\ndelay %d seconds...", delay);
	Sleep(delay * 1000);

	printf("\nstart to execute...");

	SetTrayIconVisable(FindOverflowTrayWindow(), icons, visible, !recoverable);
	SetTrayIconVisable(FindNormalTrayWindow(), icons, visible, !recoverable);

	printf("\ndone.");

	return 0;
}


///// ---for test---debug in vs---
//int main() {
//	bool visible = false;
//
//	vector<string> icons;
//	string c = "ShareOnLan";
//	string c1 = "Quick";
//	string c2 = "Ditto";
//	string c3 = "uTools.exe";
//	string c4 = "Everything";
//	string c5 = "NVIDIA";
//	icons.push_back(c);
//	icons.push_back(c1);
//	icons.push_back(c2);
//	icons.push_back(c3);
//	icons.push_back(c4);
//	icons.push_back(c5);
//
//	SetTrayIconVisable(FindOverflowTrayWindow(), icons, visible, true); // set isHardDelete to true if you don't want to restore the icon
//	SetTrayIconVisable(FindNormalTrayWindow(), icons, visible, true);
//
//	fflush(stdout);
//	system("pause");
//}
