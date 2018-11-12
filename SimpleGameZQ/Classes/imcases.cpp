//
//  imcases.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#include "imcases.hpp"
#include "imservice.hpp"
#include "YouMeHttpRequest.hpp"
#include <sstream>
#include "audio/include/SimpleAudioEngine.h"

using namespace std;

IMTestController::IMTestController()
{

    m_config = new Conifg();
    m_config->timeout = 10;
    m_robotUser = ::getRandUserName();
}

IMTestController::~IMTestController()
{
}

void IMTestController::login_and_logout()
{

    srand((unsigned int)time(NULL));
    //const XCHAR* room_id = ROOM_ID; //用局部变量
    //const XCHAR* target_id = ROBOT_TARHET_ID;
    t1 = std::thread([ this]() {
        YIMManager *im = YIMManager::CreateInstance();

        while (true) {
            int r = rand();
            if (r % 100 == 1)
                sleep(1);
            if (r % 2 == 1) {
                im->Login(m_robotUser.c_str(), __XT("12345"), __XT(""));
            } else if (r % 2 == 0) {
                im->Logout();
            } else if (r % 2 == 0) {
               // im->GetChatRoomManager()->JoinChatRoom("2019889");
            } else if (r % 7 == 0) {
               // im->GetChatRoomManager()->LeaveChatRoom("2019889");
            } else if (r % 10 == 7) {
                //im->GetMessageManager()->SendTextMessage("AutoRoboter", ChatType_RoomChat, __XT("Brown ceshi"), nullptr);
            } else if (r % 10 == 8) {
               // im->GetMessageManager()->SendTextMessage("AutoRoboter", ChatType_PrivateChat, __XT("Brown ctest test"), nullptr);
            } else {
                //

            }
        }
    });
    t1.join();
}


void IMTestController::runTests()
{
    //每次开始的时候注册回调函数
    YIMManager *im = YIMManager::CreateInstance();

    im->SetLoginCallback(this);
    im->SetMessageCallback(this);
    im->SetChatRoomCallback(this);
    im->SetNoticeCallback(this);
    im->SetContactCallback(this);
    im->SetLocationCallback(this);
    im->SetAudioPlayCallback(this);
    im->SetDownloadCallback(this);
    IM_SetMode(2);

    //加入频道
    {
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code1 = im->GetChatRoomManager()->JoinChatRoom(__XT(""));
        EXPECT_EQ("加入房间:房间名为空", "JoinChatRoom", code1, YIMErrorcode_ParamInvalid);
        YIMErrorcode code = im->GetChatRoomManager()->JoinChatRoom(ROOM_ID);
        EXPECT_EQ("加入房间", "JoinChatRoom", code, YIMErrorcode_Success);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    {
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code = im->GetChatRoomManager()->JoinChatRoom(ROOM_ID);

        int i = 0;
        const XCHAR *roomID = __XT("2019899");
        while (i < 50) {
            XUINT64 reqNo;
            XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("向机器人发出一条私聊：你好"));
            im->GetMessageManager()->SendTextMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, message_text.c_str(), &reqNo);
            ++i;
        }

        i = 0;
        while ( i < 100) {
            XUINT64 reqNo;
            XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("向机器人发出一条世界频道消息: 你好"));
            im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), &reqNo);
            ++i;
        }
    }

    login_and_logout();
}

void IMTestController::sendemail()
{
    YIMManager *im = YIMManager::CreateInstance();
    cocos2d::log("IM Finished! OK");

    actionLeaveChatRoom();

    actionLogout();

    endfile();
    //发邮件将本地的 报告递出去
    std::string extentsion = cocos2d::StringUtils::format("IM_version_%d", im->GetSDKVersion());
    YouMeHttpRequest::sendFile("http://106.75.7.162:8999/sendreport", ::filename(), extentsion.c_str());
}

void IMTestController::actionSendVoiceSpeech()
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    //录音，返回下载链接, 不转文字识别
    XUINT64 reqNo;
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->GetMessageManager()->StartAudioSpeech(&reqNo, false);
    EXPECT_EQ("录音，返回下载链接", "StartAudioSpeech", code, YIMErrorcode_Success);
    if (code == YIMErrorcode_Success) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
        std::this_thread::sleep_for(std::chrono::seconds(15));
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        YIMErrorcode code1 = im->GetMessageManager()->StopAudioSpeech();
        EXPECT_EQ("开始录音", "StartAudioSpeech", code1, YIMErrorcode_Success);

        //std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        //EXPECT_NO_TIMEOUT("录音，返回下载链接", s);
    }
}

