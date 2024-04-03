LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := supernova

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
					$(LOCAL_PATH)/../headers

LOCAL_CFLAGS :=

LOCAL_SRC_FILES := $(LOCAL_PATH)/src/engine.cpp

LOCAL_SHARED_LIBRARIES := SDL3 SDL3_image SDL3_mixer SDL3_ttf SDL3_net

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)/src

include $(BUILD_SHARED_LIBRARY)