@echo off
cls

set CMD=iwmFileDialog+FLTK.exe

set LN=--------------------------------------------------------------------------------

echo %LN%

:R10
	echo フォルダ選択
	for /f "usebackq delims=" %%s in (`"%CMD% -t=d -cp=932"`) do echo %%s
:R19

echo %LN%

:R20
	echo ファイル選択
	for /f "usebackq delims=" %%s in (`"%CMD% -t=mf -cp=932"`) do echo %%s
:R29

echo %LN%

sleep 2
exit
