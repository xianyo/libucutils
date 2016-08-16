LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libsamplemodule
LOCAL_SRC_FILES := samplemodule.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := libucutils
include $(BUILD_SHARED_LIBRARY)
