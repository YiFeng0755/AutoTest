//
//  talkinittest.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/25.
//
//

#include "talkinittest.hpp"
#include "talkservice.hpp"
#include "cocos2d.h"


USING_NS_CC;
using namespace ui;

TalkInitTests::TalkInitTests()
{
    ADD_TEST_CASE(InitTest);
    ADD_TEST_CASE(UnInitTest);
    ADD_TEST_CASE(JoinLeavRoomTest);
};

InitTest::InitTest()
{
    IYouMeVoiceEngine::getInstance()->unInit();
}

InitTest::~InitTest()
{

}

bool InitTest::init()
{
    if (!TestCase::init())
        return false;

    _label = cocos2d::Label::createWithTTF("Please click button to begin test", "fonts/arial.ttf", 10.0);
    _label->setPosition(VisibleRect::center());

    _btn_test = cocos2d::ui::Button::create();
    _btn_test->setPosition(cocos2d::Vec2(VisibleRect::center().x, VisibleRect::center().y - 20));
    _btn_test->setTitleText("Begin to Test");
    _btn_test->addTouchEventListener(CC_CALLBACK_0(InitTest::start, this));

    addChild(_label);
    addChild(_btn_test);
    return true;
}

void InitTest::onEvent(const YouMeEvent event, const YouMeErrorCode error, const char *channel, const char *param)
{

    if (event == YOUME_EVENT_INIT_OK) {
        _label->setString("Init OK!");
    } else if (event == YOUME_EVENT_INIT_FAILED) {
        char outputstr[128];
        sprintf(outputstr, "Init failed, The errorcode is %d", error);
        _label->setString(outputstr);
    }
}

