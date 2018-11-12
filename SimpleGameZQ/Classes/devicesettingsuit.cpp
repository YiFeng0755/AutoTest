//
//  devicesettingsuit.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#include "devicesettingsuit.hpp"
#include "talkservice.hpp"
#include "imservice.hpp"
#include <thread>

DeviceManageerTests::DeviceManageerTests()
{
    ADD_TEST_CASE(SpeakerMuteTest);
    ADD_TEST_CASE(MicroPhoneMuteTest);
    ADD_TEST_CASE(VolumeTest);
    ADD_TEST_CASE(AutoSendStatusTest);
    ADD_TEST_CASE(CtrlWhosMicroPhoneTest);
    ADD_TEST_CASE(CtrlWhosSpeakerMuteTest);
    ADD_TEST_CASE(SetListeneWhosVoiceTest);
    ADD_TEST_CASE(SetVadCallbackEnabledTest);
    ADD_TEST_CASE(SetHeadsetMonitorOnTest);
    ADD_TEST_CASE(SetReverbEnabledTest);
}

void DeviceBase::onEnter()
{
    TestCase::onEnter();
    TalkService::getInstance()->init();
    _engine = TalkService::getInstance()->getTalkEngine();

    _engine->joinChannelSingleMode(users[0], rooms[0], YOUME_USER_HOST);
//    int u_size = sizeof(users) / sizeof(users[0]);
//    int r_size = sizeof(rooms) / sizeof(rooms[0]);
//    for (int i = 0; i < u_size; ++i) {
//        for (int j = 0; j < r_size; ++j) {
//            _engine->joinChannelSingleMode(users[i], rooms[j], YOUME_USER_TALKER_FREE);
//        }
//    }
}

//麦克风
void MicroPhoneMuteTest::onEnter()
{
    DeviceBase::onEnter();
    bool b = _engine->getMicrophoneMute();
    if (b) {
        cocos2d::log("Micophone mute 静音");
    } else {
        cocos2d::log("Micophone unmute 没有静音");
    }

    //setiting
    assert(_engine!= nullptr);
    _engine->setMicrophoneMute(true);
    //CC_ASSERT(_engine->getMicrophoneMute() == true);
    _engine->setMicrophoneMute(false);
    //CC_ASSERT(_engine->getMicrophoneMute() == false);
}

//扬声器
void SpeakerMuteTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->setSpeakerMute(true);
    //CC_ASSERT(true == _engine->getSpeakerMute());

    _engine->setSpeakerMute(false);
    //CC_ASSERT(false == _engine->getSpeakerMute());
}

//音量设置
void VolumeTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->setVolume(1);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    _engine->setVolume(10);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    _engine->setVolume(-1);
    cocos2d::log("Finished setting volume");
}

//控制别人的麦克风
void CtrlWhosMicroPhoneTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->setOtherMicMute(TALK_USER_B, false);
}

//控制别人的扬声器
void CtrlWhosSpeakerMuteTest::onEnter()
{
    DeviceBase::onEnter();
	_engine->setOtherSpeakerMute(TALK_USER_B, false);
}

//设置是否听某个人的语音
//SetListenOtherVoice
void SetListeneWhosVoiceTest::onEnter()
{
    DeviceBase::onEnter();
	_engine->setListenOtherVoice(TALK_USER_B, false);
}

//设置语音回调检测
void SetVadCallbackEnabledTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->setVadCallbackEnabled(false);
}

//混响音效设置
//SetReverbEnabled
void SetReverbEnabledTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->joinChannelSingleMode(users[0], rooms[1], YOUME_USER_COMMANDER);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    YouMeErrorCode errcode1 = _engine->setReverbEnabled(true);
    CHECK_RETURN_CODE(errcode1, "Set Reverb success", "Set Reverb failed");

    YouMeErrorCode errcode2 = _engine->setReverbEnabled(false);
    CHECK_RETURN_CODE(errcode2, "Set Reverb success", "Set Reverb failed");
}

void SetHeadsetMonitorOnTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->setHeadsetMonitorOn(true);
    _engine->setHeadsetMonitorOn(false);
}

void AutoSendStatusTest::onEnter()
{
    DeviceBase::onEnter();
    _engine->setAutoSendStatus(true);
    _engine->setAutoSendStatus(false);
}


