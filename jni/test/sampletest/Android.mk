LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := sample
LOCAL_SRC_FILES := sample.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := libucutils
#LOCAL_CFLAGS += -pie -fPIE 
#LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)