void IMTestController::actionGetNearbyObjects()
{
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->GetLocationManager()->GetNearbyObjects(2, __XT("GD"));
    EXPECT_EQ("获取附近的人", "GetNearbyObjects", code, YIMErrorcode_Success);
}


void IMTestController::voicesetting()
{
    YIMManager *im = YIMManager::CreateInstance();

    XString path = im->GetAudioCachePath();

    cocos2d::log("%s", XStringToUTF8(path).c_str());
	std::string s = cocos2d::StringUtils::format("voicesetting is %s", XStringToUTF8(path).c_str());
	write_a_message(s);

}

void IMTestController::actionLeaveChatRoom()
{
    unique_lock<std::mutex> lk(m_mutex);
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->GetChatRoomManager()->LeaveChatRoom(ROOM_ID);

    EXPECT_EQ("退出房间成功", "LeaveChatRoom", code, YIMErrorcode_Success);
    std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
    EXPECT_NO_TIMEOUT("退出房间", s);
}

void IMTestController::OnLogout(YIMErrorcode errorcode)
{
    EXPECT_EQ("退出登录", "Logout回调", errorcode, YIMErrorcode_Success);
    m_cv.notify_one();
}

void IMTestController::actionLogout()
{
    unique_lock<std::mutex> lk(m_mutex);
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->Logout();

    EXPECT_EQ("登出", "Logout", code, YIMErrorcode_Success);
    std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
    EXPECT_NO_TIMEOUT("退出登录", s);
}

//lbs
void IMTestController::actionGetCurrentLocation()
{
    //获取位置信息
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->GetLocationManager()->GetCurrentLocation();
    EXPECT_EQ("获取当前位置", "GetCurrentLocation", code, YIMErrorcode_Success);

}

//Joinroom
void IMTestController::OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    //stringstream ss;

    EXPECT_EQ("加入房间回调", "OnJoinChatRoom", errorcode, YIMErrorcode_Success);
    //送礼物
    //const char *exparam = "{\"nickname\":\"David\",\"server_area\":\"Hong Kong\",\"location\":\"Shang hai\",\"score\":\"1000\",\"level\":\"20\",\"vip_level\":\"1\",\"extra\":\"nothing\"}";

    //YIMErrorcode errcode1 = IMService::getInstance()->getYMInst()->GetMessageManager()->SendGift(ZHUBO_ID, UCHATROOMID, 1000, 1,  exparam, &reqNo1);
    m_cv.notify_one();
}

//leave room
void IMTestController::OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID)
{
    std::string casename1 = cocos2d::StringUtils::format("LeaveChatRoomcb: %s", XStringToUTF8(chatRoomID).c_str());
    EXPECT_EQ("离开房间回调", "OnLeaveChatRoom回调", errorcode, YIMErrorcode_Success);
    m_cv.notify_one();
}


void IMTestController::OnLogin(YIMErrorcode errorcode, const XString &userID)
{
    EXPECT_EQ("登录成功", "登录成功回调", errorcode, YIMErrorcode_Success);
    m_cv.notify_one();
}


void IMTestController::OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode
                                          , bool isForbidRoom, int reasonType, XUINT64 forbidEndTime)
{

    EXPECT_EQ("发送文本消息状态回调", "OnSendMessageStatus", errorcode, YIMErrorcode_Success);
    //EXPECT_EQ("", "OnSendMessageStatus", requestID > 0, true,
    //              cocos2d::StringUtils::format("seq %lld", requestID));
    cocos2d::log("On send message status %lld", requestID);

}


void IMTestController::requestEnterRoom()
{
    std::string body1("{\"UserID\": \"Robot001\", \"ChannelID\":\"2011234\"}");
    YouMeHttpRequest::request("query_im_enter_channel", body1);

    std::string body2("{\"UserID\": \"Robot002\", \"ChannelID\":\"2011234\"}");
    YouMeHttpRequest::request("query_im_enter_channel", body2);

    std::string body3("{\"UserID\": \"Robot003\", \"ChannelID\":\"2011234\"}");
    YouMeHttpRequest::request("query_im_enter_channel", body3);
}

