On Error Resume Next

Set xlObj = CreateObject("Excel.Application")
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFolder = objFSO.GetFolder(".")
Set colFiles = objFolder.Files
For Each objFile in colFiles
	If objFSO.GetExtensionName(objFile.name) = "xlsx" Then
		set xlWB = xlObj.Workbooks.Open(objFile)
		thisFileName =Left(xlWB.FullName , InStrRev(xlWB.FullName , ".") - 1)
		xlWB.Sheets.Select
		xlWB.ActiveSheet.ExportAsFixedFormat 0, thisFileName & ".pdf", 0, 1, 0,,,0
		xlWB.close False
	End If	
Next
xlObj.quit