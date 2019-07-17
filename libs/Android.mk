
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# if $(PLATFORM_SDK_VERSION) is android8.0 (26) or higher, enable vendor img
local_silead_support_vendor := $(shell if [ $(PLATFORM_SDK_VERSION) -ge 26 ]; then echo yes; else echo no; fi)
ifeq ($(strip $(local_silead_support_vendor)),yes)
    LOCAL_VENDOR_MODULE := true
endif

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := orenda_socket
LOCAL_MODULE_OWNER := orenda

build_date := $(shell date +%y%m%d%H%M%S)
LOCAL_CFLAGS_BASE := -DBUILD_DATE="\"$(build_date)\""

LOCAL_SRC_FILES := orenda_socket_client.c orenda_socket_server.c
LOCAL_CFLAGS := $(LOCAL_CFLAGS_BASE)
LOCAL_CFLAGS += -DSIL_CODE_COMPATIBLE
# LOCAL_C_INCLUDES += orenda_socket.h
#                    $(LOCAL_PATH)/include
                    
LOCAL_SHARED_LIBRARIES := libdl libcutils liblog libutils libion


include $(BUILD_SHARED_LIBRARY)