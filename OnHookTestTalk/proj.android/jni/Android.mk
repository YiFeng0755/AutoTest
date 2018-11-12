LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

#＝＝＝＝＝＝Youme 添加＝＝＝＝＝＝＝＝＝
LOCAL_MODULE := youme_voice_engine
LOCAL_SRC_FILES := ../$(LOCAL_PATH)/../../youme_voice_engine/lib/android/$(TARGET_ARCH_ABI)/libyoume_voice_engine.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

#LOCAL_MODULE := youme_magic_voice
#LOCAL_SRC_FILES := ../$(LOCAL_PATH)/../../youme_voice_engine/lib/android/armeabi-v7a/libYouMeMagicVoice.so
#include $(PREBUILT_SHARED_LIBRARY)

#＝＝＝＝＝＝结束 Youme 添加＝＝＝＝＝＝＝

LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp
LOCAL_MODULE := bugly_native_prebuilt
# 可在Application.mk添加APP_ABI := armeabi armeabi-v7a 指定集成对应架构的.so文件
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libBugly.so 
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared
LOCAL_MODULE_FILENAME := libMyGame
LOCAL_SRC_FILES := hellocpp/main.cpp\
                   ../../Classes/AppDelegate.cpp\
                   ../../Classes/autotestscene.cpp\
                   ../../Classes/talkservice.cpp\
                   ../../Classes/md5.cpp\
                   ../../Classes/sha1.cpp\
                   ../../Classes/talkcases.cpp\
                   ../../Classes/MainScene.cpp \
				   ../../Classes/VisibleRect.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../youme_voice_engine/include \
$(LOCAL_PATH)/../../BuglyCocosPlugin/CocosPlugin


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += bugly_crashreport_cocos_static
LOCAL_SHARED_LIBRARIES := youme_voice_engine

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
# 导入 bugly 静态库目录
$(call import-module,external/bugly)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END