@ECHO OFF&PUSHD %~DP0 &TITLE flashrom
mode con cols=100 lines=30
color E
set /p com=     请输入串口号（com ?）按回车：
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200
@ echo.---------------------------------------------------
 set /p chip=请输入FLASH芯片型号（eg:MX25L3206E）并按回车：

:Menu
Cls
@ echo.
@ echo.　　　　　 菜 单 选 项
@ echo.
@ echo.       读取FLASH芯片型号信息 	       → 输入1
@ echo.
@ echo.  	    读取FLASH                  → 输入2
@ echo.
@ echo.             擦除FLASH    	       → 输入3
@ echo.
@ echo.             写入FLASH                  → 输入4
@ echo.
@ echo.               校验                     → 输入5
@ echo.
@ echo. https://www.flashrom.org/
@ echo. https://github.com/posystorage/serprog-stm32-gd32
@ echo. https://github.com/dword1511/serprog-stm32vcp
@ echo.

set /p input=     请入数字选择选项并按回车：
if /i "%input%"=="1" Goto one
if /i "%input%"=="2" Goto two
if /i "%input%"=="3" Goto three
if /i "%input%"=="4" Goto four
if /i "%input%"=="5" Goto five
: one
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200
pause 
goto Menu
: two
 :suba
@ echo.---------------------------------------------------
@ echo.此处需要先建立一个文件，重命名为xxx.bin，再拖到此处
 set /p road=拖动需要保存到的文件路径：
 set road=%road:"=%
 for %%a in ("%road%") do set rn=%%~xa
 set rn=%rn:.=%
 if  %rn%==bin goto oka
 cls
 echo 文件错误 重新输入
 pause
 goto suba
 :oka
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c  %chip% -r %road%
pause 
goto Menu
: three
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c %chip% -E
pause 
goto Menu
: four
 :subb
@ echo.---------------------------------------------------
 set /p road=拖动需要写入的文件到此处：
 set road=%road:"=%
 for %%a in ("%road%") do set rn=%%~xa
 set rn=%rn:.=%
 if  %rn%==bin goto okb
 cls
 echo 文件错误 重新输入
 pause
 goto subb
 :okb
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c %chip% -w %road%
pause 
goto Menu
: five
 :subc
@ echo.---------------------------------------------------
 set /p road=拖动需要校验的文件到此处：
 set road=%road:"=%
 for %%a in ("%road%") do set rn=%%~xa
 set rn=%rn:.=%
 if  %rn%==bin goto okc
 cls
 echo 文件错误 重新输入
 pause
 goto subc
 :okc
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c %chip% -v %road%
pause 
goto Menu