void IMTestController::requestLeaveRoom()
{
    std::string body_string_1 = cocos2d::StringUtils::format("{\"UserID\": \"Robot001\", \"ChannelID\":\"%s\"}", ROOM_ID);
    std::string body_string_2 = cocos2d::StringUtils::format("{\"UserID\": \"Robot002\", \"ChannelID\":\"%s\"}", ROOM_ID);
    std::string body_string_3 = cocos2d::StringUtils::format("{\"UserID\": \"Robot003\", \"ChannelID\":\"%s\"}", ROOM_ID);

    YouMeHttpRequest::request("query_im_leave_channel", body_string_1);
    YouMeHttpRequest::request("query_im_leave_channel", body_string_2);
    YouMeHttpRequest::request("query_im_leave_channel", body_string_3);
}

void IMTestController::requestNotice()
{
    std::string body1 = cocos2d::StringUtils::format("{                                  \
        \"Notice\":                                       \
        {                                                 \
            \"ChannelID\": \"%s\",              \
            \"NoticeType\": 2,            \
            \"LoopType\": 1,      \
            \"SendDate\": \"20170729\",           \
            \"SendStartTime\": \"15:00:00\",       \
            \"SendTimes\": 3,             \
            \"SendInterval\": 30,              \
            \"Title\": \"title12345\",         \
            \"Content\": \"content12345\",       \
            \"LinkKeyWords\": \"keywords12345\",         \
            \"LinkAddr\": \"https://www.youme.im\",        \
            \"EnableStatus\": 2,               \
            \"Creator\": \"test\"                  \
        }}", ROOM_ID
    );
    YouMeHttpRequest::request("add_notice", body1);
}

void IMTestController::requestCancelNotice(XUINT64 id)
{
    char strbuffer[128];
    sprintf(strbuffer, "{\"Notice\": [{ \"NoticeID\": %lld }]}", id);
    std::string body1(strbuffer);
    YouMeHttpRequest::request("delete_notice", strbuffer);
}

void IMTestController::OnUserLeaveChatRoom(const XString &chatRoomID,
                                           const XString &userID)
{
    EXPECT_EQ("用户离开房间回调", "OnUserLeaveChatRoom", XStringToLocal(chatRoomID).c_str(), XStringToLocal(XString(ROOM_ID)).c_str(),
    cocos2d::StringUtils::format("User %s leaved", userID.c_str()));
}

void IMTestController::OnUserJoinChatRoom(const XString &chatRoomID, const XString &userID)
{
    EXPECT_EQ("有新用户加入房间", "OnUserJoinChatRoom", chatRoomID.c_str(), ROOM_ID);
    m_cv.notify_one();
}

void IMTestController::OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString &text, LanguageCode srcLangCode, LanguageCode destLangCode)
{
    EXPECT_EQ("文本翻译完成", "TranslateText", errorcode, YIMErrorcode_Success);

    //EXPECT_EQ("TranslateText reqNo:cb", requestID, (unsigned int)reqNo);
    m_cv.notify_one();
}

//Recv Message 收消息
//#include "cocos2d.h"


#include "YouMeHttpRequest.hpp"
void IMTestController::requestSendMsg1()
{
    //std::this_thread::sleep_for(std::chrono::seconds(1));

    //第一条
    unique_lock<std::mutex> lk(m_mutex);
    char buffer1[1024];
    long timestamp = getCurrentTime();

    sprintf(buffer1, "{\"MsgSeq\": \"%ld\",\"ChatType\": 2, \"SendID\": \"Robot\",\"RecvID\" : \"%s\",\"Content\" : \"\\\"家族成员肩:负家族使命晋升为副族长身体是阿斯顿\\\"\"}",
            timestamp, XStringToLocal(XString(ROOM_ID)).c_str());

    YouMeHttpRequest::request("query_im_send_msg", buffer1);
    std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
    EXPECT_EQ("回调超时检查", "Timeout check", int(s), int(std::cv_status::timeout));

    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //第二条 私聊
    lk.unlock();

    unique_lock<std::mutex> lk2(m_mutex);
    char buffer2[1024];
    long t = getCurrentTime();
    sprintf(buffer2, "{\"MsgSeq\": \"%ld\",\"ChatType\": 1, \"SendID\": \"Robot\",\"RecvID\" : \"%s\",\"Content\" : \"明天还有明天的事，这里是毛泽东的故乡\"}", t, XStringToLocal(XString(m_robotUser)).c_str());
    YouMeHttpRequest::request("query_im_send_msg", std::string(buffer2));
    std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(10));
    EXPECT_EQ("回调超时检查", "Timeout check", int(s2), int(std::cv_status::timeout));
}

