echo ��������VS2010�����в���Ҫ���ļ�
echo ��ȷ�����ļ������ڹ���Ŀ¼֮�в��ر�VS2010
echo ��ʼ�������Ե�......

for /r %%i in (.) do (

echo ����sdf�ļ�
del /q/a/f/s %%i\*.sdf

echo ����ipch�ļ�
rd /q/s %%i\ipch 

echo ����Debug�ļ�
del /q/a/f/s %%i\Debug\*.obj
del /q/a/f/s %%i\Debug\*.tlog
del /q/a/f/s %%i\Debug\*.log
del /q/a/f/s %%i\Debug\*.idb
del /q/a/f/s %%i\Debug\*.pdb
del /q/a/f/s %%i\Debug\*.ilk
del /q/a/f/s %%i\Debug\*.pch
del /q/a/f/s %%i\Debug\*.bsc
del /q/a/f/s %%i\Debug\*.sbr

echo ����Release�ļ�
del /q/a/f/s %%i\Release\*.obj
del /q/a/f/s %%i\Release\*.tlog
del /q/a/f/s %%i\Release\*.log
del /q/a/f/s %%i\Release\*.idb
del /q/a/f/s %%i\Release\*.pdb
del /q/a/f/s %%i\Release\*.ilk
del /q/a/f/s %%i\Release\*.pch

echo ����Temp�ļ�
del /q/a/f/s %%i\Temp\*.*
)

ECHO �ļ�������ϣ���������3����˳������ڽ��뵹��ʱ......... 
@echo off 
echo WScript.Sleep 300 > %temp%.\tmp$$$.vbs 
set /a i =3 
:Timeout 
if %i% == 0 goto Next 
setlocal 
set /a i = %i% - 1 
echo ����ʱ����%i% 
cscript //nologo %temp%.\tmp$$$.vbs 
goto Timeout 
goto End 
:Next 
cls & 
echo.