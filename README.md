中文 **|** [English](https://github.com/LCiZY/HideTrayIcon/blob/main/README_EN.md)

# HideTrayIcon

Hide/Show TrayIcon for 64bit Windows 10.

---

# Function

```c++
// tray_icon.h & tray_icon.cpp
// 得到托盘窗口句柄，拓展的托盘窗口句柄
HWND FindOverflowTrayWindow();
// 得到托盘窗口句柄，任务栏的托盘窗口句柄
HWND FindNormalTrayWindow();

// 设置任务栏中图标的可见状态
// @param hWnd: 图标所在窗口的句柄
// @param process_name_or_tooltips: 这个vector的元素可以是进程名或托盘提示的子字符串 
// @param visible: true是显示，false是隐藏
// @param isHardDelete: 硬删除，仅在visible为false时生效
VOID SetTrayIconVisable(HWND hWnd, std::vector<std::string>& process_name_or_tooltips, bool visible, bool isHardDelete = true);
```

# Exe在cmd下调用方式

- 使用示例:
``` cmd
hideTrayIcon.exe -i 'Everything NVIDIA uTools.exe' [其他选项]
-d --delay
	启动延时，单位秒
	default: 0
-r --recoverable
	图标是否可以恢复显示
-a --action
	show 或 hide, 仅当系统启动后第一次运行本程序时，给--recoverable传递了true，show这个选项才生效
	default: hide
-i --icon
	图标列表，可以是托盘提示文本和进程名称，由空格分割
	eg. -i Everything
	eg. -i 'Everything NVIDIA uTools.exe'
	eg. -i 'Goland{space}2021.1.3' # \"Goland 2021.1.3\" 里包含空格，使用{space}占位符
	eg. -i 'Carl{squotation}s{space}cat' # \"Carl's cat\" 包含'，使用占位符{squotation}
	eg. -i 'Robert{space}Frost{squotation}s{space}{dquotation}Design{dquotation}.' # \"Robert Frost's \"Design\".\"包含"，使用占位符{dquotation}
	必要参数
```
> - **5**表示启动程序延时5S
>
> - *--action*的参数从**hide**和**show**二选一，分别执行隐藏和显示托盘图标的操作
>
> - *--icon*参数的每一个元素，可以是**托盘tooltip含有的部分文字**，也可以是**进程名**，可以传递多个由空格分割的元素

## 开机登陆后延时10S执行

**步骤1**

在hideTrayIcon.exe所在目录创建一个vbs文件，输入以下内容

```vbscript
Set ws = CreateObject("Wscript.Shell")
ws.run "cmd /c hideTrayIcon.exe -a hide -d 10 -i ""Everything NVIDIA uTools.exe 其他""", vbhide
```
解释：

```shell
-a hide # 表示隐藏操作
-d 10 # 表示延时10s执行；
-i "Everything NVIDIA uTools.exe 其他"  # 表示若托盘的提示文本中或进程名中含有Everything或NVIDIA或uTools.exe，则隐藏托盘
# 在vb脚本里，"需要用""表示
```

上述命令中的*其他*可以是**托盘tooltip含有的部分文字**或**进程名称**，如下图的火绒托盘，我们可以将 *其他* 替换为 “火绒安全” 或 “版本号” 或 "HipsTray.exe"（火绒托盘的进程名称）
![火绒的tooltip](./SnapShots/eg_cn.jpg)

**步骤2**：注意： 如果vbs脚本文件内存在非ascii字符(比如中文)，需要用记事本打开vbs文件然后以ascii编码重新保存

**步骤3**：将vbs文件的快捷方式放在`C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup`目录下

