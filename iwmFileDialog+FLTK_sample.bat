@echo off
cls

set CMD=iwmFileDialog+FLTK.exe

set LN=--------------------------------------------------------------------------------

echo %LN%

:R10
	echo �t�H���_�I��
	for /f "usebackq delims=" %%s in (`"%CMD% -type=d -cp=932"`) do echo %%s
:R19

echo %LN%

:R20
	echo �t�@�C���I��
	for /f "usebackq delims=" %%s in (`"%CMD% -type=mf -cp=932"`) do echo %%s
:R29

echo %LN%

sleep 2
exit