void IMTestController::requestSendMsg2()
{
    YIMManager *im = YIMManager::CreateInstance();
    int level;
    XString text = im->FilterKeyword(__XT("出售灵魂和肉体"), &level);
    cocos2d::log("%s", XStringToLocal(text).c_str());
    char buffer[1024];
    long t = getCurrentTime();
    sprintf(buffer, "{\"MsgSeq\": \"%ld\",\"ChatType\": 2, \"SendID\": \"Robot\",\"RecvID\" : \"%s\",\"Content\" : \"%s\"}", t,
            XStringToLocal(XString(m_robotUser)).c_str(),
            XStringToLocal(text).c_str());
    YouMeHttpRequest::request("query_im_send_msg", std::string(buffer));
}

void IMTestController::requestSendMsg3()
{
    char buffer[1024];
    long t = getCurrentTime();
    XString text = __XT("Tr:今日では、ゴミをクリーンアップするために、非常に満足していません");
    sprintf(buffer, "{\"MsgSeq\": \"%ld\",\"ChatType\": 2, \"SendID\": \"Robot\",\"RecvID\" : \"%s\",\"Content\" : \"%s\"}",
            t, XStringToLocal(XString(m_robotUser)).c_str(), XStringToLocal(text).c_str());

    YouMeHttpRequest::request("query_im_send_msg", std::string(buffer));
}

void IMTestController::actionSendVoiceMsgChat()
{
        //std::this_thread::sleep_for(std::chrono::seconds(10));
        YIMManager *im = YIMManager::CreateInstance();

        XUINT64 reqNo2;

        std::this_thread::sleep_for(std::chrono::seconds(2));
        YIMErrorcode errcode2 = im->GetMessageManager()->SendOnlyAudioMessage(ROBOT_TARHET_ID, YIMChatType::ChatType_PrivateChat, &reqNo2);
        EXPECT_EQ("录制带翻译的语音，私聊", "SendOnlyAudioMessage", errcode2, YIMErrorcode_Success);
        if (errcode2 == YIMErrorcode_Success) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
            std::this_thread::sleep_for(std::chrono::seconds(3));

            unique_lock<std::mutex> lk(m_mutex);
            YIMErrorcode errcode21 = im->GetMessageManager()->StopAudioMessage(__XT("{\"test\": \"thanks very much\"}"));
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            EXPECT_EQ("停止录音并发送，带翻译，私聊", "StopAudioMessage", errcode21, YIMErrorcode_Success);
            //std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
            //EXPECT_NO_TIMEOUT("发送带翻译的语音", s);
        }
        cocos2d::log("errocode is Failed: %d", errcode2);
}

void IMTestController::actionSendVoiceMsgPrivate()
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;

        YIMErrorcode code2 = im->GetMessageManager()->SendOnlyAudioMessage(ROOM_ID, ChatType_RoomChat, &reqNo);

        EXPECT_EQ("开始录音，私聊频道", "SendOnlyAudioMessage", code2, YIMErrorcode_Success);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        YIMErrorcode code3 = im->GetMessageManager()->StopAudioMessage(__XT(""));
        EXPECT_EQ("结束录音并发送", "StopAudioMessage", code3, YIMErrorcode_Success);
}

void IMTestController::actionPlayAudio()
{
    YIMManager *im = YIMManager::CreateInstance();
    XString pathName = im->GetAudioCachePath();
    unique_lock<std::mutex> lk(m_mutex);
    YIMErrorcode errcode1 = im->StartPlayAudio(pathName.c_str());
    EXPECT_EQ("播放语音", "[PlayAudio][call]", errcode1, YIMErrorcode_Success);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_EQ("检查是否在播放中", "Check is playing", true, im->IsPlaying());
    im->SetVolume(0.5);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    im->SetVolume(0.3);
    m_cv.wait_for(lk, std::chrono::seconds(10));

    YIMErrorcode errcode2 = im->StopPlayAudio();
    EXPECT_EQ("停止语音播放", "StopAudio", errcode2, YIMErrorcode_Success);
}

void IMTestController::OnPlayCompletion(YIMErrorcode errorcode, const XString &path)
{
    EXPECT_EQ("语音播放完成", "Play completion", errorcode, YIMErrorcode_Success);
    cocos2d::log("Played path : %s", XStringToLocal(path).c_str());
    m_cv.notify_one();
}

