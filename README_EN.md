[中文](https://github.com/LCiZY/HideTrayIcon#readme) **|** English

# HideTrayIcon

Hide/Show TrayIcon for 64bit Windows.

## Usage under command line：
**hideTrayIcon.exe [delay_time] [show/hide] [Text...]**

- example:
``` cmd
hideTrayIcon.exe 5 hide Text1 Text2  # hide trayicon
hideTrayIcon.exe 5 show Text1 Text2  # show trayicon
```
> - **5** means the program starts with a delay of 5 seconds
>
> - choose between **hide** and **show** to hide or show the tray icon respectively.
>
> - **Text** can be part of the text contained in the tray tooltip, or it can be the process name, and multiple "Text" can be passed

## Execute the program after 10 seconds of power on
**STEP 1**: Create a vbs file in the directory where hideTrayIcon.exe is located and enter the following code:

```
Set ws = CreateObject("Wscript.Shell")
ws.run "cmd /c hideTrayIcon.exe 10 hide Text", vbhide
```
**STEP 2**: Replace **Text** with the part of tooltip or part of process path

eg: For the Snipaste's tray icon show below, we can replace Text with *Snipaste 2.5.1* or *Beta* or *Snipaste .exe*

![火绒的tooltip](./SnapShots/eg_en.jpg)

> Note: If there are non-ascii characters in the vbs script file (e.g. Chinese), you need to open the vbs file with Notepad and re-save it with ascii encoding.

**STEP 3**: Put the shortcut to the vbs file in the `C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup` directory
