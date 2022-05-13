# HideTrayIcon
Hide/Show TrayIcon for 64bit Windows.

## exe在cmd下调用方式：
**hideTrayIcon.exe [前置时间] [show/hide] [Text...]**

- example:
``` cmd
hideTrayIcon.exe 5 hide Text1 Text2  # hide trayicon
hideTrayIcon.exe 5 show Text1 Text2  # show trayicon
```
> - 5表示启动程序延时**5**S
>
> - hide和show二选一，分别执行隐藏和显示托盘图标的操作
>
> - Text可以是**托盘tooltip含有的部分文字**，也可以是**进程名**，可以传递多个"Text"

## 开机登陆后延时10S执行
(1)在hideTrayIcon.exe所在目录创建一个vbs文件，输入以下内容
```
Set ws = CreateObject("Wscript.Shell")
ws.run "cmd /c hideTrayIcon.exe 10 hide 文本", vbhide
```
其中，参数“10”是10秒；参数“文本”是**要隐藏的托盘图标上含有的部分或全部ToolTip提示文本**或**进程名称**，如火绒的我们可以将 “文本” 替换为 “火绒安全” or “版本号” or "HipsTray.exe"
![火绒的tooltip](https://img-blog.csdnimg.cn/20210124234341996.png)

> 注意： 如果vbs脚本文件内存在非ascii字符(比如中文)，需要用记事本打开vbs文件然后以ascii编码重新保存

(2)将vbs文件的快捷方式放在C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup目录下