void IMTestController::OnRecvNotice(YIMNotice *notice)
{
    requestCancelNotice(notice->GetNoticeID());
    EXPECT_EQ_STR("收到公告消息", "RecvNotice: call",  XStringToLocal(XString(notice->GetChannelID())).c_str(), XStringToLocal(XString(ROOM_ID)).c_str(),
                  cocos2d::StringUtils::format("Channel:%s", notice->GetChannelID()));
    //EXPECT_EQ("", "RecvNotice: reqNo", notice->GetNoticeID() != 0, true);
    //其他参数校验
    requestCancelNotice(notice->GetNoticeID());
    m_cv.notify_one();
}

void IMTestController::OnCancelNotice(XUINT64 noticeID, const XString &channelID)
{
    EXPECT_EQ_STR("取消公告回调", "OnCancelNotice", XStringToLocal(channelID).c_str(),
                  "2011234", cocos2d::StringUtils::format("Channel:%s", XStringToLocal(channelID).c_str()));
    //EXPECT_EQ("取消公告回调", "OnCancelNotice", noticeID != 0, true);
    m_cv.notify_one();
}

void IMTestController::OnGetRecentContacts(YIMErrorcode errorcode,
                                           std::list<XString> &contactList)
{
    EXPECT_EQ("获取最近联系人成功", "GetRecentContacts", errorcode, YIMErrorcode_Success);
    for (auto it = contactList.begin(); it != contactList.end(); ++it) {
        cocos2d::log("%s", (*it).c_str());
        EXPECT_EQ("", "GetRecentContacts:contacts", (*it).empty(), false);
    }
    m_cv.notify_one();
}

void IMTestController::OnGetRoomMemberCount(YIMErrorcode errorcode, const XString &chatRoomID, unsigned int count)
{
    EXPECT_EQ("获取房间成员数量回调", "OnGetRoomMemberCount", errorcode, YIMErrorcode_Success);
    cocos2d::log("Room number is %d", count);

    std::string comment = cocos2d::StringUtils::format("Number is %d", count);
    EXPECT_EQ("房间人数数量至少为1", "OnGetRoomMemberCount", count >= 1, true, comment);
    m_cv.notify_one();
}

void IMTestController::OnQueryHistoryMessage(YIMErrorcode errorcode, const XString &targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList)
{
    EXPECT_EQ("查询历史消息记录成功", "QueryHistoryMessage回调", errorcode, YIMErrorcode_Success);
    for (auto it = messageList.begin(); it != messageList.end(); ++it) {
        XUINT64 id = (*it)->GetMessageID();
        std::string comment = "";

        if ((*it)->GetMessageBody()->GetMessageType() ==MessageBodyType_TXT) {
            IYIMMessageBodyText* pMsg = static_cast<IYIMMessageBodyText*>((*it)->GetMessageBody());
            comment = XStringToUTF8(XString(pMsg->GetMessageContent()));
        }
        EXPECT_EQ("历史消息内容", "QueryHistoryMessage", id != 0, true, comment);
    }
    m_cv.notify_one();
}

void IMTestController::OnQueryRoomHistoryMessage(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> > messageList)
{
    EXPECT_EQ("查询房间历史消息记录成功", "QueryRoomHistoryFromserver回调", errorcode, YIMErrorcode_Success);
    for (auto it = messageList.begin(); it != messageList.end(); ++it) {
        XUINT64 id = (*it)->GetMessageID();
        std::string comment = "";

        if ((*it)->GetMessageBody()->GetMessageType() ==MessageBodyType_TXT) {
            IYIMMessageBodyText* pMsg = static_cast<IYIMMessageBodyText*>((*it)->GetMessageBody());
            comment = XStringToUTF8(XString(pMsg->GetMessageContent()));
        }
        EXPECT_EQ("房间历史消息内容", "QueryHistoryMessage", id != 0, true, comment);
    }
    m_cv.notify_one();
}

void IMTestController::OnGetUserInfo(YIMErrorcode errorcode, const XString& userID, const XString&  userInfo)
{
    EXPECT_EQ("获取用户信息成功", "OnGetUserInfo回调", errorcode, YIMErrorcode_Success);
    EXPECT_EQ_STR("用户名校验", "GetUserInfo", XStringToLocal(userID).c_str(),
                  "David",
                  cocos2d::StringUtils::format("UserID:%s", XStringToLocal(userID).c_str()));
    m_cv.notify_one();
}

