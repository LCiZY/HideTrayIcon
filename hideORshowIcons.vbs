Dim sInput, sInput2, sInput3
sInput = InputBox("hide/show")
sInput2 = InputBox("Delay in seconds:")
sInput3 = InputBox("Enter Icon names with spaces")
Set ws = CreateObject("Wscript.Shell")
ws.run "cmd /c hideTrayIcon.exe -a " & sInput & " -d " & sInput2 & " -i " & sInput3, vbhide
