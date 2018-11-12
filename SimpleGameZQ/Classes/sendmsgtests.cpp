//
//  sendmsgtests.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "sendmsgtests.hpp"

#include "imservice.hpp"


USING_NS_CC;

SendMsgTests::SendMsgTests()
{
    ADD_TEST_CASE(SendTextMsgTest);
    ADD_TEST_CASE(SendCuostomMsgTest);
    ADD_TEST_CASE(SendGiftMsgTest);
    ADD_TEST_CASE(SendMessageTestsNoCrash);
}

bool SendTextMsgTest::init()
{
    if (!TestCase::init()) {
        return false;
    }
    auto btn = ui::Button::create("btn.png", "btn2.png", "btn2.png");
    btn->setPosition(VisibleRect::center());
    btn->addTouchEventListener(CC_CALLBACK_0(SendTextMsgTest::sendTextMsg, this));
    btn->setTitleText("Send Message");

    addChild(btn);
    return true;
}

void SendTextMsgTest::sendTextMsg()
{
   // TestCore::createTestCoreInstance()->doTestAction("SND MSG");
}

void SendTextMsgTest::onEnter()
{
    TestCase::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("SND MSG");
}

//send custom

bool SendCuostomMsgTest::init()
{
    if (!TestCase::init()) {
        return false;
    }
    auto btn = ui::Button::create("btn.png", "btn2.png", "btn2.png");
    btn->setPosition(VisibleRect::center());
    btn->addTouchEventListener(CC_CALLBACK_0(SendCuostomMsgTest::sendCustomMsg, this));
    btn->setTitleText("Send Message");
    
    addChild(btn);
    return true;
}

void SendCuostomMsgTest::sendCustomMsg()
{
    //TestCore::createTestCoreInstance()->doTestAction("SND CST MSG");
}

void SendCuostomMsgTest::onEnter()
{
    TestCase::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("SND CST MSG");
}


//send gift
bool SendGiftMsgTest::init()
{
    if (!TestCase::init()) {
        return false;
    }
    auto btn = ui::Button::create("btn.png", "btn2.png", "btn2.png");
    btn->setPosition(VisibleRect::center());
    btn->addTouchEventListener(CC_CALLBACK_0(SendGiftMsgTest::sendGiftMsg, this));
    btn->setTitleText("Send Message");

    addChild(btn);
    return true;
}

void SendGiftMsgTest::sendGiftMsg()
{
   // TestCore::createTestCoreInstance()->doTestAction("SND GIFT");
}

void SendGiftMsgTest::onEnter()
{
    TestCase::onEnter();
   // TestCore::createTestCoreInstance()->doTestAction("SND GIFT");
}


//Crash Test
bool SendMessageTestsNoCrash::init()
{
    if (!TestCase::init()) {
        return false;
    }
    _label = Label::createWithTTF("Test Robust", "fonts/arial.ttf", 24);
    _label->setPosition(VisibleRect::center());

    addChild(_label);
    return true;
}

void SendMessageTestsNoCrash::onEnter()
{
    TestCase::onEnter();
    //exeption test case
    XUINT64 reqNo;
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode errcode51 = im->GetMessageManager()->SendTextMessage(nullptr,
		YIMChatType::ChatType_Unknow, __XT(""), &reqNo);
    cocos2d::log("Param:(nullptr, YIMChatType::ChatType_Unknow): errcode %d", errcode51);

    YIMErrorcode errcode52 = im->GetMessageManager()->SendTextMessage(UROOM_ID,
		YIMChatType::ChatType_RoomChat, nullptr, &reqNo);
    cocos2d::log("reqest id is %lld", reqNo);
    cocos2d::log("Param:(UCHATROOMID, YIMChatType::ChatType_RoomChat, nullptr): errcode %d", errcode52);

    YIMErrorcode errcode53 = im->GetMessageManager()->SendTextMessage(UROOM_ID,
		YIMChatType::ChatType_RoomChat, __XT(" "), &reqNo);
    cocos2d::log("reqest id is %lld", reqNo);
    cocos2d::log("Param:(UCHATROOMID, YIMChatType::ChatType_RoomChat, [space]) errcode is %d", errcode53);

    CC_ASSERT(reqNo != 0);
    XUINT64 reqNo1 = 0;
    char msgBuffer[100] = "Robot: This is a message for test custom case";
    YIMErrorcode errcode12 = im->GetMessageManager()->SendCustomMessage(UROOM_ID,
                                                                       YIMChatType::ChatType_RoomChat,
                                                                       msgBuffer, -1, &reqNo1);
    cocos2d::log("request no is %lld", reqNo1);
    CHECK_RETURN_CODE(errcode12, "Send custom msg success", "Send custom message failed!");

    YIMErrorcode errcode13 = im->GetMessageManager()->SendCustomMessage(UROOM_ID,
		YIMChatType::ChatType_RoomChat, nullptr, 1000, &reqNo1);
    cocos2d::log("request no is %lld", reqNo1);
    CHECK_RETURN_CODE(errcode13, "Send custom msg success",
		"Send custom message failed!");

    YIMErrorcode errcode14 = im->GetMessageManager()->SendCustomMessage(UROOM_ID,
		YIMChatType::ChatType_RoomChat, msgBuffer, 0, &reqNo1);
    cocos2d::log("request no is %lld", reqNo1);
    CHECK_RETURN_CODE(errcode14, "Send custom msg success", "Send custom message failed!");

    YIMErrorcode errcode2 = im->GetMessageManager()->SendCustomMessage(UROOM_ID,
		YIMChatType::ChatType_RoomChat, msgBuffer, 10, &reqNo1);
    cocos2d::log("request no is %lld", reqNo1);
    CHECK_RETURN_CODE(errcode2, "Send custom msg success", "Send custom message failed!");
}













