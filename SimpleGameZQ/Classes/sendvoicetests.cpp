//
//  sendvoicetests.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "sendvoicetests.hpp"
#include "imservice.hpp"
#include "imcases.hpp"

SendVoiceTests::SendVoiceTests()
{
    ADD_TEST_CASE(SendVoiceMsgTest);
    ADD_TEST_CASE(SendVoiceFile);
    ADD_TEST_CASE(CancelVoiceTest);
    ADD_TEST_CASE(SendVoiceNoTextTest);
    ADD_TEST_CASE(PlayAudio);
    ADD_TEST_CASE(DownloadAudioFile);
    ADD_TEST_CASE(SendVoiceExcept);
}

SendVoiceBaseTest::SendVoiceBaseTest()
{
    YIMManager *im = YIMManager::CreateInstance();
    YIMManager::CreateInstance()->SetLoginCallback(this);
    YIMManager::CreateInstance()->SetMessageCallback(this);
    YIMManager::CreateInstance()->SetChatRoomCallback(this);

    std::unique_lock<std::mutex> lk(m_mutex);
    YIMErrorcode code = im->Login(USER_ID_A, __XT("12345"), __XT(""));
    if (code == YIMErrorcode_Success) {
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        if (s == std::cv_status::timeout) {
            cocos2d::log("Timout failed ....");}
        else {
            cocos2d::log("[finished waiting ...]");
        }
    }

    YIMErrorcode code2 = im->GetChatRoomManager()->JoinChatRoom(ROOM_ID);
    if (code2 == YIMErrorcode_Success) {
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
}

SendVoiceBaseTest::~SendVoiceBaseTest()
{
    YIMManager *im = YIMManager::CreateInstance();
    std::unique_lock<std::mutex> lk(m_mutex);
    YIMErrorcode code1 = im->GetChatRoomManager()->LeaveChatRoom(ROOM_ID);
    if (code1 == YIMErrorcode_Success) {
        if (m_cv.wait_for(lk, std::chrono::seconds(10)) == std::cv_status::timeout) {
            cocos2d::log("leave room timout...");
        } else {
            cocos2d::log("leave room success!!!");
        }
    }

    YIMErrorcode code2 =  im->Logout();

    if (code2 == YIMErrorcode_Success) {
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        if (s == std::cv_status::timeout)
            cocos2d::log("logout timeout...");
        else
            cocos2d::log("logout success...");
    }

    YIMManager::CreateInstance()->SetLoginCallback(nullptr);
    YIMManager::CreateInstance()->SetMessageCallback(nullptr);
}

bool SendVoiceMsgTest::init()
{
    if (!SendVoiceBaseTest::init()) {
        return false;
    }

    auto label = cocos2d::Label::createWithTTF("Starting working. Recording... keep 15s",
                                               "fonts/arial.ttf", 23.0);
    label->setPosition(VisibleRect::center());
    addChild(label);
    return true;
}

void SendVoiceMsgTest::onEnter()
{
    SendVoiceBaseTest::onEnter();
    YIMManager *im = YIMManager::CreateInstance();
    XUINT64 reqNo;

    std::unique_lock<std::mutex> lk2(m_mutex);

    YIMErrorcode code2 = im->GetMessageManager()->SendAudioMessage(ROOM_ID, ChatType_RoomChat, &reqNo);
    cocos2d::log("return code is %d", code2);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    YIMErrorcode code3 = im->GetMessageManager()->StopAudioMessage(__XT(""));
    cocos2d::log("return code is %d", code3);
    //m_cv.wait_for(lk2, std::chrono::seconds(20));
}

void SendVoiceNoTextTest::onEnter()
{
    SendVoiceBaseTest::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("SND VOICE ON");
}

void CancelVoiceTest::onEnter()
{
    SendVoiceBaseTest::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("CANCEL VOICE SND");
}

void SendVoiceFile::onEnter()
{
    SendVoiceBaseTest::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("SND WAV FILE");
}

void PlayAudio::onEnter()
{
    SendVoiceBaseTest::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("PLAY VOICE");
}

void DownloadAudioFile::onEnter()
{
    SendVoiceBaseTest::onEnter();
    //TestCore::createTestCoreInstance()->doTestAction("DOWNLOAD FILE");
}


//except
void SendVoiceExcept::onEnter()
{
    //65s case
    SendVoiceBaseTest::onEnter();
    YIMManager* im = YIMManager::CreateInstance();
    XUINT64 reqNo;
    YIMErrorcode errcode1 = im->GetMessageManager()->SendAudioMessage(ROOM_ID, ChatType_RoomChat, &reqNo);
    CHECK_RETURN_CODE(errcode1, "SendAudioMessage Success", "SendAudioMessage Failed");
    if (errcode1 == YIMErrorcode_Success) {
        sleep(65); //录音65秒
    }else {
        cocos2d::log("Send Error! Test Cancel! ");
        return;
    }

    YIMErrorcode errcode2 = im->GetMessageManager()->StopAudioMessage(__XT("{\"code\": \"luyin chenggong,yes\"}"));
    CHECK_RETURN_CODE(errcode2, "StopAudioMessage Success", "StopAudioMessage Failed");
}
