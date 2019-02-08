@echo off

REM Virtual address start from 0x4000_0000
set DAM_RO_BASE=0x40000000

set TOOL_PATH_ROOT=C:\compile_tools
set TOOLCHAIN_PATH=%TOOL_PATH_ROOT%\ARM_Compiler_5\bin
set LM_LICENSE_FILE=%TOOL_PATH_ROOT%\license.dat

set BAT_COMMAND=build_oem_example.bat
REM DAM related path
set DAM_OUTPUT_PATH=bin
set DAM_INC_BASE=include
set DAM_LIB_PATH=libs

REM application related path
set APP_SRC_P_PATH=src\example
set APP_OUTPUT_PATH=src\example\build

REM example utils source and header path
set APP_UTILS_SRC_PATH=src\utils\source
set APP_UTILS_INC_PATH=src\utils\include


if "%1%"=="-c" (
    echo == Cleaning... %DEMO_ELF_OUTPUT_PATH%
    del /q /s %DAM_OUTPUT_PATH%\*
    echo == Cleaning... %DEMO_APP_OUTPUT_PATH%
    del /q /s %APP_OUTPUT_PATH%\*

    echo Done.
    exit /b
) else if "%1%"=="device_info" (
	REM Example for device_info
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_DEVICE_INFO__
) else if "%1%"=="gpio" (
	REM Example for gpio
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_GPIO__
) else if "%1%"=="gpio_int" (
	REM Example for gpio_int
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_GPIO_INT__
) else if "%1%"=="gps" (
	REM Example for gps
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_GPS__
) else if "%1%"=="qt_gps" (
	REM Example for qt_gps
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_QT_GPS__
) else if "%1%"=="http" (
	REM Example for http
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_HTTP__
) else if "%1%"=="psm" (
	REM Example for psm
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_PSM__
) else if "%1%"=="rtc" (
	REM Example for rtc
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_RTC__
) else if "%1%"=="task_create" (
	REM Example for task_create
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_TASK_CREATE__
) else if "%1%"=="tcp_client" (
	REM Example for tcp_client
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_TCPCLIENT__
) else if "%1%"=="time" (
	REM Example for time
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_TIME__
) else if "%1%"=="timer" (
	REM Example for timer
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_TIMER__
) else if "%1%"=="uart" (
	REM Example for uart
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_UART__
) else if "%1%"=="atc_pipe" (
	REM Example for atc_pipe
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_ATC_PIPE__
) else if "%1%"=="atc_sms" (
	REM Example for atc_sms
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_ATC_SMS__
) else if "%1%"=="i2c" (
	REM Example for i2c
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_I2C__
) else if "%1%"=="mqtt" (
	REM Example for mqtt
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_MQTT__
) else if "%1%"=="spi" (
	REM Example for spi
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_SPI__
) else if "%1%"=="dns_client" (
	REM Example for dns_client
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_DNSCLIENT__
) else if "%1%"=="adc" (
	REM Example for adc
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_ADC__
) else if "%1%"=="qt_adc" (
	REM Example for qt_adc
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_QT_ADC__
) else if "%1%"=="nonip" (
	REM Example for nonip
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_NONIP__
) else if "%1%"=="fota" (
	REM Example for fota
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_FOTA__
) else if "%1%"=="lwm2m" (
	REM Example for lwm2m
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_LWM2M_EXT__
)else if "%1%"=="atfwd" (
	REM Example for atfwd
    echo == Compiling %1% example ...
    set BUILD_APP_FLAG=-D__EXAMPLE_ATFWD__
) else if "%1%"=="help" (
    goto help_menu
) else if "%1%"=="HELP" (
    goto help_menu
) else (
    echo Please input a valid example build id !
	goto help_menu
)

REM example source and header path
set APP_SRC_PATH=src\example\%1%\src
set APP_INC_PATH=src\example\%1%\inc

set DAM_LIBNAME="txm_lib.lib"
set TIMER_LIBNAME="timer_dam_lib.lib"

set DAM_ELF_NAME=example_%1%.elf
set DAM_TARGET_BIN=example_%1%.bin
set DAM_TARGET_MAP=example_%1%.map

if not exist %DAM_OUTPUT_PATH% (
    mkdir %DAM_OUTPUT_PATH%
)

echo == Application RO base selected = %DAM_RO_BASE%

set DAM_CPPFLAGS=-DT_ARM -D__RVCT__ -D_ARM_ASM_ -DQAPI_TXM_MODULE -DTXM_MODULE -DTX_DAM_QC_CUSTOMIZATIONS -DTX_ENABLE_PROFILING -DTX_ENABLE_EVENT_TRACE -DTX_DISABLE_NOTIFY_CALLBACKS -DTX_DAM_QC_CUSTOMIZATIONS
set DAM_CLAGS=-O1 --diag_suppress=9931 --diag_error=warning --cpu=Cortex-A7 --protect_stack --arm_only --apcs=\interwork
set DAM_INCPATHS=-I %DAM_INC_BASE% -I %DAM_INC_BASE%\threadx_api -I %DAM_INC_BASE%\qapi -I %APP_UTILS_INC_PATH% -I %APP_INC_PATH%
set APP_CFLAGS=-DTARGET_THREADX -DENABLE_IOT_INFO -DENABLE_IOT_DEBUG -DSENSOR_SIMULATE