void InitTest::start()
{
    YouMeErrorCode errcode1 = IYouMeVoiceEngine::getInstance()->init(this,
                                                                    TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");
    if (errcode1 == YOUME_SUCCESS) {
        cocos2d::log("Init success");
        _label->setString("Call init success!");
    } else {
        char buffer[128];
        sprintf(buffer, "Failed errorcode is %d", errcode1);
        _label->setString(buffer);
    }
}

//初始化和反初始化 测试稳定性
UnInitTest::UnInitTest()
    : m_case1_stop(true), m_case2_stop(true)
{
    _success_stat = 0;
    _failed_stat = 0;

    IYouMeVoiceEngine::getInstance()->unInit();
    _label = cocos2d::Label::createWithTTF("Please click button to begin test", "fonts/arial.ttf", 10.0);
    _label->setPosition(VisibleRect::center());

    btn_case1_test = cocos2d::ui::Button::create();
    btn_case1_test->setPosition(cocos2d::Vec2(VisibleRect::center().x - 25, VisibleRect::center().y - 20));
    btn_case1_test->setTitleText("Uinit");
    btn_case1_test->addTouchEventListener(CC_CALLBACK_0(UnInitTest::case1_test, this));

    btn_case2_test = cocos2d::ui::Button::create();
    btn_case2_test->setPosition(cocos2d::Vec2(VisibleRect::center().x + 25, VisibleRect::center().y - 20));
    btn_case2_test->setTitleText("Init & Uninit");
    btn_case2_test->addTouchEventListener(CC_CALLBACK_0(UnInitTest::case2_test, this));

    btn_case1_stop = cocos2d::ui::Button::create();
    btn_case1_stop->setPosition(cocos2d::Vec2(VisibleRect::center().x - 25, VisibleRect::center().y - 40));
    btn_case1_stop->setTitleText("Stop 1");
    btn_case1_stop->addTouchEventListener(CC_CALLBACK_0(UnInitTest::case1_stop, this));

    btn_case2_stop = cocos2d::ui::Button::create();
    btn_case2_stop->setPosition(cocos2d::Vec2(VisibleRect::center().x + 25, VisibleRect::center().y - 40));
    btn_case2_stop->setTitleText("Stop 2");
    btn_case2_stop->addTouchEventListener(CC_CALLBACK_0(UnInitTest::case2_stop, this));

    addChild(_label);
    addChild(btn_case1_test);
    addChild(btn_case2_test);
    addChild(btn_case1_stop);
    addChild(btn_case2_stop);
}

void UnInitTest::case1_test()
{
    _success_stat = 0;
    _failed_stat = 0;
    m_case1_stop = false;
    btn_case1_test->setEnabled(false);
    std::thread t([this]() {
        while (true) {
            if (m_case1_stop)
                break;
            IYouMeVoiceEngine::getInstance()->unInit();
        }
    });
    t.detach();
}

void UnInitTest::case2_test()
{
    _success_stat = 0;
    _failed_stat = 0;
    m_case2_stop = false;
    btn_case2_test->setEnabled(false);
    std::thread t([this]() {
        while (true) {
            IYouMeVoiceEngine::getInstance()->unInit();
            if (m_case2_stop)
                break;
            IYouMeVoiceEngine::getInstance()->init(this, TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });
    t.detach();
}

void UnInitTest::onEvent(const YouMeEvent event, const YouMeErrorCode error, const char *channel, const char *param)
{
    if (event == YOUME_EVENT_INIT_OK) {
        ++_success_stat;
    } else if (event == YOUME_EVENT_INIT_FAILED) {
        ++_failed_stat;
    }
}

void UnInitTest::case1_stop()
{
    btn_case1_test->setEnabled(true);
    m_case1_stop = true;
   // char buffer[128];
   // sprintf(buffer, "success:%d;failed:%d", int(_success_stat), int(_failed_stat));
    _label->setString("Thread 1 is stopped.");
}

void UnInitTest::case2_stop()
{
    btn_case2_test->setEnabled(true);
    m_case2_stop = true;
    char buffer[128];
    sprintf(buffer, "Thread 2 stopped.success:%d;failed:%d", int(_success_stat), int(_failed_stat));
    _label->setString(buffer);
}


// Join Leave

JoinLeavRoomTest::JoinLeavRoomTest()
: m_case1_stop(false)
{
    _success_stat = 0;
    _failed_stat = 0;

    _label = cocos2d::Label::createWithTTF("Please click button to begin test", "fonts/arial.ttf", 20.0);
    _label->setPosition(VisibleRect::center());

    btn_start = cocos2d::ui::Button::create();
    btn_start->setPosition(cocos2d::Vec2(VisibleRect::center().x - 25, VisibleRect::center().y - 30));
    btn_start->setTitleText("Start");
    btn_start->addTouchEventListener(CC_CALLBACK_0(JoinLeavRoomTest::start, this));

    btn_stop = cocos2d::ui::Button::create();
    btn_stop->setPosition(cocos2d::Vec2(VisibleRect::center().x + 25, VisibleRect::center().y - 30));
    btn_stop->setTitleText("Stop");
    btn_stop->addTouchEventListener(CC_CALLBACK_0(JoinLeavRoomTest::stop, this));

//    btn_start = cocos2d::ui::Button::create();
//    btn_case1_stop->setPosition(cocos2d::Vec2(VisibleRect::center().x - 25, VisibleRect::center().y - 40));
//    btn_case1_stop->setTitleText("Stop 1");
//    btn_case1_stop->addTouchEventListener(CC_CALLBACK_0(UnInitTest::case1_stop, this));
//
//    btn_case2_stop = cocos2d::ui::Button::create();
//    btn_case2_stop->setPosition(cocos2d::Vec2(VisibleRect::center().x + 25, VisibleRect::center().y - 40));
//    btn_case2_stop->setTitleText("Stop 2");
//    btn_case2_stop->addTouchEventListener(CC_CALLBACK_0(UnInitTest::case2_stop, this));

    addChild(_label);
    addChild(btn_start);
    addChild(btn_stop);
   // addChild(btn_case1_stop);
   // addChild(btn_case2_stop);
}

bool JoinLeavRoomTest::init()
{
    if (!TestCase::init())
        return false;
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine::getInstance()->init(this,
                                           TALK_APPKEY,
                                           TALK_APPSECRET,
                                           RTC_CN_SERVER, "");
    
    _label = cocos2d::Label::createWithTTF("Please click button to begin test", "fonts/arial.ttf", 15.0);
    _label->setPosition(Vec2(VisibleRect::bottom().x, VisibleRect::bottom().y + 35 ));

    addChild(_label);
    return true;
}

void JoinLeavRoomTest::stop()
{
    m_case1_stop = true;
    // char buffer[128];
    // sprintf(buffer, "success:%d;failed:%d", int(_success_stat), int(_failed_stat));
   // _label->setString("Thread is stopped.");
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    char buffer[128];
    sprintf(buffer, "Thread 2 stopped.success:%d;failed:%d", int(_success_stat), int(_failed_stat));
    _label->setString(buffer);
}

void JoinLeavRoomTest::start()
{
    _success_stat = 0;
    _failed_stat = 0;
    m_case1_stop = false;
    _label->setString("Running ....");
    std::thread t([this]() {
        while (true) {
            if (m_case1_stop)
                break;
            std::unique_lock<std::mutex> lk(m_mutex);
            IYouMeVoiceEngine::getInstance()->joinChannelSingleMode("TestUser", TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
            cv.wait_for(lk, std::chrono::seconds(10));
            //std::this_thread::sleep_for(std::chrono::seconds(3));

            lk.unlock();
            std::unique_lock<std::mutex> lk2(m_mutex);
            IYouMeVoiceEngine::getInstance()->leaveChannelAll();
            cv.wait_for(lk2, std::chrono::seconds(10));
        }
    });
    t.detach();

}

void JoinLeavRoomTest::onEvent(const YouMeEvent event, const YouMeErrorCode error, const char *channel, const char *param)
{
    if (event == YOUME_EVENT_JOIN_OK) {
        ++_success_stat;
        cv.notify_one();
    } else if (event == YOUME_EVENT_JOIN_FAILED) {
        ++_failed_stat;
        cv.notify_one();
    } else if (event == YOUME_EVENT_LEAVED_ALL) {
        cv.notify_one();
    }
}


