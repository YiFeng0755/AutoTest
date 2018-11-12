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

void IMTestController::runCaseThreadFunc()
{
    init_cases();
    message_cases();
    user_mgr_cases();
    //voice_cases();
    
    userinfo_cases(); // 用户资料管理
    haoyou_cases();  // 好友管理
    
    leave_cases();
}


void IMTestController::init_cases()
{
    YIMManager *im = YIMManager::CreateInstance();
    

    im->SetLoginCallback(this);
    im->SetMessageCallback(this);
    im->SetChatRoomCallback(this);
    im->SetNoticeCallback(this);
    im->SetContactCallback(this);
    im->SetLocationCallback(this);
    im->SetAudioPlayCallback(this);
    im->SetDownloadCallback(this);
    //im->SetReconnectCallback(this);
    
    im->SetNoticeCallback(this);
    im->SetFriendCallback(this);
    im->SetUserProfileCallback(this);
    
    initfile();
}

void IMTestController::message_cases()
{
    //每次开始的时候注册回调函数
    YIMManager *im = YIMManager::CreateInstance();
    {
        //不登录就调用接口
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code1 = im->GetChatRoomManager()->JoinChatRoom(ROOM_ID);
        EXPECT_EQ("未登录加入房间", "JoinChatRoom", code1, YIMErrorcode_NotLogin);

        XUINT64 reqNo;
        YIMErrorcode code2 = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, __XT("Yes"), &reqNo);
        EXPECT_EQ("未登录发世界消息", "SendTextMessage", code2, YIMErrorcode_NotLogin);

        YIMErrorcode code3 = im->GetMessageManager()->SendCustomMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, "yes", 10, &reqNo);
        EXPECT_EQ("未登录发自定义消息", "SendCustomMessage", code3, YIMErrorcode_NotLogin);

        YIMErrorcode code4 = im->GetMessageManager()->SendTextMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, __XT("123456"), &reqNo);
        EXPECT_EQ("未登录发文本私聊消息", "SendTextMessage", code4, YIMErrorcode_NotLogin);

        YIMErrorcode code5 = im->GetLocationManager()->GetCurrentLocation();
        EXPECT_EQ("未登录获取地理位置", "GetCurrentLocation", code5, YIMErrorcode_NotLogin);

        YIMErrorcode code6 = im->GetLocationManager()->GetNearbyObjects(100, __XT(""));
        EXPECT_EQ("未登录获取最近联系人", "GetNearbyObjects", code6, YIMErrorcode_NotLogin);

        YIMErrorcode code7 = im->GetMessageManager()->BlockUser(ROBOT_TARHET_ID, true);
        EXPECT_EQ("未登录，屏蔽其他用户", "BlockUser", code7, YIMErrorcode_NetError);

        YIMErrorcode code8 = im->GetUserInfo(m_robotUser.c_str());
        EXPECT_EQ("未登录，获取用户信息", "GetUserInfo", code8, YIMErrorcode_NotLogin);

        YIMErrorcode code9 = im->GetRecentContacts();
        EXPECT_EQ("未登录，获取用户信息", "GetRecentContacts", code9, YIMErrorcode_NotLogin);

        YIMErrorcode code10 = im->GetMessageManager()->SendAudioMessage(ROOM_ID, ChatType_RoomChat, &reqNo);
        EXPECT_EQ("未登录，开始录制语音", "SendAudioMessage", code10, YIMErrorcode_NotLogin);

        std::vector<XUINT64> msglist;
        YIMErrorcode code11 = im->GetMessageManager()->DeleteSpecifiedHistoryMessage(m_robotUser.c_str(), ChatType_PrivateChat, msglist);
        EXPECT_EQ("未登录，删除指定消息", "DeleteSpecifiedHistoryMessage", code11, YIMErrorcode_NotLogin);

        YIMErrorcode code12 = im->GetMessageManager()->SendFile(ROOM_ID, ChatType_RoomChat, __XT(""), NULL, __XT(""));
        EXPECT_EQ("未登录，发送文件", "SendFile", code12, YIMErrorcode_NotLogin);

        YIMErrorcode code13 = im->GetMessageManager()->SendGift(__XT("10000"), ROOM_ID, 5, 10, "{\"nickname\": \"xiaoli\"}", NULL);
        EXPECT_EQ("未登录，发送文件", "SendGift", code13, YIMErrorcode_NotLogin);
    }

    //先登录 Login
    {
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code1 = im->Login(__XT(""), __XT(""), __XT(""));
        EXPECT_EQ("用户登录:用户名为空", "Login", code1, YIMErrorcode_ParamInvalid);

        YIMErrorcode code2 = im->Login(m_robotUser.c_str(), __XT("12345"), __XT(""));
        EXPECT_EQ("用户登录", "Login", code2, YIMErrorcode_Success);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    //加入频道
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code1 = im->GetChatRoomManager()->JoinChatRoom(__XT(""));
        EXPECT_EQ("加入房间:房间名为空", "JoinChatRoom", code1, YIMErrorcode_ParamInvalid);

        YIMErrorcode code = im->GetChatRoomManager()->JoinChatRoom(ROOM_ID);
        EXPECT_EQ("加入房间", "JoinChatRoom", code, YIMErrorcode_Success);
        if (code == YIMErrorcode_Success)
            m_cv.wait_for(lk, std::chrono::seconds(10));
        else
            lk.unlock();
    }


    {
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo1;
        YIMErrorcode code1 = im->GetMessageManager()->SendTextMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, __XT("123456"), &reqNo1);

        std::vector<XUINT64> ids;
       // ids.push_back(reqNo1);
        cocos2d::log("%s", m_robotUser.c_str());

        YIMErrorcode code2 = im->GetMessageManager()->DeleteSpecifiedHistoryMessage(__XT("AutoRoboter"), ChatType_PrivateChat, ids);

        cocos2d::log("%d", code2);
    }
   
    //删除指定消息
    {
        YIMManager *im = YIMManager::CreateInstance();
        std::vector<XUINT64> messagelist;
        YIMErrorcode code1 = im->GetMessageManager()->DeleteSpecifiedHistoryMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, messagelist);
        EXPECT_EQ("删除指定用户的私聊消息", "DeleteSpecifiedHistoryMessage", code1,
                  YIMErrorcode_Success);

        XUINT64 reqNo1, reqNo2;
        YIMErrorcode code2 = im->GetMessageManager()->SendTextMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, __XT("1234"), &reqNo1);

        YIMErrorcode code3 = im->GetMessageManager()->SendTextMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, __XT("1234"), &reqNo2);

        if (code1 == YIMErrorcode_Success)
            messagelist.push_back(reqNo1);
        if (code2 == YIMErrorcode_Success)
            messagelist.push_back(reqNo2);

        YIMErrorcode code4 = im->GetMessageManager()->DeleteSpecifiedHistoryMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, messagelist);
        EXPECT_EQ("删除指定用户指定的序列号的私聊消息", "DeleteSpecifiedHistoryMessage", code4, YIMErrorcode_Success);

    }


    //发送消息私聊
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        XUINT64 reqNo;
        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("向机器人发出一条私聊：你好"));
        YIMErrorcode code1 = im->GetMessageManager()->SendTextMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, message_text.c_str(), &reqNo);
        EXPECT_EQ("发送消息，文本私聊消息", "SendTextMessage", code1, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_NO_TIMEOUT("发送消息，文本私聊消息", s);
    }

    //发世界频道消息
    {
        YIMManager *im = YIMManager::CreateInstance();
        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("向机器人房间发一条消息：大家好"));
        unique_lock<std::mutex> lk(m_mutex);
        XUINT64 reqNo;

        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), &reqNo);
        EXPECT_EQ("发送消息，文本世界频道消息", "SendTextMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_NO_TIMEOUT("发送消息，文本世界频道消息", s);
    }
    
    {
        //异常参数的case
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;
        YIMErrorcode code1 = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, __XT(""), &reqNo);
        EXPECT_EQ("发送空文本", "SendTextMessage", code1, YIMErrorcode_ParamInvalid);
        YIMErrorcode code2 = im->GetMessageManager()->SendTextMessage(__XT(""), ChatType_RoomChat, __XT("123456789"), &reqNo);
        EXPECT_EQ("房间ID为空", "SendTextMessage", code2, YIMErrorcode_ParamInvalid);

    }

    //发送特殊字符串消息
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        XUINT64 reqNo;
        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("\\\"家族成员肩负家族使命晋升为副族长身体是阿斯顿\\\""));
        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), &reqNo);
        EXPECT_EQ("发送消息，文本世界频道消息", "SendTextMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        if (s == std::cv_status::no_timeout) {
            EXPECT_EQ("发送消息，文本世界频道带引号的消息", "消息接收成功", 0, 0);
        } else {
            EXPECT_EQ("发送消息，文本世界频道带引号的消息", "消息未确认接收", 0, 1);
        }
    }

    //长文本世界频道消息
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        XUINT64 reqNo;
        int len = sizeof(MessageText) / sizeof(MessageText[0]);
        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(MessageText[rand() % len]);

        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), &reqNo);
        EXPECT_EQ("发送消息，随机长度文本世界频道消息", "SendTextMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        if (s == std::cv_status::no_timeout) {
            EXPECT_EQ("发送消息，随机长度文本世界频道消息", "消息接收成功", 0, 0);
        } else {
            EXPECT_EQ("发送消息，随机长度文本世界频道消息", "消息未确认接收", 0, 1);
        }
    }

    //敏感文本
    {
        XUINT64 reqNo;
        XString message_text = XString(PROTOCOL_TO_PREFIX)
                + XString(__XT("甲乙丙丁戊己庚辛申酉戌亥哈哈哈哈哈哈啊哈fuck"));
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), &reqNo);
        EXPECT_EQ("发送消息，敏感文本世界频道消息", "SendTextMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_NO_TIMEOUT("发送消息，敏感文本世界频道消息", s);
    }

    {
        //大文本 exceed512个字符
        YIMManager *im = YIMManager::CreateInstance();
        XString big_text = XString(PROTOCOL_TO_PREFIX);
        XString long_text_unit = XString(__XT("甲乙丙丁戊己庚辛申酉戌亥"));
        for (int i = 0; i < 50; ++i) {
            big_text += long_text_unit;
        }
        XUINT64 reqNo;
        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, big_text.c_str(), &reqNo);
        EXPECT_EQ("发送大段消息，1200个中文字符", "SendTextMessage", code,
                  YIMErrorcode_MessageTooLong, "长文本不得超过512字符");

    }

    {
        // 自定义消息 世界频道
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;
        std::string message_text = XStringToLocal(XString(PROTOCOL_TO_PREFIX)) + std::string("自定义消息，甲乙丙丁戊己庚辛申酉戌亥");
        YIMErrorcode code = im->GetMessageManager()->SendCustomMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), 30, &reqNo);

        EXPECT_EQ("世界频道自定义消息", "SendCustomMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_NO_TIMEOUT("世界频道自定义消息", s);
    }
 
    {
        //自定义消息 私聊
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;
        std::string message_text = XStringToLocal(XString(PROTOCOL_TO_PREFIX)) + std::string("自定义消息，甲乙丙丁戊己庚辛申酉戌亥");
        YIMErrorcode code = im->GetMessageManager()->SendCustomMessage(ROBOT_TARHET_ID, ChatType_PrivateChat, message_text.c_str(), 30, &reqNo);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_EQ("发送自定义消息, 私聊", "SendCustomMessage", code, YIMErrorcode_Success);
        EXPECT_NO_TIMEOUT("发送自定义消息", s);
    }

    {
        //自定义消息，字节数为临界值 2 * 1024
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;
        std::string text {' '};
        text.resize(1024 * 3, 'c');
        std::string message_text = XStringToLocal(XString(PROTOCOL_TO_PREFIX)) + text;
        YIMErrorcode code = im->GetMessageManager()->SendCustomMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), 1024 * 2, &reqNo);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_EQ("发送自定义消息， 截取字节为临界值2048", "SendCustomMessage", code, YIMErrorcode_Success);
        EXPECT_NO_TIMEOUT("发送自定义消息", s);
    }

    {
        //自定义消息，截取的字节数比原来的文本大
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;

        std::string message_text = XStringToLocal(XString(PROTOCOL_TO_PREFIX)) + std::string("abcdefghij");
        YIMErrorcode code = im->GetMessageManager()->SendCustomMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), 1024, &reqNo);

        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_EQ("发送自定义消息，截取长度比原文本数大", "SendCustomMessage", code, YIMErrorcode_Success);
        EXPECT_NO_TIMEOUT("发送自定义消息", s);
    }

    {
        //异常参数的场景
        //unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        XUINT64 reqNo;
        std::string message_text = XStringToLocal(XString(PROTOCOL_TO_PREFIX)) + XStringToLocal(XString( __XT("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

        YIMErrorcode code1 = im->GetMessageManager()->SendCustomMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), -1, &reqNo);
        EXPECT_EQ("发送自定义消息, 字节数传入-1", "SendCustomMessage", code1, YIMErrorcode_ParamInvalid);

        YIMErrorcode code2 = im->GetMessageManager()->SendCustomMessage(nullptr, ChatType_RoomChat, message_text.c_str(), 1024, &reqNo);
        EXPECT_EQ("发送自定义消息， 房间号传入空字符串", "SendCustomMessage", code2, YIMErrorcode_ParamInvalid);

        YIMErrorcode code3 = im->GetMessageManager()->SendCustomMessage(ROOM_ID, ChatType_RoomChat, nullptr, 1024, &reqNo);
        EXPECT_EQ("发送自定义消息， 内容传入空指针", "SendCustomMessage", code3, YIMErrorcode_ParamInvalid);

        YIMErrorcode code4 = im->GetMessageManager()->SendCustomMessage(ROOM_ID, ChatType_RoomChat, "", 1, &reqNo);

        EXPECT_EQ("发送自定义消息， 内容传入空字符串", "SendCustomMessage", code4, YIMErrorcode_Success);

    }

    {
        //sendGift
        YIMErrorcode code1 = im->GetMessageManager()->SendGift(__XT("10000"), ROOM_ID, 5, 10, "123214324124", NULL); //数字也是json 这里会崩溃，在修复之前
        EXPECT_EQ("发送礼物消息，内容是不合法的json字符串", "SendGift", code1, YIMErrorcode_ParamInvalid);

        YIMErrorcode code2 = im->GetMessageManager()->SendGift(__XT("10000"), ROOM_ID, 5, 10, "", NULL);
        EXPECT_EQ("发送礼物消息，内容是空字符串", "SendGift", code2, YIMErrorcode_ParamInvalid);
        const char * extraParam =
        "{\"nickname\":\"小王\",\"server_area\":\"cn\",\"location\":\"\",\"score\":\"10\", \
        \"level\":\"5\",\"vip_level\":\"10\",\"extra\":\"xcd\"}";
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code3 = im->GetMessageManager()->SendGift(__XT("10000"), ROOM_ID, 5, 10, extraParam, NULL);

        EXPECT_EQ("发送礼物消息", "SendGift", code3, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));

        EXPECT_NO_TIMEOUT("发送礼物消息", s);
    }
    {
        //SendFile

        std::ofstream outfile("test.test", std::ios::out);
        outfile << "test .... test text hello world";
        outfile.close();
        //cocos2d::FileUtils::createDirectory("test.test");
        XUINT64 reqNo;
        YIMErrorcode code1 = im->GetMessageManager()->SendFile(ROOM_ID, ChatType_RoomChat, __XT("test.test"),
			&reqNo, __XT("12312312123123"));
        EXPECT_EQ("发送文件，exparam是数字", "SendFile", code1,  YIMErrorcode_FileNotExist);

        YIMErrorcode code2 = im->GetMessageManager()->SendFile(ROOM_ID, ChatType_RoomChat,
			__XT(""), &reqNo, __XT("12312312123123"));
        EXPECT_EQ("发送文件，文件路径为空", "SendFile", code2,  YIMErrorcode_ParamInvalid);

        YIMErrorcode code3 = im->GetMessageManager()->SendFile(ROOM_ID, ChatType_RoomChat,
			__XT("test.test"), &reqNo, __XT(""));
        EXPECT_EQ("发送文件，expparam为空", "SendFile", code3,  YIMErrorcode_FileNotExist);

        YIMErrorcode code4 = im->GetMessageManager()->SendFile(ROOM_ID, ChatType_RoomChat,
			__XT("test.test"), &reqNo, __XT("{\"user\": \"tom\"}"));
       // EXPECT_EQ("发送文件，参数正常", "SendFile", code4, YIMErrorcode_Success);

        cocos2d::FileUtils::getInstance()->removeFile("test.test");
    }

    {
        //群发消息
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        std::vector<XString> recivers;
        recivers.push_back(ROBOT_TARHET_ID);
        recivers.push_back(__XT("1236"));
        recivers.push_back(__XT("1237"));

        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("群发消息，向三个用户群发"));
        YIMErrorcode code = im->GetMessageManager()->MultiSendTextMessage(recivers, message_text.c_str());
        EXPECT_EQ("多用户群发消息", "MultiSendTextMessage", code, YIMErrorcode_Success);
        //不等待回调
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        EXPECT_NO_TIMEOUT("多用户群发消息", s);
    }

    {
        //群发接口异常参数验证
        //unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        std::vector<XString> recivers;
        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("群发消息，向三个用户群发"));
        YIMErrorcode code1 = im->GetMessageManager()->MultiSendTextMessage(recivers, message_text.c_str());
        EXPECT_EQ("群发接口异常参数验证, 群发数组为空", "MultiSendTextMessage", code1, YIMErrorcode_ParamInvalid);

        recivers.push_back(ROBOT_TARHET_ID);
        recivers.push_back(__XT("1236"));
        recivers.push_back(__XT("1237"));
        YIMErrorcode code2 = im->GetMessageManager()->MultiSendTextMessage(recivers, __XT(""));
        EXPECT_EQ("群发接口异常参数验证, 发送空文本", "MultiSendTextMessage", code2, YIMErrorcode_ParamInvalid);
    }

    {
        //
    }
    //消息本地管理
    {
        //查看用户历史消息 默认参数
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code = im->GetMessageManager()->QueryHistoryMessage(m_robotUser.c_str(), ChatType_RoomChat);
        EXPECT_EQ("查询历史消息，向后查找", "QueryHistoryMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_NO_TIMEOUT("查询用户历史消息", s);
    }

    {
        //向后查找
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code = im->GetMessageManager()->QueryHistoryMessage(m_robotUser.c_str(), ChatType_RoomChat, 0, 10, 1);
        EXPECT_EQ("查询历史消息，向前查找", "QueryHistoryMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_NO_TIMEOUT("查询用户历史消息", s);
    }

    {
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code = im->GetMessageManager()->QueryHistoryMessage(m_robotUser.c_str(), ChatType_PrivateChat, 0, 10, 1); //非房间方式+非0
        EXPECT_EQ("查询历史消息，向前查找", "QueryHistoryMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_NO_TIMEOUT("查询用户历史消息", s);
    }

    {
        unique_lock<std::mutex> lk(m_mutex);
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code = im->GetMessageManager()->QueryHistoryMessage(m_robotUser.c_str(), ChatType_PrivateChat, 0, 10, 0); //非房间方式+0
        EXPECT_EQ("查询历史消息，私聊方式，向前查找", "QueryHistoryMessage", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_NO_TIMEOUT("查询用户历史消息，私聊方式，向前查找", s);
    }

    {
        //查询房间消息 异常参数
        YIMErrorcode code1 = im->GetMessageManager()->QueryHistoryMessage(nullptr, ChatType_PrivateChat, 1, 1, 1);
        EXPECT_EQ("查询房间，目标用户传入空指针", "QueryHistoryMessage", code1, YIMErrorcode_ParamInvalid);

        YIMErrorcode code2 = im->GetMessageManager()->QueryHistoryMessage(__XT(""), ChatType_PrivateChat, 1, 1, 0);
        EXPECT_EQ("查询房间，目标ID为空", "QueryHistoryMessage", code2, YIMErrorcode_ParamInvalid);
    }

    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code1 = im->GetMessageManager()->QueryRoomHistoryMessageFromServer(ROOM_ID, 3);
        EXPECT_EQ("查询房间历史消息", "QueryRoomHistoryFromserver", code1, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("查询房间历史消息，向后查找", s);
    }

    {
        //异常参数校验
        YIMManager *im = YIMManager::CreateInstance();
        YIMErrorcode code1 = im->GetMessageManager()->QueryRoomHistoryMessageFromServer(nullptr);
        EXPECT_EQ("查询房间历史消息，参数roomid传入空指针", "QueryRoomHistoryFromserver", code1, YIMErrorcode_ParamInvalid);
    }

    {
        //删除历史消息
        YIMManager *im = YIMManager::CreateInstance();
        //参数不要传 time=0 和ChatType_Unknown否则删除表
        time_t t = time(0);
        YIMErrorcode code1 = im->GetMessageManager()->DeleteHistoryMessage(ChatType_Multi, t);
        EXPECT_EQ("删除历史消息，群发，截止到当前时刻", "DeleteHistoryMessage", code1, YIMErrorcode_Success);

        YIMErrorcode code2 = im->GetMessageManager()->DeleteHistoryMessage(ChatType_Unknow, t);
        EXPECT_EQ("删除历史消息， 未知聊天类型，截止到当前时刻", "DeleteHistoryMessage", code2, YIMErrorcode_Success);

        YIMErrorcode code3 = im->GetMessageManager()->DeleteHistoryMessage(ChatType_RoomChat, t);
        EXPECT_EQ("删除历史消息, 房间维度，截止到当前时间", "DeleteHistoryMessage", code3, YIMErrorcode_Success);

        YIMErrorcode code4 = im->GetMessageManager()->DeleteHistoryMessage(ChatType_PrivateChat, 0);
        EXPECT_EQ("删除历史消息, 房间维度，时刻为0", "DeleteHistoryMessage", code4, YIMErrorcode_Success);

    }
}


void IMTestController::runTests()
{
    m_exe_thread = std::thread(&IMTestController::runCaseThreadFunc, this);
    m_exe_thread.join();

    /**
     语音聊天
     */
    cocos2d::log("Finished!!!!!");
    sendemail();
}

void IMTestController::OnKickOff()
{
    cocos2d::log("kick off somebody:");
}

void IMTestController::user_mgr_cases()
{
    {
        //读写用户信息
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        const XCHAR *userinfo = __XT("{\"nickname\":\"David\",\"server_area_id\":\"10086\",\"server_area\":\"dongfangkejidasha\",\"location_id\":\"shenzhen\",\"location\":\"shenzhen\",\"level\":\"100\",\"vip_level\":\"10\",\"platform_id\":\"iphone_id\",\"platform\":\"iphone\"}");
        YIMErrorcode code = im->SetUserInfo(userinfo);
        EXPECT_EQ("设置用户信息", "SetUserInfo", code, YIMErrorcode_Success);

        YIMErrorcode code1 = im->GetUserInfo(__XT("David"));
        EXPECT_EQ("获取用户信息", "GetUserInfo", code1, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_NO_TIMEOUT("读写用户信息", s);
    }

    {
        //获取最近联系人
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code = im->GetRecentContacts();
        EXPECT_EQ("获取最近联系人", "GetRecentContacts", code, YIMErrorcode_Success);
        //std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(20));
        //EXPECT_NO_TIMEOUT("获取最近联系人", s);
    }

    {
        //手动拉取消息
        unique_lock<std::mutex> lk(m_mutex);
        XUINT64 reqNo;
        YIMManager *im = YIMManager::CreateInstance();
        vector<XString> rooms{ROOM_ID};

        YIMErrorcode code = im->GetMessageManager()->SetReceiveMessageSwitch(rooms, false);
        EXPECT_EQ("设置收动拉取消息：是", "SetReceiveMessageSwitch", code, YIMErrorcode_Success);

        //与机器人交互
        XString message_text = XString(PROTOCOL_TO_PREFIX) + XString(__XT("手动拉取消息，请点拉取"));
        YIMErrorcode code1 = im->GetMessageManager()->SendTextMessage(ROOM_ID, ChatType_RoomChat, message_text.c_str(), &reqNo);
        EXPECT_EQ("发送手动拉取消息", "SendTextMessage", code1, YIMErrorcode_Success);
        //手动拉取消息
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("手动拉取消息", s);
    }

    {
        //文本翻译
        unique_lock<std::mutex> lk(m_mutex);
        translate(__XT("今日では、ゴミをクリーンアップするために、非常に満足していません"));
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("文本翻译", s);
    }
    //获取房间人数
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);

        YIMErrorcode code = im->GetChatRoomManager()->GetRoomMemberCount(ROOM_ID);
        EXPECT_EQ("获取房间人数", "GetRoomMemberCount", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(25));
        EXPECT_NO_TIMEOUT("获取房间人数回调", s);
    }
    //屏蔽一个用户
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk(m_mutex);
        YIMErrorcode code = im->GetMessageManager()->BlockUser(ROBOT_TARHET_ID, true);
        EXPECT_EQ("屏蔽用户", "BlockUser", code, YIMErrorcode_Success);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("屏蔽用户回调", s);
    }

    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk1(m_mutex);
        YIMErrorcode code1 = im->GetMessageManager()->GetBlockUsers();
        EXPECT_EQ("获取被屏蔽的用户数量", "GetBlockUsers", code1, YIMErrorcode_Success);
        std::cv_status s1 = m_cv.wait_for(lk1, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("获取被屏蔽的用户数量", s1);
    }

    //解除屏蔽
    {
        YIMManager *im = YIMManager::CreateInstance();
        unique_lock<std::mutex> lk2(m_mutex);
        YIMErrorcode code2 = im->GetMessageManager()->BlockUser(ROBOT_TARHET_ID, false);
        EXPECT_EQ("解除屏蔽用户", "BlockUser", code2, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("解除屏蔽用户回调", s2);
    }

    {
        YIMManager *im = YIMManager::CreateInstance();
        std::this_thread::sleep_for(std::chrono::seconds(10));
        unique_lock<std::mutex> lk3(m_mutex);
        YIMErrorcode code3 = im->GetMessageManager()->BlockUser(ROBOT_TARHET_ID, true);
        EXPECT_EQ("屏蔽用户", "BlockUser", code3, YIMErrorcode_Success);
        std::cv_status s3 = m_cv.wait_for(lk3, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("屏蔽用户", s3);
        lk3.unlock();

        unique_lock<std::mutex> lk4(m_mutex);
        YIMErrorcode code4= im->GetMessageManager()->UnBlockAllUser();
        EXPECT_EQ("解除所有屏蔽的用户", "UnBlockAllUser", code4, YIMErrorcode_Success);
        std::cv_status s4 = m_cv.wait_for(lk4, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("解除所有被屏蔽用户回调", s4);
    }

}

void IMTestController::voice_cases()
{
    YIMManager *im = YIMManager::CreateInstance();
    {
        //SendOnlyAudioMessage 带翻译的录音 公聊
        XUINT64 reqNo1;
        YIMErrorcode errcode1 = im->GetMessageManager() ->SendAudioMessage(ROOM_ID,YIMChatType::ChatType_RoomChat, &reqNo1);
		EXPECT_EQ("不带翻译的录音 公聊", "SendAudioMessage", errcode1, YIMErrorcode_Success);
        if (errcode1 == YIMErrorcode_Success) {
            //EXPECT_EQ("", "[SendOnlyAudioMessage][normal][call]", errcode1, YIMErrorcode_Success);
            cocos2d::log("Request id is %lld", reqNo1);
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("mangetunoyoruniodore.mp3");
            std::this_thread::sleep_for(std::chrono::seconds(15));

            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

            unique_lock<std::mutex> lk(m_mutex);
            YIMErrorcode errcode11 = im->GetMessageManager()->StopAudioMessage(__XT("{\"test\": \"thanks very much\"}"));
            EXPECT_EQ("不带翻译的录音 公聊", "StopAudioMessage", errcode11, YIMErrorcode_Success);
           // std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
           // EXPECT_NO_TIMEOUT("发送带翻译的语音", s);
        }
    }

    {
        //SendOnlyAudioMessage 带翻译的录音 私聊
        XUINT64 reqNo2;

        std::this_thread::sleep_for(std::chrono::seconds(5));
        YIMErrorcode errcode2 = im->GetMessageManager()->SendAudioMessage(ROBOT_TARHET_ID, YIMChatType::ChatType_PrivateChat, &reqNo2);
        EXPECT_EQ("录制带翻译的语音，私聊", "SendOnlyAudioMessage", errcode2, YIMErrorcode_Success);
        if (errcode2 == YIMErrorcode_Success) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
            std::this_thread::sleep_for(std::chrono::seconds(10));

            //unique_lock<std::mutex> lk(m_mutex);
            YIMErrorcode errcode21 = im->GetMessageManager()->StopAudioMessage(__XT("{\"test\": \"thanks very much\"}"));
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            EXPECT_EQ("停止录音并发送，带翻译，私聊", "StopAudioMessage", errcode21, YIMErrorcode_Success);
            //std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
            //EXPECT_NO_TIMEOUT("发送带翻译的语音", s);
        }
        cocos2d::log("errocode is Failed: %d", errcode2);
    }
    {
        //世界频道发送语音 不带翻译
        XUINT64 reqNo;
        
        YIMErrorcode code1 = im->GetMessageManager()->SendOnlyAudioMessage(ROOM_ID, ChatType_RoomChat, &reqNo);
        EXPECT_EQ("录制不带翻译的语音消息", "SendOnlyAudioMessage", code1, YIMErrorcode_Success);
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
        //sleep(5);
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        YIMErrorcode code2 = im->GetMessageManager()->StopAudioMessage(__XT(""));
        EXPECT_EQ("停止录制不带翻译的语音消息并发送出去", "SendOnlyAudioMessage", code2, YIMErrorcode_Success);
        //EXPECT_NO_TIMEOUT("发送不带翻译的语音", m_cv.wait_for(lk, std::chrono::seconds(10)));
    }

    {
        //SendAudioMessage 私聊
        XUINT64 reqNo;
		std::this_thread::sleep_for(std::chrono::seconds(5));
        YIMErrorcode code1 = im->GetMessageManager()->SendOnlyAudioMessage(ROBOT_TARHET_ID,                  YIMChatType::ChatType_PrivateChat, &reqNo);
        EXPECT_EQ("录制不带翻译的语音消息，私聊", "SendOnlyAudioMessage", code1, YIMErrorcode_Success);

        if (code1 == YIMErrorcode_Success) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
            std::this_thread::sleep_for(std::chrono::seconds(10));
            //录制10s钟
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
           
            YIMErrorcode code2 = im->GetMessageManager()->StopAudioMessage(__XT(""));
            EXPECT_EQ("录制不带翻译的语音消息，停止录制并发送，私聊", "StopAudioMessage", code2, YIMErrorcode_Success);
            //EXPECT_NO_TIMEOUT("发送不带翻译的语音", m_cv.wait_for(lk, std::chrono::seconds(10)));
        }
    }

    {
        //取消录音
        XUINT64 reqNo;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        YIMErrorcode code = im->GetMessageManager()->SendAudioMessage(ROOM_ID, YIMChatType::ChatType_RoomChat, &reqNo);
        EXPECT_EQ("", "SendAudioMessage", code, YIMErrorcode_Success);
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        YIMErrorcode code1 = im->GetMessageManager()->CancleAudioMessage();
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        EXPECT_EQ("取消录音", "CancleAudioMessage", code1, YIMErrorcode_Success);
    }

    {
        //举报用户
        unique_lock<std::mutex> lk(m_mutex);
        actionJubao();
        //std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        //EXPECT_NO_TIMEOUT("举报用户", s);
    }
}

// 用户信息
void IMTestController::userinfo_cases()
{
    YIMManager *im = YIMManager::CreateInstance();
   
     { // 编辑基本资料
         unique_lock<std::mutex> lk2(m_mutex);
         IMUserSettingInfo userSettingInfo;
         userSettingInfo.nickName = ::getRandUserName();// "testQA";
         userSettingInfo.sex = SEX_FEMALE;
         userSettingInfo.personalSignature = __XT("QA 质量第一，积极奋进");
         userSettingInfo.city = XString(__XT("huizhou"));
		 userSettingInfo.province = XString(__XT("guangdong"));//"fujian";
		 userSettingInfo.country = XString(__XT("CHINA"));
         YIMErrorcode code =im->GetUserProfileManager()->SetUserProfileInfo(userSettingInfo);
         
         EXPECT_EQ("设置用户基本资料", "SetUserProfileInfo", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("设置用户基本资料", s2);
     }
    
     { // 修改基本资料
        IMUserSettingInfo userSettingInfo;
		userSettingInfo.nickName = XString(__XT("深圳广州香港澳门珠海*&￥中山惠州东莞北京上海武汉南京长春辽宁浙江杭州c"));  // >32个汉字 %#*&
        userSettingInfo.sex = SEX_MALE;
		userSettingInfo.personalSignature = XString(__XT("深圳广州香港惠州%#&￥c"));  // QA 质量第一%#*&￥
		userSettingInfo.city = XString(__XT("nanning"));
		userSettingInfo.province = XString(__XT("guangxi"));
		userSettingInfo.country = XString(__XT("CHINA"));
        YIMErrorcode code =im->GetUserProfileManager()->SetUserProfileInfo(userSettingInfo);
         
        EXPECT_EQ("设置用户基本资料-昵称长度超64b", "SetUserProfileInfo", code, YIMErrorcode_NickNameTooLong);
     }
    
    { // 修改基本资料
        IMUserSettingInfo userSettingInfo;
		userSettingInfo.nickName = XString(__XT("深圳广州香港澳门%#*&￥北京1234杭州cdsdg"));  // <32个汉字
        userSettingInfo.sex = SEX_MALE;
		userSettingInfo.personalSignature = XString(__XT("深圳广州香港惠州%#*&￥c深圳广州香港澳门珠海中山惠州东莞北京上海武汉南京长春辽宁浙江杭州c深圳广州香港澳门珠海中山惠州东莞北京上海武汉南京长春辽宁浙江杭州c深圳广州香港澳门珠海中山惠州东莞北京上海武汉南京长春辽宁浙江杭州c"));
		userSettingInfo.city = XString(__XT("nanning"));
		userSettingInfo.province = XString(__XT("guangxi"));
		userSettingInfo.country = XString(__XT("CHINA"));
        YIMErrorcode code =im->GetUserProfileManager()->SetUserProfileInfo(userSettingInfo);
        
        EXPECT_EQ("设置用户基本资料-签名长度超120b", "SetUserProfileInfo", code, YIMErrorcode_SignatureTooLong);
    }
    
    
    {  // 设置头像.jpg
        unique_lock<std::mutex> lk2(m_mutex);
        std::string resourceroot =
        cocos2d::FileUtils::getInstance()->fullPathForFilename("23.jpg");
        XString filepath = LocalToXString(resourceroot);
        YIMErrorcode code =im->GetUserProfileManager()->SetUserProfilePhoto(filepath.c_str());
        
        EXPECT_EQ("设置用户头像", "SetUserProfilePhoto", code, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("设置用户头像", s2);
     }
            
     {  // 设置头像.png
         std::string resourceroot =
         cocos2d::FileUtils::getInstance()->fullPathForFilename("303K.png");
         XString filepath = LocalToXString(resourceroot);
         YIMErrorcode code =im->GetUserProfileManager()->SetUserProfilePhoto(filepath.c_str());
         EXPECT_EQ("设置用户头像，图片大小超过100kb", "SetUserProfilePhoto", code, YIMErrorcode_PhotoSizeTooLarge);
     }
    
     {  // 设置头像
        const XCHAR* resourceroot =__XT("dskja4vtnojikpomlmnkhggvyjmkmojnmjundskja4vtnojikpomlmnkhggvyjmkmojnmjundskja4vtnojikpomldskja4vtnojikpomlmnkhggvyjmkmojnmjundskja4vtnojikpomlmnkhggvyjmkmonmundskja4vtnojikpomlmnkh67656hjfsweqeweertuioplmnhjgfdsazxcvkmojnmjundskja4vtnojikpomlmnkhggvyjmkmojnmjundskja4vtnojikpomldskja4vtnojikpomlmnkhggvyjmkmojnmjundskja4vtnojikpomlmnkhggvyskja4vtnojikpomlmnkhhuihyugygygyug67656hazxcvjnmjundskja4vtnojikpomlmnkhggvyjmkmojnmjundskja4vtnopomldskja4vtnojikpomlmnkhggvymkmoundskja4vtnojikpomlmnkhggvyundsk4vtnopomlmnkhggvymkmlklkyug67656hee4dxs356rtuioplmnhjgfdsazxcv23.jpg");
      
        YIMErrorcode code =im->GetUserProfileManager()->SetUserProfilePhoto(resourceroot);
        //cocos2d::log("头像err %d, len %ld ",code, resourceroot.size());
        EXPECT_EQ("设置用户头像，URL长度超过500b", "SetUserProfilePhoto", code, YIMErrorcode_PhotoUrlTooLong );
     }
    
    { // 切换用户状态-隐身
        unique_lock<std::mutex> lk2(m_mutex);
        YIMErrorcode code = im->GetUserProfileManager()->SwitchUserStatus(m_robotUser.c_str(), STATUS_INVISIBLE); // 隐身
        
        EXPECT_EQ("切换用户状态-隐身", "SwitchUserStatus", code, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("切换用户状态-隐身", s2);
    }
    
    { // 获取用户资料
        unique_lock<std::mutex> lk2(m_mutex);
        YIMErrorcode code =im->GetUserProfileManager()->GetUserProfileInfo(m_robotUser.c_str() ); // 本人
        //   YIMErrorcode code =im->GetUserProfileManager()->GetUserProfileInfo(__XT("1225") ); // 好友
        //   im->GetUserProfileManager()->GetUserProfileInfo(__XT("6666") ); // 黑名单中
        //   im->GetUserProfileManager()->GetUserProfileInfo(__XT("abcd") ); // 陌生人
        
        EXPECT_EQ("获取用户资料", "GetUserProfileInfo", code, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("获取用户资料", s2);
    }
    
    { // 设置好友验证权限 displayPermission被查找时是否显示被添加权限true是；  addPermission 被添加好友验证权限
        
        YIMErrorcode code =im->GetUserProfileManager()->SetAddPermission(true, NEED_VALIDATE); // 需要验证
        EXPECT_EQ("设置好友验证权限", "SetAddPermission", code, YIMErrorcode_Success);
        
        //YIMErrorcode code =im->GetUserProfileManager()->SetAddPermission(true, NOT_ALLOW_ADD); // 不允许被添加
        //YIMErrorcode code =im->GetUserProfileManager()->SetAddPermission(true, NO_ADD_PERMISSION); // 不需要验证，默认值
    }
    // 验证好友 0同意；1拒绝；在添加请求通知里调用
    
    { // 切换用户状态-离线
        unique_lock<std::mutex> lk2(m_mutex);
        YIMErrorcode code = im->GetUserProfileManager()->SwitchUserStatus(m_robotUser.c_str(), STATUS_OFFLINE); // 离线
        
        EXPECT_EQ("切换用户状态-离线", "SwitchUserStatus", code, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("切换用户状态-离线", s2);
    }
    
    { // 切换用户状态-在线
        unique_lock<std::mutex> lk2(m_mutex);
        YIMErrorcode code = im->GetUserProfileManager()->SwitchUserStatus(m_robotUser.c_str(), STATUS_ONLINE); // 在线
        
        EXPECT_EQ("切换用户状态-在线", "SwitchUserStatus", code, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("切换用户状态-在线", s2);
    }
}

void IMTestController::haoyou_cases()
{
    YIMManager *im = YIMManager::CreateInstance();
    
     { // 查找好友 0:按ID查找；1:按昵称查找
         unique_lock<std::mutex> lk2(m_mutex);
         YIMErrorcode code = im->GetFriendManager()->FindUser(0, __XT("5002"));
         EXPECT_EQ("按ID查找用户", "FindUser", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("按ID查找用户", s2);
     }
     {
         unique_lock<std::mutex> lk2(m_mutex);
         YIMErrorcode code = im->GetFriendManager()->FindUser(1, __XT("深圳"));
         EXPECT_EQ("按昵称查找用户", "FindUser", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("按昵称查找用户", s2);
     }
    
     { // 添加好友
         unique_lock<std::mutex> lk2(m_mutex);
         std::vector<XString> users;
         // 批量添加好友
         users.push_back(__XT("1225"));
         users.push_back(__XT("1228"));
         users.push_back(__XT("5002"));
         users.push_back(__XT("5003")); // 不需要验证
         YIMErrorcode code = im->GetFriendManager()->RequestAddFriend(users, __XT("hello friend56#*&￥浙江c深")); // 特殊字符

         EXPECT_EQ("添加好友", "RequestAddFriend", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("添加好友", s2);
     }
    
     { // 添加好友
         unique_lock<std::mutex> lk2(m_mutex);
         std::vector<XString> users;
         users.push_back(__XT("2229"));
         YIMErrorcode code = im->GetFriendManager()->RequestAddFriend(users, __XT("深圳广州香港澳门珠海中山惠州东莞北京上海武汉南京长春辽宁浙江杭州深圳广州香港澳门珠海6t") ); // 128字节
         
         EXPECT_EQ("添加好友", "RequestAddFriend", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("添加好友，内容超128b", s2);
     }
    
    
     { // 查询我的好友 type: 0正常好友；1被拉黑好友
         unique_lock<std::mutex> lk2(m_mutex);
         YIMErrorcode code = im->GetFriendManager()->QueryFriends(0, 0, 51);
         EXPECT_EQ("查询好友资料", "QueryFriends", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("查询好友资料", s2);
     }

    
    
     { // 获取好友请求list
         unique_lock<std::mutex> lk2(m_mutex);
         YIMErrorcode code = im->GetFriendManager()->QueryFriendRequestList(1, 21);
     
         EXPECT_EQ("获取好友请求list", "QueryFriendRequestList", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("获取好友请求list", s2);
     }
     
     { // 删除好友 tpye：0双向删除；1单向删除
         unique_lock<std::mutex> lk2(m_mutex);
         std::vector<XString> users;
         users.push_back(__XT("1228"));
         YIMErrorcode code = im->GetFriendManager()->DeleteFriend(users, 1);  // 1单向删除；
         
         EXPECT_EQ("单向删除好友", "DeleteFriend", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("单向删除好友", s2);
     }
     {
         unique_lock<std::mutex> lk2(m_mutex);
         std::vector<XString> users1;
         users1.push_back(__XT("5003"));
         YIMErrorcode code = im->GetFriendManager()->DeleteFriend(users1, 0); // 0双向删除；
       
         EXPECT_EQ("双向删除好友", "DeleteFriend", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("双向删除好友", s2);
     }
    
     
     { // 拉黑好友 type 0拉黑；1解除拉黑
         unique_lock<std::mutex> lk2(m_mutex);
         std::vector<XString> users;
         users.push_back(__XT("1225"));
         YIMErrorcode code = im->GetFriendManager()->BlackFriend(0, users);  // 0拉黑
         
         EXPECT_EQ("拉黑好友", "BlackFriend", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("拉黑好友", s2);
     }
    { // 查询我的好友 type: 0正常好友；1被拉黑好友
        unique_lock<std::mutex> lk2(m_mutex);
        YIMErrorcode code = im->GetFriendManager()->QueryFriends(1, 0, 5);
        EXPECT_EQ("查询被拉黑好友", "QueryFriends", code, YIMErrorcode_Success);
        std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
        EXPECT_NO_TIMEOUT("查询被拉黑好友", s2);
    }
    
     { // 拉黑好友 type 0拉黑；1解除拉黑
         unique_lock<std::mutex> lk2(m_mutex);
         std::vector<XString> users;
         users.push_back(__XT("1225"));
         YIMErrorcode code = im->GetFriendManager()->BlackFriend(1, users);  // 1解除拉黑
         
         EXPECT_EQ("解除拉黑", "BlackFriend", code, YIMErrorcode_Success);
         std::cv_status s2 = m_cv.wait_for(lk2, std::chrono::seconds(15));
         EXPECT_NO_TIMEOUT("解除拉黑", s2);
     }
    
}



void IMTestController::leave_cases()
{
    //清理工作 退出房间等
    actionLeaveChatRoom();
    actionLogout();
    endfile();
}

void IMTestController::sendemail()
{
    YIMManager *im = YIMManager::CreateInstance();
    cocos2d::log("IM Finished! OK");

    //发邮件将本地的 报告递出去
    std::string extentsion = cocos2d::StringUtils::format("IM_version_%d", im->GetSDKVersion());
    YouMeHttpRequest::sendFile("http://106.75.7.162:8890/sendreport", ::filename(), extentsion.c_str());

	/*debug*/
    //YouMeHttpRequest::sendFile("http://106.75.7.162:8998/sendreport", ::filename(), extentsion.c_str());
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

        YIMErrorcode errcode2 = im->GetMessageManager()->SendOnlyAudioMessage(ROBOT_TARHET_ID, YIMChatType::ChatType_PrivateChat, &reqNo2);
        EXPECT_EQ("录制带翻译的语音，私聊", "SendOnlyAudioMessage", errcode2, YIMErrorcode_Success);
        if (errcode2 == YIMErrorcode_Success) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("nekomimi.mp3");
            std::this_thread::sleep_for(std::chrono::seconds(5));

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

void IMTestController::OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode,
                                    const XString& roomID, int remain, std::list<std::shared_ptr<IYIMMessage> >& messageList)
{
    EXPECT_EQ("查询房间历史消息记录成功", "QueryRoomHistoryFromServer回调", errorcode, YIMErrorcode_Success,  std::string("RoomID is ") + XStringToLocal(roomID));

    for (auto it = messageList.begin(); it != messageList.end(); ++it) {
        XUINT64 id = (*it)->GetMessageID();
        std::string comment = "";

        if ((*it)->GetMessageBody()->GetMessageType() ==MessageBodyType_TXT) {
            IYIMMessageBodyText* pMsg = static_cast<IYIMMessageBodyText*>((*it)->GetMessageBody());
            comment = XStringToUTF8(XString(pMsg->GetMessageContent()));
        }
        EXPECT_EQ("房间历史消息内容", "QueryRoomHistoryMessageFromServer", id != 0, true, comment);
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
		m_cv.notify_one();
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

// 查找用户回调
void IMTestController::OnFindUser(YIMErrorcode errorcode, std::shared_ptr<std::shared_ptr<IYIMUserBriefInfo> >& users )
{
    m_cv.notify_one();
}
// 请求添加好友
void IMTestController::OnRequestAddFriend(YIMErrorcode errorcode, const XString& userID )
{
    m_cv.notify_one();
    //cocos2d::log("请求添加好友回调cback err %d, uid %s", errorcode, userID.c_str() );
}
// 被邀请添加好友通知（需要验证）
void IMTestController::OnBeRequestAddFriendNotify(const XString& userID, const XString& comments )
{
    //cocos2d::log("被邀请添加好友通知.需要验证cback  uid %s", userID.c_str() );
    YIMErrorcode err ;
    YIMManager *im = YIMManager::CreateInstance();
    // 验证好友 0同意；1拒绝；
    err = im->GetFriendManager()->DealBeRequestAddFriend(userID.c_str(), 0);  // 同意
    //  err = im->GetFriendManager()->DealBeRequestAddFriend(userID.c_str(), 1);  // 拒绝
    // 不做操作，等待超时返回。
    EXPECT_EQ("处理好友请求", "DealBeRequestAddFriend", err, YIMErrorcode_Success);
    m_cv.notify_one();
}

// 被添加为好友通知（不需要验证）
void IMTestController::OnBeAddFriendNotify(const XString& userID, const XString& comments )
{
    m_cv.notify_one();
    //cocos2d::log("被添加为好友通知.不需要验证cback uid %s", userID.c_str() );
}

// 处理被请求添加好友结果回调
void IMTestController::OnDealBeRequestAddFriend(YIMErrorcode errorcode, const XString& userID, const XString& comments, int dealResult)
{
    m_cv.notify_one();
    //cocos2d::log("处理被请求添加好友结果cback err %d uid %s，result %d ", errorcode, userID.c_str(), dealResult);
}

// 请求添加好友结果通知
void IMTestController::OnRequestAddFriendResultNotify(const XString& userID, const XString& comments, int dealResult)
{
    m_cv.notify_one();
    //cocos2d::log("请求添加好友结果通知cback uid %s comment %s, result %d", userID.c_str(),comments.c_str() ,dealResult);
}

// 功能：删除好友结果回调
void IMTestController::OnDeleteFriend(YIMErrorcode errorcode, const XString& userID)
{
    m_cv.notify_one();
    //cocos2d::log("删除好友结果cback err %d uid %s ", errorcode, userID.c_str() );
}
// 功能：被好友删除通知
void IMTestController::OnBeDeleteFriendNotify(const XString& userID)
{
    m_cv.notify_one();
    //cocos2d::log("被好友删除通知cback uid %s", userID.c_str() );
}

// 功能：拉黑或解除拉黑好友回调
void IMTestController::OnBlackFriend(YIMErrorcode errorcode, int type, const XString& userID)
{
    m_cv.notify_one();
    //cocos2d::log("拉黑0或解除1cback err %d，type %d, uid %s", errorcode, type, userID.c_str() );
}


// 功能：查询我的好友回调
void IMTestController::OnQueryFriends(YIMErrorcode errorcode, int type, int startIndex, std::list<std::shared_ptr<IYIMUserBriefInfo> >& friends)
{
    //cocos2d::log("查询我的好友cback err %d type %d ", errorcode, type);
    
    for (auto it = friends.begin(); it != friends.end(); ++it) {
        cocos2d::log("Fri UID: %s", XStringToLocal(XString((*it)->GetUserID())).c_str() );
        cocos2d::log("Fri NName: %s", XStringToLocal(XString((*it)->GetNickname())).c_str() );
        cocos2d::log("Fri online: %d", (*it)->GetUserStatus() );
    }
    
    m_cv.notify_one();
}

// 功能：查询好友请求列表回调
void IMTestController::OnQueryFriendRequestList(YIMErrorcode errorcode, int startIndex, std::list<std::shared_ptr<IYIMFriendRequestInfo> >& requestList)
{
    YIMErrorcode err ;
    int nflag = -1;
    for (auto it = requestList.begin(); it != requestList.end(); ++it) {
        cocos2d::log("请求方UID: %s", XStringToLocal(XString((*it)->GetAskerID())).c_str());
        cocos2d::log("受邀方UID: %s", XStringToLocal(XString((*it)->GetInviteeID())).c_str());
        
        nflag = (*it)->GetStatus();  // 3 等待我验证
        if (3 == nflag) {
            
            YIMManager *im = YIMManager::CreateInstance();
            // 验证好友 0同意；1拒绝；
            err =im->GetFriendManager()->DealBeRequestAddFriend(XString((*it)->GetAskerID()).c_str() , 0);  // 同意
            //err =im->GetFriendManager()->DealBeRequestAddFriend(XString((*it)->GetAskerID()).c_str() , 1);  // 拒绝
            // 不做操作，等待超时返回。
            
            EXPECT_EQ("处理好友请求", "DealBeRequestAddFriend", err, YIMErrorcode_Success);
            //cocos2d::log("DealState: err %d",  err);
        }
    }
    
    m_cv.notify_one();
}


// 功能：查询用户信息回调
void IMTestController::OnQueryUserInfo(YIMErrorcode errorcode, const IMUserProfileInfo &userInfo)
{
    m_cv.notify_one();
 /*   if (STATUS_ONLINE == userInfo.onlineState) {
        cocos2d::log("查询用户信息cback err %d，uid %s ,%d 在线  ", errorcode, XStringToLocal(userInfo.userID).c_str(), STATUS_ONLINE);
    }
    else if (STATUS_INVISIBLE == userInfo.onlineState)
    {
        cocos2d::log("查询用户信息cback err %d，uid %s ,%d 隐身  ", errorcode, XStringToLocal(userInfo.userID).c_str(), STATUS_INVISIBLE);
    }
    else if (STATUS_OFFLINE)
    {
        cocos2d::log("查询用户信息cback err %d，uid %s ,%d 离线  ", errorcode, XStringToLocal(userInfo.userID).c_str(), STATUS_OFFLINE);
    }
    */
    
}

// 功能：设置用户信息回调
void IMTestController::OnSetUserInfo(YIMErrorcode errorcode)
{
    //cocos2d::log("设置用户信息cback err %d", errorcode);
    m_cv.notify_one();
}

// 功能：切换用户在线状态回调
void IMTestController::OnSwitchUserOnlineState(YIMErrorcode errorcode)
{
    m_cv.notify_one();
}

// 功能：设置头像回调
void IMTestController::OnSetPhotoUrl(YIMErrorcode errorcode, const XString &photoUrl)
{
    m_cv.notify_one();
}

// 功能：用户信息变更通知
void IMTestController::OnUserInfoChangeNotify(const XString& userID)
{
    EXPECT_EQ("用户资料变更通知", "回调", YIMErrorcode_Success, YIMErrorcode_Success);
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

