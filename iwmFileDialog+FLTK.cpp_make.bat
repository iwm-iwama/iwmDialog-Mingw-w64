@echo off
cls

:: "ベースファイル名" + "_make.bat"
set batch=%~nx0
set src=%batch:_make.bat=%
set fn=
for /f "delims=. tokens=1,2" %%i in ("%src%") do (
	set fn=%%i%
)
set fn_exe=%fn%.exe
set cc=g++.exe -std=c++23
set lib=lib_iwmutil2.a
:: -static 以降は bash $(fltk-config --ldstaticflags) を展開したもの
set op_link=-Os -Wall -Wextra -lgdi32 -luser32 -lshlwapi -static -pipe -Wl,-subsystem,console -mwindows c:\msys64\mingw64\lib\libfltk.a -lole32 -luuid -lcomctl32 -lws2_32

:: Make
	echo --- Make ------------------------------------------
	%cc% %src% %lib% %op_link% -o %fn_exe%
	strip %fn_exe%
	echo.

:: Test
	pause
	%fn%.exe
	for /f "usebackq delims=" %%s in (`"%fn_exe% -cp=932"`) do echo %%s

:: Quit
	echo.
	pause
	exit
