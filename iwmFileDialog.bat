:: Ini ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	@echo off
	cls

	:: ファイル名はソースと同じ
	set fn=%~n0
	set src=%fn%.c
	set fn_exe=%fn%.exe
	set cc=gcc.exe
	set cc_op=-Os -Wall -lgdi32 -luser32 -lshlwapi -lcomdlg32
	set lib=lib_iwmutil2.a

:: Make ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::	echo --- Compile -S ------------------------------------
::	for %%s in (%src%) do (
::		%cc% %%s -S %cc_op%
::		echo %%~ns.s
::	)
::	echo.

	echo --- Make ------------------------------------------
	for %%s in (%src%) do (
		echo %%s
		%cc% %%s -c -Wall %cc_op%
	)
	%cc% *.o %lib% -o %fn_exe% %cc_op%
	echo.

	:: 後処理
	strip %fn_exe%
	rm *.o

	:: 失敗
	if not exist "%fn_exe%" goto end

	:: 成功
	echo.
	pause
	cls

:: Test ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	%fn_exe% -h

	chcp 65001
	for /f "usebackq delims=" %%s in (`%fn_exe% -d=c:\windows`) do echo %%s

:: Quit ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:end
	echo.
	pause
	exit
