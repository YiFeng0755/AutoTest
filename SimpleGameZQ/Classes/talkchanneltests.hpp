//
//  talkchanneltests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#ifndef talksettingsuit_hpp
#define talksettingsuit_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"
#include "talkservice.hpp"

DEFINE_TEST_SUITE(TalkChannelTests);

class TalkChannelTest : public TestCase
{
public:
    CREATE_FUNC(TalkChannelTest);
    TalkChannelTest();
    ~TalkChannelTest();


    //callback
    virtual void onEnter() override { TestCase::onEnter(); }
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
private:
    cocos2d::Label* _label;
};

class TalkChannelTestBase : public TestCase
{
public:
    virtual std::string title() const override {
        return "Talk Test";
    }
    //virtual void onEnter() override;
};

class JoinChannelTest : public TalkChannelTestBase
{
public:
    JoinChannelTest();
    ~JoinChannelTest();
    virtual std::string subtitle() const override {
        return "Join a single channel Test";
    }
    //virtual void onEnter() override;
    CREATE_FUNC(JoinChannelTest);
    virtual void onEnter() override;
};

class LeaveChannelTest : public TalkChannelTestBase
{
public:
    CREATE_FUNC(LeaveChannelTest);
    virtual std::string subtitle() const override {
        return "Leave a single channel Test";
    }
    virtual void onEnter() override;

};

class JoinChannelMultiModeTest : public TalkChannelTestBase
{
public:
    CREATE_FUNC(JoinChannelMultiModeTest);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Join multi channel Test";
    }
};

class GetChannelUserlistTest : public TalkChannelTestBase
{
public:
    CREATE_FUNC(GetChannelUserlistTest);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Get channel User List Test";
    }
};

class LeaveChannelMultiModeTest: public TalkChannelTestBase
{
public:
    CREATE_FUNC(LeaveChannelMultiModeTest);
    virtual std::string subtitle() const override {
        return "Leave a channel at multi mode Test";
    }
    virtual void onEnter() override;
};

class ChooseAChannelToSpeak : public TalkChannelTestBase
{
public:
    CREATE_FUNC(ChooseAChannelToSpeak);
    virtual std::string subtitle() const override {
        return "Choose A Channel To Speak";
    }
    virtual void onEnter() override;
private:
    //cocos2d::Label _label;
};

class JoinMutiChannelTest : public TalkChannelTestBase
{
public:
    CREATE_FUNC(JoinMutiChannelTest);
    virtual std::string subtitle() const override {
        return "Join Muti Channel Test";
    }
    virtual void onEnter() override;
private:
    //cocos2d::Label _label;
};

class JoinMutiChannelExceptTest: public TalkChannelTestBase
{
public:
    CREATE_FUNC(JoinMutiChannelExceptTest);
    virtual std::string subtitle() const override {
        return "Join Multi Channel Except Test";
    }
    virtual std::string title() const override {
        return "Except Test";
    }
    virtual void onEnter() override;
};

class JoinSingleChannelExceptTest: public TalkChannelTestBase
{
public:
    JoinSingleChannelExceptTest();
    CREATE_FUNC(JoinSingleChannelExceptTest);
    virtual std::string subtitle() const override {
        return "Join Single Channel Except Test";
    }
    virtual std::string title() const override {
        return "Except Test";
    }
    virtual void onEnter() override;
};

class LeaveChannelExceptTest : public TalkChannelTestBase
{
public:
    CREATE_FUNC(LeaveChannelExceptTest);
    virtual std::string subtitle() const override {
        return "Leave Channel Except Test";
    }
    virtual std::string title() const override {
        return "Except Test";
    }
    virtual void onEnter() override;
};

class LeaveChannelAllExceptTest : public TalkChannelTestBase
{
public:
    CREATE_FUNC(LeaveChannelAllExceptTest);
    virtual std::string subtitle() const override {
        return "Leave Channel Except Test";
    }
    virtual std::string title() const override {
        return "Except Test";
    }
    virtual void onEnter() override;
};

class JoinSingleChannelExceptTest2:
    public TalkChannelTestBase,
    public IYouMeEventCallback
{
public:
    JoinSingleChannelExceptTest2();
    ~JoinSingleChannelExceptTest2();
    CREATE_FUNC(JoinSingleChannelExceptTest2);
    virtual std::string subtitle() const override {
        return "Join Single Channel Except Test2 perf:";
    }
    virtual std::string title() const override {
        return "Except Test";
    }
    virtual void onEnter() override;
    virtual void onExit() override;

    void onEvent(const YouMeEvent event, const YouMeErrorCode error,
                 const char *channel, const char * param) override;

    //执行一千次加入房间的操作
    void exceute();
    void startCallback(Ref* node);
private:
    int _success_num;
    int _failed_num;

    cocos2d::Label *_label_success;
    cocos2d::Label *_label_faled;
    cocos2d::Label *_label_info;
    cocos2d::ui::Button *_btn_begin;

    bool _stop;
};
#endif /* talksettingsuit_hpp */
