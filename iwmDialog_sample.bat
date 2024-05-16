@echo off
cls

set LN=--------------------------------------------------------------------------------

echo %LN%

:R10
	for /f "usebackq delims=" %%s in (`"iwmDirDialog.exe -cp=932"`) do echo %%s
:R19

echo %LN%

:R20
	for /f "usebackq delims=" %%s in (`"iwmFileDialog.exe -cp=932"`) do echo %%s
:R29

echo %LN%

sleep 2
exit
