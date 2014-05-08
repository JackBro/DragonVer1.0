@echo off
if "%1"=="" goto usage
if "%3"=="" goto usage
if not exist %1\bin\setenv.bat goto usage

call %1\bin\setenv %1 %4 %5
%2
cd %3
build -b -w %7 %8 %9 %10

md output
copy .\VCMobi.inf .\output\VCMobi.inf

if "%4"=="checked" goto cpychk 
if "%4"=="free" goto cpyfree 
goto exit

:cpychk
copy .\objchk_wnet_x86\i386\VCMobi.sys .\output\VCMobi.sys
goto exit
:cpyfree
copy .\objfre_wnet_x86\i386\VCMobi.sys .\output\VCMobi.sys
goto exit

:usage
echo usage   MakeDrvr [DDKDIR] [WORKDRIVE] [PRJDIR] free/checked ....
:exit
