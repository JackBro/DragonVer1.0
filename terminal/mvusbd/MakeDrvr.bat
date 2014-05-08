@echo off
if "%1"=="" goto usage
if "%3"=="" goto usage
if not exist %1\bin\setenv.bat goto usage

call %1\bin\setenv %1 %4 %5
%2
cd %3
build -b -w %7 %8 %9 %10

goto exit


:usage
echo usage   MakeDrvr [DDKDIR] [WORKDRIVE] [PRJDIR] free/checked ....
:exit
