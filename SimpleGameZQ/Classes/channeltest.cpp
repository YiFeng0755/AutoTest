//
//  channeltest.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//
#include "cocos2d.h"
#include "channeltest.hpp"
#include "ui/CocosGUI.h"
#include <YIM.h>

USING_NS_CC;
using namespace ui;

ChannelTests::ChannelTests()
{
    ADD_TEST_CASE(LoginTest);
    ADD_TEST_CASE(ChannelTest);
    ADD_TEST_CASE(ChannelLeaveTest);
//    ADD_TEST_CASE(LoginTest);
}


LoginTest::LoginTest()
{

}

LoginTest::~LoginTest()
{

}

void LoginTest::onEnter()
{
    TestCase::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("LOGIN");
}

ChannelTest::ChannelTest()
{
    _label = Label::createWithTTF("Click button join the room", "fonts/arial.ttf", 23);
    _label->setPosition(Vec2(VisibleRect::center()));
    //_label->retain();

    auto btn_enter_room = Button::create("btn.png", "btn2.png", "btn2.png");
    btn_enter_room->setTitleText("Join Room");
    btn_enter_room->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y - 50));
    btn_enter_room->addTouchEventListener(CC_CALLBACK_1(ChannelTest::onEnterRoom, this));

    addChild(_label);
    addChild(btn_enter_room);
}

void ChannelTest::onEnter()
{
    TestCase::onEnter();

    YIMManager *im = YIMManager::CreateInstance();
    im->Login(__XT("robot123"), __XT("12345"), __XT(""));


    //TestCore::createTestCoreInstance()->doTestAction("ENT ROOM");
}

void ChannelTest::onEnterRoom(cocos2d::Ref *psender)
{
    //TestCore::createTestCoreInstance()->doTestAction("ENT ROOM");
}

ChannelTest::~ChannelTest()
{   

}

ChannelLeaveTest::ChannelLeaveTest()
{

}

ChannelLeaveTest::~ChannelLeaveTest()
{

}
