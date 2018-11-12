//
//  autosendrecvmsg.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#include "autosendrecvmsg.hpp"
#include "imservice.hpp"

SendMsgDemoTests::SendMsgDemoTests()
{
    ADD_TEST_CASE(SendMsgDemoTest);
}

SendMsgDemoTest::SendMsgDemoTest()
{

}

SendMsgDemoTest::~SendMsgDemoTest()
{

}

void SendMsgDemoTest::onEnter()
{
    TestCase::onEnter();
}

bool SendMsgDemoTest::init()
{
    if(!TestCase::init()) {
        return false;
    }
    text1 = cocos2d::ui::TextField::create();
    text2 = cocos2d::ui::TextField::create();

    text1->setPosition(cocos2d::Vec2(VisibleRect::left().x + 30, VisibleRect::left().y));
    text2->setPosition(VisibleRect::center());
    text1->setFontSize(18.0);
    text2->setFontSize(18.0);

    text1->setString("Messages");

    _status_label = cocos2d::Label::createWithTTF("status", "fonts/arial.ttf", 15.0);
    _status_label->setPosition(cocos2d::Vec2(VisibleRect::bottom().x, VisibleRect::bottom().y + 40));

    addChild(_status_label);
    addChild(text1);
    addChild(text2);

    cocos2d::ui::Button *send_msg_btn = cocos2d::ui::Button::create("common_btn.png");
    send_msg_btn->setTitleFontSize(20.0f);
    send_msg_btn->setTitleText("Send");
    send_msg_btn->addTouchEventListener(CC_CALLBACK_0(SendMsgDemoTest::sendTextMsg, this));
    send_msg_btn->setPosition(cocos2d::Vec2(VisibleRect::bottom().x, VisibleRect::bottom().y + 20));

    addChild(send_msg_btn);

    //setting callback
    YIMManager* im = YIMManager::CreateInstance();
    //CCASSERT(im != nullptr, "im message cannot be nullptr");
    im->SetLoginCallback(this);
    im->SetChatRoomCallback(this);
    im->SetMessageCallback(this);
    return true;
}

void SendMsgDemoTest::OnRecvMessage(std::shared_ptr<IYIMMessage> message)
{
    
}

void SendMsgDemoTest::OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString &text, LanguageCode srcLangCode, LanguageCode destLangCode)
{

}

void SendMsgDemoTest::OnLogin(YIMErrorcode errorcode, const XString& userID)
{
    //const XCHAR* chatroom_id = __XT("2011234");
    //char buffer[128];
    //YIMManager *im = IMService::getInstance()->getYMInst();
    //XUINT64 reqNo;
    //if (errorcode == YIMErrorcode_Success) {
    //    sprintf(buffer, "User:%s login success", XStringToLocal(userID).c_str());
    //    _status_label->setString(buffer);
    //    YIMErrorcode code = im->GetChatRoomManager()->JoinChatRoom(chatroom_id);

        //im->Login("Tony", "12345", "");
    //    if (code != 0)
    //        _status_label->setString("user join room failed");

    //} else {
    //    sprintf(buffer, "User:%s login failed", XStringToLocal(userID).c_str());
    //    _status_label->setString(buffer);
    //}
    cocos2d::log("Login success...");
}

void SendMsgDemoTest::sendTextMsg()
{
    
    YIMManager *im = YIMManager::CreateInstance();
    CCASSERT(im != nullptr, "im handle cannot be nullptr");

    YIMErrorcode code1 = im->Login(__XT("Micheal"), __XT("12345"), __XT(""));

    if (code1 != 0) {
        text1->setString("User 1: Micheal Login Failed!");
    }
}


void SendMsgDemoTest::OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID)
{
    YIMManager *im = YIMManager::CreateInstance();
    XUINT64 reqNo;
    if (errorcode == YIMErrorcode_Success) {
        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(chatRoomID.c_str() , ChatType_RoomChat, __XT("test"), &reqNo);
        if (code != 0) {
            text1->setString("Enterroom Failed");
        }
    }
    im->Login(__XT("Tony"), __XT("12345"), __XT(""));
}

void SendMsgDemoTest::OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID)
{

}

void SendMsgDemoTest:: OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID)
{

}

void SendMsgDemoTest::OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID)
{

}

void SendMsgDemoTest::OnReceiveMessageNotify(YIMChatType chatType, const XString &targetID)
{

}
