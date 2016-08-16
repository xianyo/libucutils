LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libucutils_static_prebuild
#LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libucutils.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE :=libucutils_static_prebuild
#LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libucutils.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
#LOCAL_MODULE_CLASS:=STATIC_LIBRARIES
#LOCAL_MODULE_SUFFIX:=.a
#include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := libucutils_prebuild
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libucutils.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)