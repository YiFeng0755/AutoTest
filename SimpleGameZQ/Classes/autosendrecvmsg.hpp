//
//  autosendrecvmsg.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#ifndef autosendrecvmsg_hpp
#define autosendrecvmsg_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "testbase/BaseTest.h"
#include "YIM.h"


DEFINE_TEST_SUITE(SendMsgDemoTests);

/*实现一收一发的两个用户登录*/
class SendMsgDemoTest : public TestCase, public IYIMMessageCallback
, public IYIMLoginCallback
, public IYIMChatRoomCallback
{
public:
    SendMsgDemoTest();
    ~SendMsgDemoTest();
    virtual bool init() override ;
    CREATE_FUNC(SendMsgDemoTest);
    virtual std::string subtitle() const override {
        return "Send Message Demo Test";
    }
    void sendTextMsg();
    virtual void onEnter() override;

    virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) override;
    void OnReceiveMessageNotify(YIMChatType chatType,  const XString&  targetID) override;
    virtual void OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString& text, LanguageCode srcLangCode, LanguageCode destLangCode) override;
//    virtual void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime ) override;

    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID) override;

    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) override;
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) override;
    virtual void OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID) override;
    virtual void OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID) override;
    
private:
    cocos2d::ui::TextField *text1;
    cocos2d::ui::TextField *text2;
    cocos2d::Label *_status_label;
};

#endif /* autosendrecvmsg_hpp */
