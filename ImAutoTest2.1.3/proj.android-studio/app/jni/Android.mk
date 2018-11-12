LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

APP_PLATFORM := android-19


LOCAL_MODULE := bugly_native_prebuilt

LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libBugly.so
include $(PREBUILT_SHARED_LIBRARY)


LOCAL_MODULE := MyGame_shared



LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp\
                   ../../../Classes/AppDelegate.cpp\
                   ../../../Classes/autocontroller.cpp\
                   ../../../Classes/autotestscene.cpp\
                   ../../../Classes/imservice.cpp\
                   ../../../Classes/md5.cpp\
                   ../../../Classes/sha1.cpp\
                   ../../../Classes/imcases.cpp\
                   ../../../Classes/testbase/VisibleRect.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
$(LOCAL_PATH)/../../../yim/include \
$(LOCAL_PATH)/../../../BuglyCocosPlugin/CocosPlugin
	
# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += bugly_crashreport_cocos_static
LOCAL_SHARED_LIBRARIES := YIM

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)
include $(LOCAL_PATH)/../../../yim/lib/android/Android.mk


$(call import-module,.)
# 导入 bugly 静态库目录
$(call import-module,external/bugly)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
