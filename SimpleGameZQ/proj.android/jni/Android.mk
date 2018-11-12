LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

#＝＝＝＝＝＝Youme 添加＝＝＝＝＝＝＝＝＝
LOCAL_MODULE := youme_voice_engine
LOCAL_SRC_FILES := ../$(LOCAL_PATH)/../../youme_voice_engine/lib/android/$(TARGET_ARCH_ABI)/libyoume_voice_engine.so
include $(PREBUILT_SHARED_LIBRARY)
#＝＝＝＝＝＝结束 Youme 添加＝＝＝＝＝＝＝

include $(CLEAR_VARS)

LOCAL_MODULE := bugly_native_prebuilt
# 可在Application.mk添加APP_ABI := armeabi armeabi-v7a 指定集成对应架构的.so文件
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libBugly.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp\
                   ../../Classes/AppDelegate.cpp\
                   ../../Classes/autocontroller.cpp\
                   ../../Classes/autosendrecvmsg.cpp\
                   ../../Classes/autotestscene.cpp\
                   ../../Classes/channeltest.cpp\
                   ../../Classes/devicesettingsuit.cpp\
                   ../../Classes/imautotest.cpp\
                   ../../Classes/imcommontests.cpp\
                   ../../Classes/imservice.cpp\
                   ../../Classes/Imtest.cpp\
                   ../../Classes/imtestcases.cpp\
				   ../../Classes/imcases.cpp\
                   ../../Classes/locationtest.cpp\
                   ../../Classes/md5.cpp\
                   ../../Classes/sha1.cpp\
                   ../../Classes/networksettingtests.cpp\
                   ../../Classes/sendmsgtests.cpp\
                   ../../Classes/sendvoicetests.cpp\
                   ../../Classes/settings.cpp\
                   ../../Classes/talkcases.cpp\
                   ../../Classes/talkchanneltests.cpp\
                   ../../Classes/talkmanagertests.cpp\
                   ../../Classes/talkservice.cpp\
                   ../../Classes/talktests.cpp\
                   ../../Classes/talkinittest.cpp\
                   ../../Classes/zoneSettingTests.cpp\
                   ../../Classes/MainScene.cpp\
                   ../../Classes/testbase/BaseTest.cpp\
                   ../../Classes/testbase/controller.cpp\
                   ../../Classes/testbase/VisibleRect.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../yim/include \
$(LOCAL_PATH)/../../youme_voice_engine/include \
$(LOCAL_PATH)/../../BuglyCocosPlugin/CocosPlugin


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += bugly_crashreport_cocos_static

LOCAL_SHARED_LIBRARIES := YIM
LOCAL_SHARED_LIBRARIES += youme_voice_engine


# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/../../yim/lib/android/Android.mk

$(call import-module,.)
# 导入 bugly 静态库目录
$(call import-module,external/bugly)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
