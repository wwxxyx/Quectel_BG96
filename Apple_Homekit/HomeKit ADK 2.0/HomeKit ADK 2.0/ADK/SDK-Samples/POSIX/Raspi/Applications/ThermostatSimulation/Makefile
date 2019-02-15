default: app

TARGET = ThermostatSimulation

# System info
$(info Platform: $(shell uname -srvm))

# Supported options: OpenWRT/uclibc
PLATFORM: PLATFORM ?= NONE
UNAME_S = $(shell uname -s)
UNAME_M = $(shell uname -m)

debug: HAP_LOG_LEVEL ?= 3
default: HAP_LOG_LEVEL ?= 0

app: CFLAGS += \
	-DHAP_LOG_LEVEL=$(HAP_LOG_LEVEL)

ifeq ($(USE_DISPLAY), 1)
	CFLAGS += -DHAVE_DISPLAY=1
endif

ifeq ($(USE_WAC), 1)
	CFLAGS += -DHAVE_WAC=1
endif

ifeq ($(UNAME_S), Darwin)
	LDFLAGS += \
		-L../../HAP/lib/x86_64/Clang/x86_64-apple-darwin17.4.0

	SRC += \
		../../PAL/HAPPlatformServiceDiscovery.c \
		../../../Xtras/PAL/RandomNumber/POSIX/HAPPlatformRandomNumber.c

	ifeq ($(USE_MFI_HW_AUTH), 1)
		CFLAGS += -DHAVE_MFI_HW_AUTH=1

		SRC += \
			../../Dependencies/hidapi/mac/hid.c \
			../../../Xtras/PAL/MFiHWAuth/MCP2221/HAPPlatformMFiHWAuth.c

		INC += \
			-I../../../Xtras/PAL/MFiHWAuth/MCP2221

		CFLAGS-../../Dependencies/hidapi/mac/hid.o += \
			-Wno-cast-qual \
			-Wno-conversion \
			-Wno-format-pedantic \
			-Wno-shorten-64-to-32 \
			-Wno-sign-compare \
			-Wno-sign-conversion \
			-Wno-unused-parameter

		LDLIBS += -framework IOKit -framework CoreFoundation
	else
		CFLAGS += -DHAVE_MFI_HW_AUTH=0

		SRC += \
			../../../Xtras/PAL/MFiHWAuth/Null/HAPPlatformMFiHWAuth.c

		INC += \
			-I../../../Xtras/PAL/MFiHWAuth/Null
	endif
else
	ifeq ($(USE_MFI_HW_AUTH), 0)
		CFLAGS += -DHAVE_MFI_HW_AUTH=0
	else
		CFLAGS += -DHAVE_MFI_HW_AUTH=1
	endif

	LDFLAGS += -lm

	SRC += \
		../../PAL/HAPPlatformRandomNumber.c

	ifeq ($(UNAME_M), armv7l)
		LDFLAGS += \
			-L../../HAP/lib/ARM/GCC/arm-unknown-linux-gnueabihf/armv7-a
	else ifeq ($(UNAME_M), armv6l)
		LDFLAGS += \
			-L../../HAP/lib/ARM/GCC/arm-unknown-linux-gnueabihf/armv6
	else ifeq ($(UNAME_M), armv5tejl)
		LDFLAGS += \
			-L../../HAP/lib/ARM/GCC/arm-unknown-linux-gnueabi/armv5te
	else ifeq ($(UNAME_M), mips)
		LDFLAGS += \
			-L../../HAP/lib/MIPS/GCC/mipsel-multilib-linux-gnu
	else ifeq ($(UNAME_M), x86_64)
		LDFLAGS += \
			-L../../HAP/lib/x86_64/GCC/x86_64-unknown-linux-gnu
	else
		$(error System not recognized.)
	endif

	ifeq ($(I2CLIB), MCP2221)
		SRC += \
			../../Dependencies/hidapi/libusb/hid.c

		CFLAGS-../../Dependencies/hidapi/libusb/hid.o += \
			-Wno-clobbered \
			-Wno-unused-parameter

		INC += \
			-I/usr/include/libusb-1.0

		LDLIBS += -lusb-1.0

		SRC += \
			../../../Xtras/PAL/MFiHWAuth/MCP2221/HAPPlatformMFiHWAuth.c
		INC += \
			-I../../../Xtras/PAL/MFiHWAuth/MCP2221
	else
		SRC += \
			../../PAL/HAPPlatformMFiHWAuth.c
	endif

	ifeq ($(SDLIB), Avahi)
		SRC += \
			../../../Xtras/PAL/ServiceDiscovery/Avahi/HAPPlatformServiceDiscovery.c

		INC += \
			-I../../../Xtras/PAL/ServiceDiscovery/Avahi

		LDLIBS += -lavahi-common -lavahi-client
	else
		SRC += \
			../../PAL/HAPPlatformServiceDiscovery.c

		LDLIBS += -ldns_sd
	endif

	ifeq ($(USE_NFC), 1)
		LDLIBS += -lnfc
		CFLAGS += -DHAVE_NFC=1
	endif