void IMTestController::OnBlockUser(YIMErrorcode errorcode, const XString &userID, bool block)
{

    std::string comment = "";
    if (block) {
        comment = cocos2d::StringUtils::format("user %s is block", XStringToLocal(userID).c_str());
    } else {
        comment = cocos2d::StringUtils::format("user %s is unblock", XStringToLocal(userID).c_str());
    }

    EXPECT_EQ("屏蔽用户", "OnBlockUser", errorcode, YIMErrorcode_Success, comment);
    m_cv.notify_one();

}

void IMTestController::OnUnBlockAllUser(YIMErrorcode errorcode)
{
    EXPECT_EQ("解除屏蔽所有的用户", "OnUnBlockAllUser", errorcode, YIMErrorcode_Success);
    m_cv.notify_one();
}

void IMTestController::OnGetBlockUsers(YIMErrorcode errorcode, std::list<XString> userList)
{
    EXPECT_EQ("获取被屏蔽的所有的用户", "OnGetBlockUsers", errorcode, YIMErrorcode_Success);

    std::string comments = "";
    auto it = userList.begin();
    for (; it != userList.end(); ++it) {
        XString userName = (*it);
        comments += XStringToLocal(userName);
        comments += ";";
    }
    cocos2d::StringUtils::format("Users : %s", comments.c_str());
    m_cv.notify_one();
}

void IMTestController::actionDeletemessage()
{
}

void IMTestController::actionJubao()
{
    YIMManager *im = YIMManager::CreateInstance();

    const XCHAR* exparams = __XT("{\"nickname\":\"Mark\", \"server_area\":\"Dongfangkjds\", \"level\":1,\"vip_level\":1}");
    YIMErrorcode code = im->GetMessageManager()->Accusation(__XT("RobotJubao"), ChatType_RoomChat,
                                                            1, __XT("涉黄"), exparams);
    EXPECT_EQ("举报用户", "Accusation", code, YIMErrorcode_Success);


    const char *fmt = "{ \
        \"MsgID\" : 76360433937952770, \
        \"SendID\" : \"%s\",  \
        \"RecvID\" : \"RobotJubao\",  \
        \"Strategy\": \"ignore\"  \
    }";
    std::string body = cocos2d::StringUtils::format(fmt, "RobotRand");
    YouMeHttpRequest::request("audit_tipoff_msg", body.c_str());
}

void IMTestController::requestForbid()
{
    const char *body = "{                       \
        \"ChannelID\":\"channel123\",           \
        \"ChannelDescription\":\"ios-1\",       \
        \"ShutUpTime\":200,                   \
        \"UserList\":[                           \
                    {                            \
                    \"UserID\":\"RobotJinyan\"         \
                    }                             \
                    ]                            \
    }";
    YouMeHttpRequest::request("forbid_im_send_msg", body);
}

void IMTestController::translate(const XCHAR* content)
{
    YIMManager *im = YIMManager::CreateInstance();
    unsigned int no;
    YIMErrorcode code = im->GetMessageManager()->TranslateText(&no, content, LANG_ZH_CN, LANG_JA);
    EXPECT_EQ("翻译文本", "TranslateText", code, YIMErrorcode_Success);
}

