@echo off
chcp 932 >NUL
cls

set CMD=.\iwmFileDialog+FLTK.exe -cp=932

echo.

:R10
	echo フォルダ選択
	for /f "usebackq delims=" %%s in (`"%CMD% -t=d"`) do echo %%s
:R19

echo.

:R20
	echo ファイル選択
	for /f "usebackq delims=" %%s in (`"%CMD% -t=m"`) do echo %%s
:R29

echo.

pause
exit
