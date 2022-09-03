#pragma once
#include<string>


std::string specification = "\n\
-d --delay\n\
	start-up delay in seconds ������ʱ���룩\n\
	default: 0\n\
-r --recoverable\n\
	whether the icon can be restored to display ͼ���Ƿ���Իָ���ʾ\n\
-a --action\n\
	show or hide, show only works if - r or --recoverable is passed to program when the program is run for the first time ������һ�����б�����ʱ����--recoverable����true��show���ѡ�����Ч\n\
	default: hide\n\
-i --icon\n\
	list of trayIcons, can be tooltip and processName, separated by space ͼ���б�������������ʾ�ı��ͽ������ƣ��ɿո�ָ�\n\
	eg. -i 'Everything NVIDIA uTools.exe'\n\
	eg. -i 'Goland{space}2021.1.3' # \"Goland 2021.1.3\" contain space\n\
	eg. -i 'Carl{squotation}s{space}cat' # \"Carl's cat\" contain single quotation mark and space\n\
	eg. -i 'Robert{space}Frost{squotation}s{space}{dquotation}Design{dquotation}.' # \"Robert Frost's \"Design\".\" contain single quotation mark and double quotation mark and space\n\
	mandatory\n\n\
";