void IMTestController::OnRecvMessage(std::shared_ptr<IYIMMessage> pMessage)
{
    /***
     1 公聊
     2 私聊
     3 脏词广告 公聊
     4 日语 公聊
     5 公聊 自定义
     **/
    YIMMessageBodyType msgType = pMessage->GetMessageBody()->GetMessageType();
    if (msgType == YIMMessageBodyType::MessageBodyType_TXT) {
        //文本消息
        //static int index = 0;
        IYIMMessageBodyText* pMsg = static_cast<IYIMMessageBodyText*>(pMessage->GetMessageBody());
        ;
        cocos2d::log("Message: %s",XStringToLocal(XString(pMsg->GetMessageContent())).c_str());
        //XUINT64 id = pMessage->GetMessageID();
        std::string text = XStringToLocal(XString(pMsg->GetMessageContent()));
        std::string msg = XStringToLocal(XString(PROCOLOL_FROM_PREFIX));
        if (isStartWith(msg, text)) {
            m_cv.notify_one();
        }
    } else if (msgType == YIMMessageBodyType::MessageBodyType_Gift) {
        IYIMMessageGift *pMsg = static_cast<IYIMMessageGift*>(pMessage->GetMessageBody());
        cocos2d::log("[Recv msg:][ Message id : %lld", pMessage->GetMessageID());
        cocos2d::log("             Message exp: %s",
                     XStringToLocal(XString(pMsg->GetExtraParam())).c_str());
        cocos2d::log("             sender: %s", pMessage->GetSenderID());
        cocos2d::log("             recver: %s", pMessage->GetReceiveID());
        cocos2d::log("             zhubo: %s", XStringToLocal(XString(pMsg->GetAnchor())).c_str());
        cocos2d::log("             gift cnt: %d", pMsg->GetGiftCount());
        cocos2d::log("             gift id: %d", pMsg->GetGiftID());
        cocos2d::log("             ]");
    } else if (msgType == YIMMessageBodyType::MessageBodyType_Voice) {
        // voice
        IYIMMessageBodyAudio * pMsgVoice = (IYIMMessageBodyAudio*)pMessage->GetMessageBody();
        cocos2d::log("[Recv msg:][ Message id : %lld", pMessage->GetMessageID());
        cocos2d::log("             sender: %s", XStringToLocal(XString(pMessage->GetSenderID())).c_str());
        cocos2d::log("             recvor: %s", XStringToLocal(XString(pMessage->GetReceiveID())).c_str());
        cocos2d::log("[Recv msg:][  %s \n messageid is %llu \n param is %s \n audio time is %d\n]",  pMsgVoice->GetText(),
                     pMessage->GetMessageID(),
                     pMsgVoice->GetExtraParam(),
                     pMsgVoice->GetAudioTime());

        //下载语音文件示例
        //->DownloadFile(pMessage->GetMessageID(), "/sdcard/audiocaches/123.wav");
        YIMManager::CreateInstance()->GetMessageManager()->DownloadFile(pMessage->GetMessageID(), wav_save_path.c_str());
       // m_cv.notify_one();
    } else if (msgType == YIMMessageBodyType::MessageBodyType_CustomMesssage) {
        //static int index = 0;
        IYIMMessageBodyCustom *pMsg = static_cast<IYIMMessageBodyCustom*>(pMessage->GetMessageBody());
        cocos2d::log("[Custom msg: [%s]",
                     pMsg->GetCustomMessage().c_str());
    }
}

void IMTestController::OnReceiveMessageNotify(YIMChatType chatType, const XString &targetID)
{
    EXPECT_EQ("手动消息拉取，消息到达通知", "OnReceiveMessageNotify", chatType, ChatType_RoomChat);
	EXPECT_EQ_STR("手动消息拉取，消息到达通知，用户ID", "OnReceiveMessageNotify", XStringToLocal(targetID).c_str(), XStringToLocal(XString(ROOM_ID)).c_str());
    YIMManager *im = YIMManager::CreateInstance();

    //手动获取消息
    vector<XString> rooms{ ROOM_ID };
    YIMErrorcode code = im->GetMessageManager()->GetNewMessage(rooms);
    EXPECT_EQ("手动获取消息", "GetNewMessage", code, YIMErrorcode_Success);


    YIMErrorcode code2 = im->GetMessageManager()->SetReceiveMessageSwitch(rooms, false);
    EXPECT_EQ("关闭手动消息拉取的开关", "SetReceiveMessageSwitch", code2, YIMErrorcode_Success);
    m_cv.notify_one();
}

void IMTestController::OnAccusationResultNotify(AccusationDealResult result, const XString &userID, unsigned int accusationTime)
{
    EXPECT_EQ_STR("举报结果通知", "[AccusationResultNotify]:cb", XStringToLocal(userID).c_str(), "Robot",
                  cocos2d::StringUtils::format("AccusationResultNotify:Time is %u", accusationTime));
    //EXPECT_EQ("", "[AccusationResultNotify]:time", accusationTime, (unsigned int)200);
    m_cv.notify_one();
}

void IMTestController::OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString &text, const XString &audioPath, unsigned int audioTime)
{
    EXPECT_EQ("停止语音录制回调", "OnStartSendAudioMessage", errorcode, YIMErrorcode_Success);
    cocos2d::log("OnStartSendAudioMessage cb");
    //m_cv.notify_one(); 另一个回调接口已经通知过了
}

void IMTestController::OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo)
{
    EXPECT_EQ("语音消息发送成功回调", "[StopAudioSpeechStatus]:cb", errorcode, YIMErrorcode_Success,
              XStringToLocal(XString(audioSpeechInfo->GetText())));
//    const XCHAR *url = audioSpeechInfo->GetDownloadURL();
//    const XCHAR *localath = audioSpeechInfo->GetLocalPath();
//    //const XCHAR* text = audioSpeechInfo->GetText();
//    XUINT64 rid = audioSpeechInfo->GetRequestID();
    cocos2d::log("[cb:success]");
    m_cv.notify_one();
}