@echo off

@echo == Compiling .S file...
%TOOLCHAIN_PATH%\armcc -E -g %DAM_CPPFLAGS% %DAM_CFLAGS% %APP_SRC_P_PATH%\txm_module_preamble.S > txm_module_preamble_pp.S
%TOOLCHAIN_PATH%\armcc -g -c %DAM_CPPFLAGS% %DAM_CFLAGS% txm_module_preamble_pp.S -o %APP_OUTPUT_PATH%\txm_module_preamble.o
del txm_module_preamble_pp.S

@echo == Compiling .C file...
for %%x in (%APP_SRC_PATH%\*.c) do (
    %TOOLCHAIN_PATH%\armcc -g -c %DAM_CPPFLAGS% %DAM_CFLAGS% %APP_CFLAGS% %BUILD_APP_FLAG% %DAM_INCPATHS% %%x
)
for %%x in (%APP_UTILS_SRC_PATH%\*.c) do (
    %TOOLCHAIN_PATH%\armcc -g -c %DAM_CPPFLAGS% %DAM_CFLAGS% %APP_CFLAGS% %BUILD_APP_FLAG% %DAM_INCPATHS% %%x
)
move *.o %APP_OUTPUT_PATH%

setlocal enabledelayedexpansion
set OBJ_FULL_PATH=
for %%x in (%APP_OUTPUT_PATH%\*.o) do (
    set OBJ_FULL_PATH=!OBJ_FULL_PATH! %%x
)

echo == Linking Example %1% application
%TOOLCHAIN_PATH%\armlink -d -o %DAM_OUTPUT_PATH%\%DAM_ELF_NAME% --elf --ro %DAM_RO_BASE% --first txm_module_preamble.o --entry=_txm_module_thread_shell_entry --map --remove --symbols --list %DAM_OUTPUT_PATH%\%DAM_TARGET_MAP% %OBJ_FULL_PATH% %DAM_LIB_PATH%\timer_dam_lib.lib %DAM_LIB_PATH%\txm_lib.lib
%TOOLCHAIN_PATH%\fromelf --bincombined %DAM_OUTPUT_PATH%\%DAM_ELF_NAME% --output %DAM_OUTPUT_PATH%\%DAM_TARGET_BIN%

echo == Example image built at %DAM_OUTPUT_PATH%\%DAM_TARGET_BIN%
set /p =/datatx/example_%1%.bin<nul > .\bin\oem_app_path.ini
echo Done.
goto end

:help_menu
    echo Supported example :
    echo    device_info [ cmd - %BAT_COMMAND% device_info ]
    echo    gpio        [ cmd - %BAT_COMMAND% gpio        ]
    echo    gpio_int    [ cmd - %BAT_COMMAND% gpio_int    ]
    echo    gps         [ cmd - %BAT_COMMAND% gps         ]
    echo    qt_gps      [ cmd - %BAT_COMMAND% qt_gps      ]
    echo    http        [ cmd - %BAT_COMMAND% http        ]
    echo    psm         [ cmd - %BAT_COMMAND% psm         ]
    echo    rtc         [ cmd - %BAT_COMMAND% rtc         ]
	echo    task_create [ cmd - %BAT_COMMAND% task_create ]
    echo    tcp_client  [ cmd - %BAT_COMMAND% tcp_client  ]
    echo    time        [ cmd - %BAT_COMMAND% time        ]
    echo    timer       [ cmd - %BAT_COMMAND% timer       ]
    echo    uart        [ cmd - %BAT_COMMAND% uart        ]
    echo    atc_pipe    [ cmd - %BAT_COMMAND% atc_pipe    ]
    echo    atc_sms     [ cmd - %BAT_COMMAND% atc_sms     ]
    echo    i2c         [ cmd - %BAT_COMMAND% i2c         ]
	echo    mqtt        [ cmd - %BAT_COMMAND% mqtt        ]
    echo    spi         [ cmd - %BAT_COMMAND% spi         ]
    echo    dns_client  [ cmd - %BAT_COMMAND% dns_client  ]
    echo    adc         [ cmd - %BAT_COMMAND% adc         ]
    echo    qt_adc      [ cmd - %BAT_COMMAND% qt_adc      ]
    echo    nonip       [ cmd - %BAT_COMMAND% nonip       ]
    echo    fota        [ cmd - %BAT_COMMAND% fota        ]
    echo    lwm2m       [ cmd - %BAT_COMMAND% lwm2m       ]
	echo    atfwd       [ cmd - %BAT_COMMAND% atfwd       ]
    exit /b

:end
