LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := YIM
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libyim.so \

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := YIMM
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libmsc.so \

include $(PREBUILT_SHARED_LIBRARY)


