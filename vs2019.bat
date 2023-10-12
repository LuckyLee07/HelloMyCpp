@echo off

set SCRIPT_DIRECTORY=%~dp0
set CURRENT_DIRECTORY=%cd%
set ARGUMENTS=%*

cd /d %SCRIPT_DIRECTORY%

rem rmdir /S/Q build
cd build
del /S/Q *.sln *.*
cd ..

cd bin
del /S/Q *.log *.pdb *.exe
cd ..

echo on
tools\premake\premake5 --os=windows --file=premake.lua vs2019 %ARGUMENTS%

cd /d %CURRENT_DIRECTORY%

pause