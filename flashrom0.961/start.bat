@ECHO OFF&PUSHD %~DP0 &TITLE flashrom
mode con cols=100 lines=30
color E
set /p com=     请输入串口号（com ?）按回车：
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
 set /p filea=请输入读取后保存的地址和保存名（eg:f:\read.bin）并按回车：
 set /p chipa=请输入FLASH芯片型号（eg:MX25L3206E）并按回车：
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c  %chipa% -r %filea%
pause 
goto Menu
: three
 set /p chipb=请输入FLASH芯片型号（eg:MX25L3206E）并按回车：
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c %chipb% -E
pause 
goto Menu
: four

 set /p fileb=请输入需写入文件的地址和名称（eg:f:\write.bin）并按回车：
 set /p chipc=请输入FLASH芯片型号（eg:MX25L3206E）并按回车：
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c %chipc% -w %fileb%
pause 
goto Menu
: five

 set /p filec=请输入需校验文件的地址和名称（eg:f:\verify.bin）并按回车：
 set /p chipd=请输入FLASH芯片型号（eg:MX25L3206E）并按回车：
 %~dp0flashrom -p serprog:dev=\\.\com%com%:115200 -c %chipd% -v %filec%
pause 
goto Menu
