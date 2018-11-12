//
//  sendvoicetests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#ifndef sendvoicetests_hpp
#define sendvoicetests_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"
#include "YIM.h"
#include <condition_variable>
#include <mutex>

DEFINE_TEST_SUITE(SendVoiceTests);

class SendVoiceBaseTest : public TestCase,
public IYIMLoginCallback,
public IYIMMessageCallback,
public IYIMChatRoomCallback
{
public:
    SendVoiceBaseTest();
    ~SendVoiceBaseTest();

    virtual std::string title() const override {
        return "Send Voice tests";
    }
    void OnLogin(YIMErrorcode errorcode, const XString& userID) override {
        if (errorcode == YIMErrorcode_Success)
            cocos2d::log("Success login %s", XStringToLocal(userID).c_str());
        m_cv.notify_one();
    }

    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode,
                                         const XString& text, const XString& audioPath,
                                         unsigned int audioTime) override {
        cocos2d::log("... %s ...", XStringToLocal(text).c_str());
        m_cv.notify_one();
    }


    virtual void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, unsigned int sendTime, bool isForbidRoom, int reasonType, XUINT64 forbidEndTime) {}

    void OnLogout(YIMErrorcode errorcode) override
    {
        m_cv.notify_one();
    }

    void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) override {
        cocos2d::log("Join Chat room %s", XStringToLocal(chatRoomID).c_str());
        m_cv.notify_one();
    }
    //离开频道回调
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) override {
        cocos2d::log("Leave Chat room %s", XStringToLocal(chatRoomID).c_str());
        m_cv.notify_one();
    }

protected:
    std::mutex m_mutex;
    std::condition_variable m_cv;
};
/**
 case_send_voice_msg_text_only();
 case_send_voice_msg();
 case_stop_void_msg();
 case_cancel_voice_msg();
 case_send_voice_file();
 case_play_autdio();

 case_send_voice_except();
 */
class SendVoiceMsgTest : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(SendVoiceMsgTest);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Send Normal Voice Message Test";
    }
    bool init() override;
};

class CancelVoiceTest : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(CancelVoiceTest);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Cancel Voice Record Test";
    }
};

class SendVoiceNoTextTest : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(SendVoiceNoTextTest);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Send Voice Message Wirh No Text Test";
    }
};

class SendVoiceFile : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(SendVoiceFile);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Send Voice File Test";
    }
};

class SendVoiceExcept : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(SendVoiceExcept);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Send Voice Except";
    }
};

class PlayAudio : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(PlayAudio);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Play Audio Test";
    }
};

class DownloadAudioFile : public SendVoiceBaseTest
{
public:
    CREATE_FUNC(DownloadAudioFile);
    virtual void onEnter() override;
    virtual std::string subtitle() const override {
        return "Download audio file Test";
    }
};

#endif /* sendvoicetests_hpp */
