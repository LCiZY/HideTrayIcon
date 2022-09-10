#pragma once
#include<string>


std::string specification = "\n\
-d --delay\n\
	start-up delay in seconds 启动延时（秒）\n\
	default: 0\n\
-r --recoverable\n\
	whether the icon can be restored to display 图标是否可以恢复显示\n\
-a --action\n\
	show or hide, show only works if - r or --recoverable is passed to program when the program is run for the first time 仅当第一次运行本程序时，给--recoverable传递true，show这个选项才生效\n\
	default: hide\n\
-i --icon\n\
	list of trayIcons, can be tooltip and processName, separated by space 图标列表，可以是托盘提示文本和进程名称，由空格分割\n\
	eg. -i 'Everything NVIDIA uTools.exe'\n\
	eg. -i 'Goland{space}2021.1.3' # \"Goland 2021.1.3\" contain space\n\
	eg. -i 'Carl{squotation}s{space}cat' # \"Carl's cat\" contain single quotation mark and space\n\
	eg. -i 'Robert{space}Frost{squotation}s{space}{dquotation}Design{dquotation}.' # \"Robert Frost's \"Design\".\" contain single quotation mark and double quotation mark and space\n\
	mandatory\n\n\
";
