LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := YouMeCommon_static
LOCAL_MODULE_FILENAME := YouMeCommon
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libYouMeCommon.a

include $(PREBUILT_STATIC_LIBRARY)
