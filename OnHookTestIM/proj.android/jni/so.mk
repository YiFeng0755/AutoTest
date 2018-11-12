LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libnlscppsdk
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libnlscppsdk.so \

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libuscasr
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libuscasr.so \

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libuuid
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libuuid.so \

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := YIM
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libyim.so \

include $(PREBUILT_SHARED_LIBRARY)

LOCAL_MODULE := libmsc
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libmsc.so \

include $(PREBUILT_SHARED_LIBRARY)

