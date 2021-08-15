# HideTrayIcon
Hide/Show TrayIcon for 64bit Windows.

## exe在cmd下调用方式：
**hideTrayIcon.exe [前置时间] [托盘tooltip含有的部分文字] [...]**
如：hideTrayIcon.exe 5 hide/show Text1 Text2
（表示启动程序延时**5**S，**隐藏(hide)**或**显示(show)**图标，且如果托盘的提示文字中含有**Text1**或**Text2**字样，就隐藏此托盘图标，还可以有Text3 Text4等..）

## 开机登陆后延时10S执行
(1)在hideTrayIcon.exe所在目录创建一个vbs文件，输入以下内容
```
Set ws = CreateObject("Wscript.Shell")
ws.run "cmd /c hideTrayIcon.exe 10 hide 文本", vbhide
```
其中，参数“10”是10秒；参数“文本”是要隐藏的托盘图标上含有的部分或全部ToolTip提示文本，如火绒的我们可以将 “文本” 替换为 “火绒安全” 或 “版本号”
![火绒的tooltip](https://img-blog.csdnimg.cn/20210124234341996.png)


(2)将vbs文件的快捷方式放在C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup目录下
