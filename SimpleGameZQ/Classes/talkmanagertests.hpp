//
//  talkmanagertests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/19.
//
//

#ifndef talkmanagertests_hpp
#define talkmanagertests_hpp

#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(TalkManagerTests);

class TalkManagerTestBase : public TestCase
{
public:
    virtual std::string title() const override {
        return "Talk manger test";
    }

    virtual void onEnter() override;
    TalkManagerTestBase();
};

//暂停通话
class PauseChannelTest : public TalkManagerTestBase
{
public:
    CREATE_FUNC(PauseChannelTest);
    virtual std::string subtitle() const override {
        return "Pause channel test";
    }
    virtual void onEnter() override;
};

//恢复通话
class ResumeChannelTest : public TalkManagerTestBase
{
public:
    CREATE_FUNC(ResumeChannelTest);
    virtual std::string subtitle() const override {
        return "Resume Channel Test";
    }
    virtual void onEnter() override;
};

//设置语音检测
class MuteCheckTest : public TalkManagerTestBase
{
public:
    CREATE_FUNC(MuteCheckTest);
    virtual std::string subtitle() const override {
        return "Mute Check Test";
    }
    virtual void onEnter() override;
};

//播放背景音乐
class PlayBackgroundMusicTest : public TalkManagerTestBase
{
public:
    CREATE_FUNC(PlayBackgroundMusicTest);
    virtual std::string subtitle() const override {
        return "Play Background Music Test";
    }
    virtual void onEnter() override;
};

//停止播放背景音乐
class StopBackgroundMusicTest : public TalkManagerTestBase
{
public:
    CREATE_FUNC(StopBackgroundMusicTest);
    virtual std::string subtitle() const override {
        return "Stop Background Music Test";
    }
    virtual void onEnter() override;
};

//设置背景音乐音量
class BackgroundMusicVolumeSettingTest :  public TalkManagerTestBase
{
public:
    CREATE_FUNC(BackgroundMusicVolumeSettingTest);
    virtual std::string subtitle() const override {
        return "Background Music Volume Setting Test";
    }
    virtual void onEnter() override;
};
#endif /* talkmanagertests_hpp */
