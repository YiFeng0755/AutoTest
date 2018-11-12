//
//  channeltest.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#ifndef channeltest_hpp
#define channeltest_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(ChannelTests);

class LoginTest : public TestCase
{
public:
    LoginTest();
    ~LoginTest();
    CREATE_FUNC(LoginTest);

    virtual void onEnter() override;
    virtual std::string subtitle() const override{
        return "Login Test";
    }
    virtual std::string title() const override {
        return "Channel Test";
    }
};

class ChannelTest : public TestCase
{
public:
    ChannelTest();
    ~ChannelTest();

    CREATE_FUNC(ChannelTest);
    virtual std::string title() const override {
        return "Channel Test";
    }
    virtual std::string subtitle() const override {
        return "Channel Enter Test";
    }
    void onEnter() override;
    void onEnterRoom(cocos2d::Ref* psender);
    void onLeaveRoom(cocos2d::Ref *pSender);
private:
    cocos2d::Label * _label;
};


class ChannelLeaveTest : public TestCase
{
public:
    ChannelLeaveTest();
    ~ChannelLeaveTest();
    CREATE_FUNC(ChannelLeaveTest);

    virtual std::string title() const override {
        return "Channel  Test";
    }

    virtual std::string subtitle() const override {
        return "Channel Leave Test";
    }

private:
    
};

#endif /* channeltest_hpp */
