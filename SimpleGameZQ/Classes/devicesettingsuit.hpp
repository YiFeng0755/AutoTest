//
//  devicesettingsuit.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#ifndef devicesettingsuit_hpp
#define devicesettingsuit_hpp

//设备状态管理相关的用例
#include "testbase/BaseTest.h"
#include "cocos2d.h"

DEFINE_TEST_SUITE(DeviceManageerTests);

static const char *users[] = {"XXX1000", "xxx1001", "xxx1002"};
static const char *rooms[] = {"1111234", "1111235", "1111236"};

class IYouMeVoiceEngine;
class DeviceBase : public TestCase
{
public:
    virtual std::string title() const override {
        return "Device setting test";
    }

    virtual void onEnter() override;

protected:
    IYouMeVoiceEngine* _engine;
};

//设置 获取扬声器状态
//包含接口 : SetSpeakerMute & GetSpeakerMute
class SpeakerMuteTest : public DeviceBase
{
public:
    CREATE_FUNC(SpeakerMuteTest);
    virtual std::string subtitle() const override {
        return "Speak mute test";
    }
    virtual void onEnter() override;
};

//设置、获取麦克风状态
class MicroPhoneMuteTest : public DeviceBase
{
public:
    CREATE_FUNC(MicroPhoneMuteTest);
    virtual std::string subtitle() const override {
        return "Micro Phone Mute Test";
    }
    virtual void onEnter() override;
};

//音量设置
class VolumeTest : public DeviceBase
{
public:
    CREATE_FUNC(VolumeTest);
    virtual std::string subtitle() const override {
        return "Volume Test";
    }
    virtual void onEnter() override;

};

//设置是否通知他人 扬声器和麦克风的开关
//接口： SetAutoSendStatus
class AutoSendStatusTest : public DeviceBase
{
public:
    CREATE_FUNC(AutoSendStatusTest);
    virtual std::string subtitle() const override {
        return "Auto Send Status Test";
    }
    virtual void onEnter() override;
};

//控制别人的麦克风
class CtrlWhosMicroPhoneTest : public DeviceBase
{
public:
    CREATE_FUNC(CtrlWhosMicroPhoneTest);
    virtual std::string subtitle() const override {
        return "Ctrl Who‘s MicroPhone Test";
    }
    virtual void onEnter() override;
};

//控制别人的扬声器
class CtrlWhosSpeakerMuteTest : public DeviceBase
{
public:
    CREATE_FUNC(CtrlWhosSpeakerMuteTest);
    virtual std::string subtitle() const override {
        return "Ctrl Who’s Speaker Mute Test";
    }
    virtual void onEnter() override;
};

//设置是否听某个人的语音
//SetListenOtherVoice
class SetListeneWhosVoiceTest : public DeviceBase
{
public:
    CREATE_FUNC(SetListeneWhosVoiceTest);
    virtual std::string subtitle() const override {
        return "Set Listen Who‘s Voice Test";
    }
    virtual void onEnter() override;
};

//设置语音检测回调
//SetVadCallbackEnabled
class SetVadCallbackEnabledTest : public DeviceBase
{
public:
    CREATE_FUNC(SetVadCallbackEnabledTest);
    virtual std::string subtitle() const override {
        return "Set Vad Callback Enabled Test";
    }
    virtual void onEnter() override;
};

//设置语音监听 ：插耳机的情况下的开启或关闭语音监听
//SetHeadsetMonitorOn(bool b)
class SetHeadsetMonitorOnTest : public DeviceBase
{
public:
    CREATE_FUNC(SetHeadsetMonitorOnTest);
    virtual std::string subtitle() const override {
        return "Set Headset Monitor On Test";
    }
    virtual void onEnter() override;
};

//混响音效设置
//SetReverbEnabled
class SetReverbEnabledTest : public DeviceBase
{
public:
    CREATE_FUNC(SetReverbEnabledTest);
    virtual std::string subtitle() const override {
        return "Set Reverb Enabled Test";
    }
    virtual void onEnter() override;
};
#endif /* devicesettingsuit_hpp */