endif

SRC += \
	App.c \
	DB.c \
	Main.c \
	../../../Xtras/PAL/Camera/Null/HAPPlatformCamera.c \
	../../PAL/HAPPlatform.c \
	../../PAL/HAPPlatformAbort.c \
	../../PAL/HAPPlatformAccessorySetup.c \
	../../PAL/HAPPlatformBLEPeripheralManager.c \
	../../PAL/HAPPlatformClock.c \
	../../PAL/HAPPlatformFileManager.c \
	../../PAL/HAPPlatformKeyValueStore.c \
	../../PAL/HAPPlatformLog.c \
	../../PAL/HAPPlatformMFiTokenAuth.c \
	../../PAL/HAPPlatformRunLoop.c \
	../../PAL/HAPPlatformSoftwareAccessPoint.c \
	../../PAL/HAPPlatformTCPStreamManager.c \
	../../PAL/HAPPlatformWiFiManager.c

INC += \
	-I./ \
	-I../ \
	-I../../HAP/include \
	-I../../PAL \
	-I../../PAL/include \
	-I../../Dependencies/hidapi \
	-I../../Dependencies/hidapi/hidapi

LDLIBS += \
	-lhap \
	-lpthread

OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
-include $(DEP)

CFLAGS += -std=c99 -pedantic -pedantic-errors -Werror -g0 -Os \
	-fno-strict-overflow -fno-strict-aliasing \
	-Wall -Wextra \
	-pthread \
	$(INC)

ifeq ($(findstring clang, $(shell gcc --version)), clang)
	CFLAGS += \
		-Weverything \
		-Wno-documentation \
		-Wno-documentation-unknown-command \
		-Wno-gcc-compat \
		-Wno-missing-noreturn \
		-Wno-nullability-extension \
		-Wno-padded \
		-Wno-unknown-warning-option \
		-Wno-unreachable-code-break \
		-fmacro-backtrace-limit=0

	LDFLAGS += -Wl,-dead_strip
else
	CFLAGS += \
		-fno-delete-null-pointer-checks

	LDFLAGS += -Wl,--gc-sections
endif

ifeq ($(UNAME_S), Linux)
	CFLAGS += -D_POSIX_C_SOURCE=200112 -D_DEFAULT_SOURCE
endif
ifeq ($(PLATFORM), OpenWRT/uclibc)
	LDLIBS += -lc -lm -lrt -ldl
	CFLAGS += -D_ATFILE_SOURCE=1 -D_BSD_SOURCE -D_POSIX_FSYNC -D_SVID_SOURCE -D_XOPEN_SOURCE=700
endif

CFLAGS += $(CFLAGS-$@)

CPPFLAGS += -MMD -MP

debug: CFLAGS += -DDEBUG
debug: app

app: $(OBJ)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)

.PHONY: default debug clean
