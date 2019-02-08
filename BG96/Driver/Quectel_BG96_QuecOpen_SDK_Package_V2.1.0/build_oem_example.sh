#!/bin/sh

##Virtual address start from 0x4000_0000
DAM_RO_BASE=0x40000000

TOOLCHAIN_PATH="C:/compile_tools/ARM_Compiler_5/bin"
export LM_LICENSE_FILE="C:/compile_tools/license.dat"

SH_COMMAND=build_oem_example.sh
#DAM related path
DAM_OUTPUT_PATH="./bin"
DAM_INC_BASE="./include"
DAM_LIB_PATH="./libs"

#application related path
APP_SRC_P_PATH="./src/example"
APP_OUTPUT_PATH="./src/example/build"

#example utils source and header path
APP_UTILS_SRC_PATH+="./src/utils/source"
APP_UTILS_INC_PATH+="./src/utils/include"

#parse command line input parameters
if [ $# -eq 1 ]; then
    if [ $1 == "-c" ]; then
		echo "Cleaning..."
		rm -rf $DAM_OUTPUT_PATH/*
		rm -rf $APP_OUTPUT_PATH/*o
		echo "Done."
		exit
	# build device_info
	elif [ $1 == "device_info" ]; 	then	BUILD_APP_FLAG="-D__EXAMPLE_DEVICE_INFO__"
	# build gpio
	elif [ $1 == "gpio" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_GPIO__"
	# build gpio_int
	elif [ $1 == "gpio_int" ]; 		then	BUILD_APP_FLAG="-D__EXAMPLE_GPIO_INT__"
	# build gps
	elif [ $1 == "gps" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_GPS__"
	# build qt_gps
	elif [ $1 == "qt_gps" ]; 		then	BUILD_APP_FLAG="-D__EXAMPLE_QT_GPS__"
	# build gpiio
	elif [ $1 == "http" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_HTTP__"
	# build psm
	elif [ $1 == "psm" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_PSM__"
	# build rtc
	elif [ $1 == "rtc" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_RTC__"
	# build task_create
	elif [ $1 == "task_create" ]; 	then	BUILD_APP_FLAG="-D__EXAMPLE_TASK_CREATE__"
	# build tcp_client
	elif [ $1 == "tcp_client" ]; 	then	BUILD_APP_FLAG="-D__EXAMPLE_TCPCLIENT__"
	# build time
	elif [ $1 == "time" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_TIME__"
	# build timer
	elif [ $1 == "timer" ]; 		then	BUILD_APP_FLAG="-D__EXAMPLE_TIMER__"
	# build uart
	elif [ $1 == "uart" ]; 			then 	BUILD_APP_FLAG="-D__EXAMPLE_UART__"
	# build atc_pipe
	elif [ $1 == "atc_pipe" ]; 		then 	BUILD_APP_FLAG="-D__EXAMPLE_ATC_PIPE__"
	# build atc_sms
	elif [ $1 == "atc_sms" ]; 		then 	BUILD_APP_FLAG="-D__EXAMPLE_ATC_SMS__"
	# build i2c
	elif [ $1 == "i2c" ]; 			then 	BUILD_APP_FLAG="-D__EXAMPLE_I2C__"
	# build mqtt
	elif [ $1 == "mqtt" ]; 			then 	BUILD_APP_FLAG="-D__EXAMPLE_MQTT__"
	# build spi
	elif [ $1 == "spi" ]; 			then 	BUILD_APP_FLAG="-D__EXAMPLE_SPI__"
	# build dns client
	elif [ $1 == "dns_client" ]; 	then	BUILD_APP_FLAG="-D__EXAMPLE_DNSCLIENT__"
	# build adc
	elif [ $1 == "adc" ]; 			then 	BUILD_APP_FLAG="-D__EXAMPLE_ADC__"
	# build quectel_adc
	elif [ $1 == "qt_adc" ]; 		then 	BUILD_APP_FLAG="-D__EXAMPLE_QT_ADC__"
	# build nipd
	elif [ $1 == "nonip" ]; 		then	BUILD_APP_FLAG="-D__EXAMPLE_NONIP__"
	# build fota
	elif [ $1 == "fota" ]; 			then	BUILD_APP_FLAG="-D__EXAMPLE_FOTA__"
	# build lwm2m
	elif [ $1 == "lwm2m" ];			then	BUILD_APP_FLAG="-D__EXAMPLE_LWM2M_EXT__"
	# build atfwd
	elif [ $1 == "atfwd" ];			then	BUILD_APP_FLAG="-D__EXAMPLE_ATFWD__"
	elif [ $1 == "help" ];			then
	        echo "Supported example :"
			echo "  device_info [ cmd - $SH_COMMAND device_info ]"
			echo "  gpio        [ cmd - $SH_COMMAND gpio        ]"
			echo "  gpio_int    [ cmd - $SH_COMMAND gpio_int    ]"
			echo "  gps         [ cmd - $SH_COMMAND gps         ]"
			echo "  qt_gps      [ cmd - $SH_COMMAND qt_gps      ]"
			echo "  http        [ cmd - $SH_COMMAND http        ]"
			echo "  psm         [ cmd - $SH_COMMAND psm         ]"
			echo "  rtc         [ cmd - $SH_COMMAND rtc         ]"
			echo "  task_create [ cmd - $SH_COMMAND task_create ]"
			echo "  tcp_client  [ cmd - $SH_COMMAND tcp_client  ]"
			echo "  time        [ cmd - $SH_COMMAND time        ]"
			echo "  timer       [ cmd - $SH_COMMAND timer       ]"
			echo "  uart        [ cmd - $SH_COMMAND uart        ]"
			echo "  atc_pipe    [ cmd - $SH_COMMAND atc_pipe    ]"
			echo "  atc_sms     [ cmd - $SH_COMMAND atc_sms     ]"
			echo "  i2c         [ cmd - $SH_COMMAND i2c         ]"
			echo "  mqtt        [ cmd - $SH_COMMAND mqtt        ]"
			echo "  spi         [ cmd - $SH_COMMAND spi         ]"
			echo "  dns_client  [ cmd - $SH_COMMAND dns_client  ]"
			echo "  adc         [ cmd - $SH_COMMAND adc         ]"
			echo "  qt_adc      [ cmd - $SH_COMMAND qt_adc      ]"
			echo "  nonip       [ cmd - $SH_COMMAND nonip       ]"
			echo "  fota        [ cmd - $SH_COMMAND fota        ]"
			echo "  lwm2m       [ cmd - $SH_COMMAND lwm2m       ]"
			echo "  atfwd       [ cmd - $SH_COMMAND atfwd       ]"
			exit
	else
		echo "Please input a valid example build id !"
		exit
	fi
else
	echo "Please input a valid example build id !"
	exit
fi

#example source and header path
APP_SRC_PATH="./src/example/$1/src"
APP_INC_PATH="./src/example/$1/inc"

DAM_LIBNAME="txm_lib.lib"
TIMER_LIBNAME="timer_dam_lib.lib"

DAM_ELF_NAME="example_$1.elf"
DAM_TARGET_BIN="example_$1.bin"
DAM_TARGET_MAP="example_$1.map"

if [ ! -d $DAM_OUTPUT_PATH ]; then
    mkdir $DAM_OUTPUT_PATH
fi

echo "=== Application RO base selected = $DAM_RO_BASE"

export DAM_CPPFLAGS="-DT_ARM -D__RVCT__ -D_ARM_ASM_ -DQAPI_TXM_MODULE -DTXM_MODULE -DTX_DAM_QC_CUSTOMIZATIONS -DTX_ENABLE_PROFILING -DTX_ENABLE_EVENT_TRACE -DTX_DISABLE_NOTIFY_CALLBACKS -DTX_DAM_QC_CUSTOMIZATIONS" 
export DAM_CLAGS="-O1 --diag_suppress=9931 --diag_error=warning --cpu=Cortex-A7 --protect_stack --arm_only --apcs=/interwork"
export DAM_INCPATHS="-I $DAM_INC_BASE -I $DAM_INC_BASE/threadx_api -I $DAM_INC_BASE/qapi -I $APP_UTILS_INC_PATH -I $APP_INC_PATH"
export APP_CFLAGS="-DTARGET_THREADX -DENABLE_IOT_INFO -DENABLE_IOT_DEBUG -DSENSOR_SIMULATE"

#Turn on verbose mode by default
set -x;

echo "=== Compiling Example $1"
echo "== Compiling .S file..."
$TOOLCHAIN_PATH/armcc -E -g $DAM_CPPFLAGS $DAM_CFLAGS $APP_SRC_P_PATH/txm_module_preamble.S > txm_module_preamble_pp.S
$TOOLCHAIN_PATH/armcc -g -c $DAM_CPPFLAGS $DAM_CFLAGS txm_module_preamble_pp.S -o $APP_OUTPUT_PATH/txm_module_preamble.o
rm txm_module_preamble_pp.S

echo "== Compiling .C file..."
$TOOLCHAIN_PATH/armcc -g -c $DAM_CPPFLAGS $DAM_CFLAGS $APP_CFLAGS $BUILD_APP_FLAG $DAM_INCPATHS $APP_UTILS_SRC_PATH/*.c $APP_SRC_PATH/*.c 
mv *.o $APP_OUTPUT_PATH

echo "=== Linking Example $1 application"
$TOOLCHAIN_PATH/armlink -d -o $DAM_OUTPUT_PATH/$DAM_ELF_NAME --elf --ro $DAM_RO_BASE --first txm_module_preamble.o --entry=_txm_module_thread_shell_entry --map --remove --symbols --list $DAM_OUTPUT_PATH/$DAM_TARGET_MAP $APP_OUTPUT_PATH/*.o $DAM_LIB_PATH/$DAM_LIBNAME $DAM_LIB_PATH/$TIMER_LIBNAME
$TOOLCHAIN_PATH/fromelf --bincombined $DAM_OUTPUT_PATH/$DAM_ELF_NAME --output $DAM_OUTPUT_PATH/$DAM_TARGET_BIN

set +x;

echo "=== Example $1 is built at" $DAM_OUTPUT_PATH/$DAM_TARGET_BIN
echo -n "/datatx/example_$1.bin" > ./bin/oem_app_path.ini
echo "Done."
