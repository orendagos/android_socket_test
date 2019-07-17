
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# if $(PLATFORM_SDK_VERSION) is android8.0 (26) or higher, enable vendor img
local_silead_support_vendor := $(shell if [ $(PLATFORM_SDK_VERSION) -ge 26 ]; then echo yes; else echo no; fi)
ifeq ($(strip $(local_silead_support_vendor)),yes)
    LOCAL_VENDOR_MODULE := true
endif

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := orenda_server
LOCAL_MODULE_OWNER := orenda

build_date := $(shell date +%y%m%d%H%M%S)
LOCAL_CFLAGS_BASE := -DBUILD_DATE="\"$(build_date)\""

LOCAL_SRC_FILES := orenda_server.c
LOCAL_CFLAGS := $(LOCAL_CFLAGS_BASE)
LOCAL_CFLAGS += -DSIL_CODE_COMPATIBLE
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libs
#                    $(LOCAL_PATH)/include
                    
LOCAL_SHARED_LIBRARIES := libdl libcutils liblog libutils libion orenda_socket 


include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

# if $(PLATFORM_SDK_VERSION) is android8.0 (26) or higher, enable vendor img
local_silead_support_vendor := $(shell if [ $(PLATFORM_SDK_VERSION) -ge 26 ]; then echo yes; else echo no; fi)
ifeq ($(strip $(local_silead_support_vendor)),yes)
    LOCAL_VENDOR_MODULE := true
endif

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := orenda_client
LOCAL_MODULE_OWNER := orenda

build_date := $(shell date +%y%m%d%H%M%S)
LOCAL_CFLAGS_BASE := -DBUILD_DATE="\"$(build_date)\""

LOCAL_SRC_FILES := orenda_client.c
LOCAL_CFLAGS := $(LOCAL_CFLAGS_BASE)
LOCAL_CFLAGS += -DSIL_CODE_COMPATIBLE
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libs
#                    $(LOCAL_PATH)/include
                    
LOCAL_SHARED_LIBRARIES := libdl libcutils liblog libutils libion orenda_socket 


include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under, $(LOCAL_PATH))
