echo 正在清理VS2010工程中不需要的文件
echo 请确保本文件放置在工程目录之中并关闭VS2010
echo 开始清理请稍等......

for /r %%i in (.) do (

echo 清理sdf文件
del /q/a/f/s %%i\*.sdf

echo 清理ipch文件
rd /q/s %%i\ipch 

echo 清理Debug文件
del /q/a/f/s %%i\Debug\*.obj
del /q/a/f/s %%i\Debug\*.tlog
del /q/a/f/s %%i\Debug\*.log
del /q/a/f/s %%i\Debug\*.idb
del /q/a/f/s %%i\Debug\*.pdb
del /q/a/f/s %%i\Debug\*.ilk
del /q/a/f/s %%i\Debug\*.pch
del /q/a/f/s %%i\Debug\*.bsc
del /q/a/f/s %%i\Debug\*.sbr

echo 清理Release文件
del /q/a/f/s %%i\Release\*.obj
del /q/a/f/s %%i\Release\*.tlog
del /q/a/f/s %%i\Release\*.log
del /q/a/f/s %%i\Release\*.idb
del /q/a/f/s %%i\Release\*.pdb
del /q/a/f/s %%i\Release\*.ilk
del /q/a/f/s %%i\Release\*.pch

echo 清理Temp文件
del /q/a/f/s %%i\Temp\*.*
)

ECHO 文件清理完毕！本程序将在3秒后退出！现在进入倒计时......... 
@echo off 
echo WScript.Sleep 300 > %temp%.\tmp$$$.vbs 
set /a i =3 
:Timeout 
if %i% == 0 goto Next 
setlocal 
set /a i = %i% - 1 
echo 倒计时……%i% 
cscript //nologo %temp%.\tmp$$$.vbs 
goto Timeout 
goto End 
:Next 
cls & 
echo.