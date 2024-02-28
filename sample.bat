@echo off
cls

set LN=--------------------------------------------------------------------------------

echo %LN%

:R10
	set rtn=
	for /f "usebackq delims=" %%s in (`"iwmDirDialog.exe"`) do (
		set rtn=%%s
		echo %%s
	)
	if "%rtn%" == "" goto R19
	goto R10
:R19

echo %LN%

:R20
	set rtn=
	for /f "usebackq delims=" %%s in (`"iwmFileDialog.exe -l"`) do (
		set rtn=%%s
		echo %%s
	)
	if "%rtn%" == "" goto R29
	goto R20
:R29

echo %LN%

::pause
exit
