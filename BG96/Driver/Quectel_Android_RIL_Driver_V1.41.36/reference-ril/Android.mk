# Copyright 2006 The Android Open Source Project

# XXX using libutils for simulator build only...
#
USE_MUXD=1
USE_NDIS=1
LOCAL_PATH:= $(call my-dir)
$(shell touch $(LOCAL_PATH)/*)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    reference-ril.c \
    atchannel.c \
    misc.c \
    at_tok.c \
    ql-pppd.c \
    ql-gps.c \
    ql-datafwd.c \
    ql-dtmf.c \
    ql-tty2tcp.c \
    ql-usb.c \
    ql-conf.c

ifneq ($(wildcard ql-buildinfo.c),)
LOCAL_SRC_FILES += ql-buildinfo.c
LOCAL_CFLAGS += -DUSE_GIT
endif

ifeq ($(USE_NDK),1)
LOCAL_CFLAGS += -DUSE_NDK
LOCAL_LDLIBS += -llog
LOCAL_SRC_FILES += ql-ndk.c
else
LOCAL_SHARED_LIBRARIES := \
    libcutils libutils libril libnetutils
endif

# for asprinf
LOCAL_CFLAGS += -D_GNU_SOURCE

#LOCAL_C_INCLUDES := $(KERNEL_HEADERS)

ifeq ($(TARGET_DEVICE),sooner)
  LOCAL_CFLAGS += -DOMAP_CSMI_POWER_CONTROL -DUSE_TI_COMMANDS
endif

ifeq ($(TARGET_DEVICE),surf)
  LOCAL_CFLAGS += -DPOLL_CALL_STATE -DUSE_QMI
endif

ifeq ($(TARGET_DEVICE),dream)
  LOCAL_CFLAGS += -DPOLL_CALL_STATE -DUSE_QMI
endif

LOCAL_CFLAGS += -Wno-unused-parameter

ifeq ($(USE_MUXD),1)
LOCAL_CFLAGS += -DMUX_ANDROID -DUSE_MUXD
$(shell touch $(LOCAL_PATH)/CMUX/*)
LOCAL_SRC_FILES += CMUX/gsm0710muxd_bp.c
endif
ifeq ($(USE_NDIS),1)
LOCAL_CFLAGS += -DANDROID -DUSE_NDIS
$(shell touch $(LOCAL_PATH)/quectel-CM/*)
LOCAL_SRC_FILES += ql-ndis.c quectel-CM/GobiNetCM.c quectel-CM/main.c quectel-CM/MPQMUX.c quectel-CM/QMIThread.c quectel-CM/QmiWwanCM.c quectel-CM/udhcpc.c quectel-CM/util.c
endif

ifeq (foo,foo)
  #build shared library
  LOCAL_CFLAGS += -DRIL_SHLIB
  LOCAL_MODULE:= libreference-ril
  include $(BUILD_SHARED_LIBRARY)
else
  #build executable
  LOCAL_SHARED_LIBRARIES += \
      libril
  LOCAL_MODULE:= reference-ril
  include $(BUILD_EXECUTABLE)
endif

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= chat.c
LOCAL_CFLAGS += -Wno-unused-parameter -Wno-sign-compare
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
ifeq ($(USE_NDK),1)
LOCAL_CFLAGS += -DUSE_NDK
LOCAL_LDLIBS += -llog
else
LOCAL_SHARED_LIBRARIES := libcutils libutils
endif
LOCAL_MODULE_TAGS:=eng optional
LOCAL_MODULE:= chat
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= ip-up.c
ifeq ($(USE_NDK),1)
LOCAL_CFLAGS += -DUSE_NDK
LOCAL_LDLIBS += -llog
else
LOCAL_SHARED_LIBRARIES += libcutils libutils
endif
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_MODULE_TAGS:=eng optional
LOCAL_MODULE_PATH:= $(TARGET_OUT_ETC)/ppp
LOCAL_MODULE:= ip-up
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= ip-down.c
ifeq ($(USE_NDK),1)
LOCAL_CFLAGS += -DUSE_NDK
LOCAL_LDLIBS += -llog
else
LOCAL_SHARED_LIBRARIES += libcutils libutils
endif
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_MODULE_TAGS:=eng optional
LOCAL_MODULE_PATH:= $(TARGET_OUT_ETC)/ppp
LOCAL_MODULE:= ip-down
include $(BUILD_EXECUTABLE)