//下载回调
void IMTestController::OnDownload(YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> msg, const XString &savePath)
{
    EXPECT_EQ("下载成功", "OnDownload", errorcode, YIMErrorcode_Success);
    m_cv.notify_one();
}

void IMTestController::OnDownloadByUrl(YIMErrorcode errorcode, const XString &strFromUrl, const XString &savePath)
{
    EXPECT_EQ("根据url下载语音成功", "OnDownloadByUrl", errorcode, YIMErrorcode_Success);
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->StartPlayAudio(savePath.c_str());
    EXPECT_EQ("开始播放语音", "StartPlayAudio", code, YIMErrorcode_Success);
    if (code == YIMErrorcode_Success) {
        bool b = im->IsPlaying();
        EXPECT_EQ("判断是否正在播放语音", "IsPlaying", b, true);
    }
    YIMErrorcode code1 = im->StopPlayAudio();
    EXPECT_EQ("停止播放语音", "StopPlayAudio", code1, YIMErrorcode_Success);
    m_cv.notify_one();
}

//stopAudio 停止录音并发送 回调到此
void IMTestController::OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString &text, const XString &audioPath, unsigned int audioTime, bool isForbidRoom, int reasonType, XUINT64 forbidEndTime)
{

    EXPECT_EQ("停止录音并发送成功", "OnSendAudioMessageStatus", errorcode, YIMErrorcode_Success, XStringToUTF8(text).c_str());
    cocos2d::log("[=====][cb:success]");
}
//LBS

void IMTestController::OnGetNearbyObjects(YIMErrorcode errorcode, std::list<std::shared_ptr<RelativeLocation> > neighbourList, unsigned int startDistance, unsigned int endDistance)
{
    std::string info = cocos2d::StringUtils::format("distans: (%d, %d)", startDistance, endDistance);
    EXPECT_EQ("获取附近联系人", "OnGetNearbyObjects", errorcode, YIMErrorcode_Success, info);
    m_cv.notify_one();
}

void IMTestController::OnUpdateLocation(YIMErrorcode errorcode, std::shared_ptr<GeographyLocation> location)
{
    EXPECT_EQ("更新地理位置信息成功", "OnUpdateLocation", errorcode, YIMErrorcode_Success);
    m_cv.notify_one();
}


/**/

//VoiceCaseController::VoiceCaseController()
//{
//    YIMManager *im = YIMManager::CreateInstance();
//    std::unique_lock<std::mutex> lk(m_mutex);
//    YIMErrorcode code = im->Login(USER_ID_A, __XT("12345"), __XT(""));
//
//    if (code == YIMErrorcode_Success) {
//        m_cv.wait_for(lk, std::chrono::seconds(10));
//    }
//
//    lk.unlock();
//
//    std::unique_lock<std::mutex> lk2(m_mutex);
//    YIMErrorcode code1 = im->GetChatRoomManager()->JoinChatRoom(ROOM_ID);
//    if (code1 == YIMErrorcode_Success) {
//        m_cv.wait_for(lk, std::chrono::seconds(10));
//    }
//}
//
//VoiceCaseController::~VoiceCaseController()
//{
//
//}
//
//void VoiceCaseController::case_send_voice_chatroom()
//{

//}
//
//void VoiceCaseController::case_send_voice_private()
//{
//    YIMManager *im = YIMManager::CreateInstance();
//    XUINT64 reqNo;
//
//    //std::unique_lock<std::mutex> lk2(m_mutex);
//
//    YIMErrorcode code2 = im->GetMessageManager()->SendOnlyAudioMessage(ROOM_ID, ChatType_RoomChat, &reqNo);
//
//    EXPECT_EQ("开始录音，私聊频道", "SendOnlyAudioMessage", code2, YIMErrorcode_Success);
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//
//    YIMErrorcode code3 = im->GetMessageManager()->StopAudioMessage(__XT(""));
//    EXPECT_EQ("结束录音并发送", "StopAudioMessage", code3, YIMErrorcode_Success);
//}
//
//
//
//void VoiceCaseController::OnRecvMessage(std::shared_ptr<IYIMMessage> message)
//{
//
//}
//
//void VoiceCaseController::OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString &text, const XString &audioPath, unsigned int audioTime)
//{
//
//}
//
//void VoiceCaseController::OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString &text, const XString &audioPath, unsigned int audioTime, bool isForbidRoom, int reasonType, XUINT64 forbidEndTime)
//{
//
//}

