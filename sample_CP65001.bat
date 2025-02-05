@echo off
cls

set CMD=.\iwmFileDialog+FLTK.exe

echo.

:R10
	chcp 65001 >NUL
	echo フォルダ選択
	for /f "usebackq delims=" %%s in (`"%CMD% -t=d"`) do echo %%s
:R19

echo.

:R20
	chcp 65001 >NUL
	echo ファイル選択
	for /f "usebackq delims=" %%s in (`"%CMD% -t=m"`) do echo %%s
:R29

echo.

pause
exit
