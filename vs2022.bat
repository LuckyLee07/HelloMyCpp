@echo off

set SCRIPT_DIRECTORY=%~dp0
set CURRENT_DIRECTORY=%cd%
set ARGUMENTS=%*

cd /d %SCRIPT_DIRECTORY%

if exist build (
	cd build
	del /S/Q *.sln *.*
	cd ..
) else (
	mkdir build
)

cd bin
del /S/Q *.log *.pdb *.exe
cd ..

echo on
tools\premake\premake5 --os=windows --file=premake.lua vs2022 %ARGUMENTS%

cd /d %CURRENT_DIRECTORY%

rem pause