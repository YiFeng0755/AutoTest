//
//  talkmanagertests.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/19.
//
//

#include "talkmanagertests.hpp"
#include "talkservice.hpp"
#include "imservice.hpp"


TalkManagerTests::TalkManagerTests()
{
    ADD_TEST_CASE(PauseChannelTest);
    ADD_TEST_CASE(ResumeChannelTest);
    ADD_TEST_CASE(MuteCheckTest);
    ADD_TEST_CASE(PlayBackgroundMusicTest);
    ADD_TEST_CASE(StopBackgroundMusicTest);
    ADD_TEST_CASE(BackgroundMusicVolumeSettingTest);
}

//自由模式
TalkManagerTestBase::TalkManagerTestBase()
{

}

void TalkManagerTestBase::onEnter()
{
    TestCase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    YouMeErrorCode errcode = engine->joinChannelSingleMode(TALK_USER_A, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    if (errcode != YOUME_SUCCESS) {
        cocos2d::log("Init Error!");
        return ;
    }

    engine->joinChannelSingleMode(TALK_USER_B, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    engine->joinChannelSingleMode("TalkUser1000", TALK_CHAT_ROOM, YOUME_USER_LISTENER);

    engine->joinChannelSingleMode("TalkUser1001", TALK_CHAT_ROOM, YOUME_USER_GUSET);
    engine->joinChannelSingleMode("TalkUser1002", TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
}


void PauseChannelTest::onEnter()
{
    TalkManagerTestBase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    YouMeErrorCode errcode = engine->pauseChannel();
    CHECK_RETURN_CODE(errcode, "pauseChannel Success", "pauseChannel Failed");
}

void ResumeChannelTest::onEnter()
{
    TalkManagerTestBase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    YouMeErrorCode errcode = engine->resumeChannel();
    CHECK_RETURN_CODE(errcode, "resumeChannel Success", "resumeChannel Failed");
}

void MuteCheckTest::onEnter()
{
    TalkManagerTestBase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    YouMeErrorCode errcode = engine->setVadCallbackEnabled(true);
    CHECK_RETURN_CODE(errcode, "setVadCallbackEnabled Success", "setVadCallbackEnabled Failed");
    sleep(5);
}

void PlayBackgroundMusicTest::onEnter()
{
    TalkManagerTestBase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    sleep(5);
    std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("nekomimi.mp3");
    YouMeErrorCode errcode = engine->playBackgroundMusic(path.c_str(), false);
    CHECK_RETURN_CODE(errcode, "playBackgroundMusic Success", "playBackgroundMusic Failed");

}

void StopBackgroundMusicTest::onEnter()
{
    TalkManagerTestBase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    YouMeErrorCode errcode = engine->stopBackgroundMusic();
    CHECK_RETURN_CODE(errcode, "stopBackgroundMusic Success", "stopBackgroundMusic Failed");
    sleep(5);
}

void BackgroundMusicVolumeSettingTest::onEnter()
{
    //主播模式背景音乐播放才有效果
    TalkManagerTestBase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    YouMeErrorCode errcode = engine->setBackgroundMusicVolume(50);
    if (errcode == YOUME_SUCCESS)
        sleep(5);
    YouMeErrorCode errcode1 = engine->setBackgroundMusicVolume(75);
    if (errcode1 == YOUME_SUCCESS)
        sleep(5);
    YouMeErrorCode errcode2 = engine->setBackgroundMusicVolume(120);
    if (errcode2 == YOUME_SUCCESS)
        sleep(5);
}

