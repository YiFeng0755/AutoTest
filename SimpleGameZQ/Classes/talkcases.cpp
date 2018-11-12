//
//  talkcases.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/8/2.
//
//

#include "talkcases.hpp"
#include "talkservice.hpp"
#include "autocontroller.hpp"
#include "YouMeHttpRequest.hpp"


TalkCasesController::TalkCasesController()
{
    m_robotuser = ::getRandUserName_Talk();
    ::initfile();
}

TalkCasesController::~TalkCasesController()
{

}

void TalkCasesController::onPcmData(int channelNum,
                            int samplingRateHz,
                            int bytesPerSample,
                            void* data,
                            int dataSizeInByte)
{

}

void TalkCasesController::onRequestRestAPI( int requestID,
                                           const YouMeErrorCode &iErrorCode,
                                           const char* strQuery,
                                           const char*  strResult )
{

}

void TalkCasesController::onMemberChange( const char* channel, std::list<MemberChange>& listMemberChange )
{
    cocos2d::log("Channel member changed .. %s", channel);
    for (auto i = listMemberChange.begin(); i != listMemberChange.end(); ++i) {
        cocos2d::log("User ID is %s: ", (*i).userID);
    }
}

void TalkCasesController::onBroadcast(const YouMeBroadcast bc, const std::string& channel, const std::string& param1, const std::string& param2, const std::string& strContent)
{

}

//talk callback
//这里是talk的回调函数
void TalkCasesController::onEvent(const YouMeEvent event,
                          const YouMeErrorCode error,
                          const char *channel,
                          const char *param)
{
    cocos2d::log("Callback onevent !");
    //cocos2d::log("channel is %s", channel);
    switch (event) {
        case YOUME_EVENT_INIT_OK:
        {
            EXPECT_EQ("初始化成功", "初始化成功回调", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_JOIN_OK:
        case YOUME_EVENT_JOIN_FAILED:
        {
            EXPECT_EQ("加入房间失败;", "回调：YOUME_EVENT_JOIN_FAILED", error, YOUME_SUCCESS);
            //if (error == YOUME_SUCCESS)
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_RESUMED:
        {
            EXPECT_EQ("恢复通话回调", "回调：YOUME_EVENT_RESUMED", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_LEAVED_ALL:
        {
            EXPECT_EQ("离开所有房间回调;", "回调：YOUME_EVENT_LEAVED_ALL", error, YOUME_SUCCESS);

            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_BGM_FAILED:
        {
            EXPECT_EQ("背景音乐播放失败", "回调:YOUME_EVENT_BGM_FAILED", error, YOUME_ERROR_STOP_FAILED);

            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_MIC_CTR_ON:
            break;
        case YOUME_EVENT_LEAVED_ONE:
        {
            EXPECT_EQ("离开一个房间", "回调:YOUME_EVENT_LEAVED_ONE", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_PAUSED:
        {
            EXPECT_EQ("停止通话回调", "回调:YOUME_EVENT_PAUSED", error, YOUME_SUCCESS);

            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_REC_FAILED:
            break;
        case YOUME_EVENT_BGM_STOPPED:
        {
            EXPECT_EQ("停止播放背景音乐回调",
                      "YOUME_EVENT_BGM_STOPPED",
                      error,
                      YOUME_SUCCESS);

            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_INIT_FAILED:
        {
            EXPECT_EQ("初始化失败回调",
                      "回调:YOUME_EVENT_INIT_FAILED",
                      error, YOUME_SUCCESS);
        }
            break;

        case YOUME_EVENT_MIC_CTR_OFF:
        {
            EXPECT_EQ("麦克风被其他用户关闭",
                      "回调:YOUME_EVENT_MIC_CTR_OFF",
                      error, YOUME_SUCCESS);

            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_RECONNECTED:
        {
            cocos2d::log("Reconnecting to network callback ....");
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_MY_MIC_LEVEL:
            break;
        case YOUME_EVENT_OTHERS_MIC_ON:
        {
            EXPECT_EQ("其他用户麦克风打开", "回调:YOUME_EVENT_OTHERS_MIC_ON",
                      error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_RECONNECTING:
            break;
        case YOUME_EVENT_SPEAK_SUCCESS:
        {
            EXPECT_EQ("切换对指定频道讲话成功", "回调:YOUME_EVENT_SPEAK_SUCCESS", error, YOUME_SUCCESS);
            cocos2d::log("[Youme Test]Callback: switch channel to speak success!");
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_SPEAK_FAILED:
            cocos2d::log("[Youme Test]Callback: switch channel to speak failed!");
            EXPECT_EQ("切换对指定频道讲话失败", "回调:YOUME_EVENT_SPEAK_FAILED", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;

        case YOUME_EVENT_OTHERS_MIC_OFF:
        {
            EXPECT_EQ("其他用户麦克风关闭", "回调:YOUME_EVENT_OTHERS_MIC_OFF", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_SPEAKER_CTR_ON:
        {
            cocos2d::log("My speaker is open by other users");
        }
            break;
        case YOUME_EVENT_LISTEN_OTHER_ON:
        {
            
        }
            break;
        case YOUME_EVENT_OTHERS_VOICE_ON:
            cocos2d::log("Other listener is no mute. They cann talk....");
            break;
        case YOUME_EVENT_SPEAKER_CTR_OFF:
        {
            cocos2d::log("My speaker is closed by other users");
        }
            break;
        case YOUME_EVENT_LISTEN_OTHER_OFF:
            cocos2d::log("Other lisener is off:");
            break;
        case YOUME_EVENT_OTHERS_VOICE_OFF:
            cocos2d::log("Other lisener is mute: ");
            break;
        case YOUME_EVENT_OTHERS_SPEAKER_ON:
        {
            cocos2d::log("Others speaker opened! [[====]]");
            m_cv.notify_one();
        }
            break;
//        case YOUME_EVENT_SET_WHITE_USER_LIST_OK:
//        case YOUME_EVENT_SET_WHITE_USER_LIST_FAILED:
//        {
//            EXPECT_EQ("[SetUserWhiteList][Cb]", error, YOUME_ERROR_WHITE_SOMEUSER_ABNORMAL);
//            m_cv.notify_one();
//        }
        case YOUME_EVENT_OTHERS_SPEAKER_OFF:
            cocos2d::log("Others speaker closed! [[====]]");
            break;
//        case YOUME_EVENT_SET_WHITE_USER_LIST_OK:
//        {
//            EXPECT_EQ("[Set white list ok][Cb]", error, YOUME_SUCCESS);
//            is_continue = true;
//            m_cv.notify_one();
//        }
//            break;
        case YOUME_EVENT_SEND_MESSAGE_RESULT:
        {
            EXPECT_EQ("发送文本消息结果通知", "回调事件:YOUME_EVENT_SEND_MESSAGE_RESULT", error, YOUME_SUCCESS);
           
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_MESSAGE_NOTIFY:
        {
            EXPECT_EQ("收到文本消息通知", "回调事件:YOUME_EVENT_MESSAGE_NOTIFY", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }

        default:
            break;
    }
}

void TalkCasesController::actionUninit()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->unInit();

   // EXPECT_EQ("反初始化", "unInit", code, YOUME_SUCCESS);
}

void TalkCasesController::runTests()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    engine->setMemberChangeCallback(this);
    //engine->setNotifyCallback(this);
    engine->setPcmCallback(this, true);
    engine->setRestApiCallback(this);

    actionUninit();
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionInit();
        cocos2d::log("Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));

        EXPECT_EQ("是否已经初始化成功", "isInited", engine->isInited(), true);
    }
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinSingleRoom();
        cocos2d::log("[joinSingleRoom] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));

        //这只用户角色
        YouMeErrorCode code = engine->setUserRole(YOUME_USER_COMMANDER);

        EXPECT_EQ("设置用户说话时的角色", "setUserRole", code, YOUME_SUCCESS);

        EXPECT_EQ("获取用户说话时的角色", "getUserRole", engine->getUserRole(), YOUME_USER_COMMANDER) ;
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionLeaveSingleChannel();

        cocos2d::log("[LeaveSingleChannel] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }


    /**
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinMultiRoom();
        is_continue = false;
        cocos2d::log("[LeaveSingleChannel] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionLeaveMultiMode();
        is_continue = false;
        cocos2d::log("[LeaveSingleChannel] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
     **/
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinRoomZhubo();
        cocos2d::log("[LeaveSingleChannel] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    // 扬声器控制
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        engine->setSpeakerMute(true);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        bool status1 = engine->getSpeakerMute(); //获取扬声器状态
        EXPECT_EQ("获取静音状态,关闭扬声器", "getSpeakerMute", status1, true);

        engine->setSpeakerMute(false); //打开扬声器
        std::this_thread::sleep_for(std::chrono::seconds(3));
        bool status2 = engine->getSpeakerMute(); //获取扬声器状态
        EXPECT_EQ("设置静音状态，打开扬声器", "getSpeakerMute", status2, false);
    }
    //输出到扬声器
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->setOutputToSpeaker(false);
        EXPECT_EQ("设置是否输出到扬声器:否", "setOutputToSpeaker", code, YOUME_SUCCESS);

        YouMeErrorCode code1 = engine->setOutputToSpeaker(true);
        EXPECT_EQ("设置是否输出到扬声器：是", "setOutputToSpeaker", code1, YOUME_SUCCESS);
    }

    //麦克风控制
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        engine->setMicrophoneMute(true);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        EXPECT_EQ("麦克风控制：关闭麦克风", "setMicrophoneMute", engine->getMicrophoneMute(), true);

        engine->setMicrophoneMute(false); //打开麦克风
        std::this_thread::sleep_for(std::chrono::seconds(3));
        EXPECT_EQ("麦克风控制：打开麦克风", "setMicrophoneMute", engine->getMicrophoneMute(), false);

    }

    //音量控制
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("nekomimi.mp3").c_str(), true);
        if (code == YOUME_SUCCESS) {
            for (int i = 0; i < 10; ++i) {
                engine->setVolume(i + 5);
                std::this_thread::sleep_for(std::chrono::microseconds(50));
            }
        }
        engine->setVolume(0);
        EXPECT_EQ("音量控制：设置音量为0，持续5s", "setVolume", (int)engine->getVolume(), 0);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        engine->setVolume(100);
        EXPECT_EQ("音量控制：设置音量为100，持续5s", "setVolume", (int)engine->getVolume(), 100);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        engine->setVolume(45);
        EXPECT_EQ("音量控制：设置音量为45，持续5s", "setVolume", (int)engine->getVolume(), 45);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    //混响控制
    {

        YouMeErrorCode code1 = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("mangetunoyoruniodore.mp3").c_str(),
                                                           true);
        EXPECT_EQ("播放背景音乐", "playBackgroundMusic", code1, YOUME_SUCCESS);

        for (int i = 0; i < 3; ++i) {
            YouMeErrorCode code = engine->setReverbEnabled(false);
            EXPECT_EQ("混响控制：打开混响","setReverbEnabled", code, YOUME_SUCCESS);

            std::this_thread::sleep_for(std::chrono::seconds(15));
            YouMeErrorCode code2 = engine->setReverbEnabled(true);
            EXPECT_EQ("混响控制：打开混响", "setReverbEnabled", code2, YOUME_SUCCESS);
        }
    }

    {
        IYouMeVoiceEngine* engine =IYouMeVoiceEngine::getInstance();
        engine->setAutoSendStatus(true);
        EXPECT_EQ("设置是否通知其他人自己的开关麦克风和扬声器的状态:是", "setAutoSendStatus", 0, 0);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        engine->setAutoSendStatus(false);
        EXPECT_EQ("设置是否通知其他人自己的开关麦克风和扬声器的状态:否", "setAutoSendStatus", 0, 0);
    }
    //网络
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        engine->setUseMobileNetworkEnabled(true);
        EXPECT_EQ("启用移动网络", "setUseMobileNetworkEnabled", 0, 0);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        engine->setUseMobileNetworkEnabled(false);
        EXPECT_EQ("禁用移动网络", "setUseMobileNetworkEnabled", 0, 0);
    }

    //通话控制
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->pauseChannel();
        EXPECT_EQ("通话控制：暂停通话", "pauseChannel",  code, YOUME_SUCCESS);
        cocos2d::log("Pause Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    // 恢复通话
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->resumeChannel();
        EXPECT_EQ("通话控制：恢复通话", "resumeChannel", code, YOUME_SUCCESS);
        cocos2d::log("Resume Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    //背景音乐播放
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        std::unique_lock<std::mutex> lk(m_mutex);

        YouMeErrorCode code = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("mangetunoyoruniodore.mp3").c_str(),
                                                          true);
        engine->setVolume(10);
        EXPECT_EQ("背景音乐播放", "playBackgroundMusic", code, YOUME_SUCCESS);
        cocos2d::log("playBackgroundMusic Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));

        std::this_thread::sleep_for(std::chrono::seconds(15));
        YouMeErrorCode stopcode = engine->stopBackgroundMusic();
        EXPECT_EQ("停止背景音乐播放", "stopBackgroundMusic", stopcode, YOUME_SUCCESS);
    }

    //监听设置
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->setHeadsetMonitorOn(true);
        EXPECT_EQ("监听控制:打开监听", "setHeadsetMonitorOn", code, YOUME_SUCCESS);

        std::this_thread::sleep_for(std::chrono::seconds(15));
        YouMeErrorCode code1 = engine->setHeadsetMonitorOn(false);
        EXPECT_EQ("监听控制：关闭监听", "setHeadsetMonitorOn", code1, YOUME_SUCCESS);
    }

    //控制别人的麦克风
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->setOtherMicMute("1234", true);
        EXPECT_EQ("控制他人麦克风，让用户静音", "setHeadsetMonitorOn", code, YOUME_SUCCESS);
        std::unique_lock<std::mutex> lk(m_mutex);
        cocos2d::log("setOtherMicMute Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));

        lk.unlock();

        YouMeErrorCode code1 = engine->setOtherMicMute("1234", false);
        EXPECT_EQ("控制他人麦克风，取消用户静音", "setHeadsetMonitorOn", code1, YOUME_SUCCESS);
        std::unique_lock<std::mutex> lk1(m_mutex);
        cocos2d::log("setOtherMicMute Wait for callback...");
        m_cv.wait_for(lk1, std::chrono::seconds(10));
    }

    //广播
    {
        //YouMeErrorCode  sendMessage( const char* pChannelID,  const char* pContent, int* requestID );
        int reqNumber;
        YouMeErrorCode code = engine->sendMessage(TALK_CHAT_ROOM,
                                                "Test Test Test Test Test", &reqNumber);
        EXPECT_EQ("发出广播消息", "sendMessage", code, YOUME_SUCCESS);
        std::unique_lock<std::mutex> lk(m_mutex);
        cocos2d::log("Waiting for callback! .... ");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    //查频道用户列表
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        //YouMeErrorCode getChannelUserList( const char*  channelID, int maxCount, bool notifyMemChange );
        YouMeErrorCode code = engine->getChannelUserList(TALK_CHAT_ROOM, -1, true);

        EXPECT_EQ("查询频道用户列表:全部","getChannelUserList", code, YOUME_SUCCESS);
            //m_cv.wait_for(lk, std::chrono::seconds(10));

        //lk.unlock();

        //std::unique_lock<std::mutex> lk1(m_mutex);
        YouMeErrorCode code1 = engine->getChannelUserList(TALK_CHAT_ROOM, 10, true);

            EXPECT_EQ("查询频道用户列表:10个用户, 其他用户进出房间时通知", "getChannelUserList", code1, YOUME_SUCCESS);
            //m_cv.wait_for(lk1, std::chrono::seconds(10));


        //lk1.unlock();
        //std::unique_lock<std::mutex> lk2(m_mutex);
        YouMeErrorCode code2 = engine->getChannelUserList(TALK_CHAT_ROOM, 10, false);

            EXPECT_EQ("查询频道用户列表:10个用户, 其他用户进出房间时不通知", "[GetChannelUserList][call]", code2, YOUME_SUCCESS);
           // m_cv.wait_for(lk2, std::chrono::seconds(10));

    }

    //踢人
    {
//        std::unique_lock<std::mutex> lk(m_mutex);
//        YouMeErrorCode code = engine->kickOtherFromChannel(USER_ID_B, TALK_CHAT_ROOM, 10);
//        if (code == YOUME_SUCCESS) {
//            EXPECT_EQ("[KickOtherFromChannel call]", code,      YOUME_SUCCESS); //机器人造出来后再恢复
//            m_cv.wait_for(lk, std::chrono::seconds(10));
//        }
    }
    //IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    engine->unInit();
    cocos2d::log("Finished! Single Mode");

    //多频道模式
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionInit();
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    //加入多频道主播模式
    {
        std::unique_lock<std::mutex> lk(m_mutex);

        YouMeErrorCode code = engine->joinChannelMultiMode(m_robotuser.c_str(),
                                                               "20000001", YOUME_USER_HOST);
        EXPECT_EQ("多频道主播模式下离开房间", "joinChannelMultiMode", code, YOUME_SUCCESS);

        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    //离开频道
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->leaveChannelMultiMode("20000001");
        EXPECT_EQ("多频道模式下离开房间", "leaveChannelMultiMode", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    //leaveAll
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->leaveChannelAll();
        EXPECT_EQ("离开所有房间", "LeaveChannelAll", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    //再次加入
    {
        std::unique_lock<std::mutex> lk(m_mutex);
 //       int base_num = 100000;
//        for (int i = 0; i < 100; ++i) {
//            YouMeErrorCode code = engine->joinChannelMultiMode(m_robotuser.c_str(),
//                                                               std::to_string(base_num + i + 1).c_str());
//            EXPECT_EQ("[Join multi room]", code, YOUME_SUCCESS);
//        }
        YouMeErrorCode code = engine->joinChannelMultiMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
        EXPECT_EQ("多频道模式下加入房间", "joinChannelMultiMode", code, YOUME_SUCCESS);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_EQ("多频道模式下加入房间【Callback】", "joinChannelMultiMode(cb)", int(s), int(std::cv_status::no_timeout));

        YouMeErrorCode code1 = engine->joinChannelMultiMode(m_robotuser.c_str(), "robot001", YOUME_USER_TALKER_FREE);
        EXPECT_EQ("多频道模式下加入房间", "joinChannelMultiMode", code1, YOUME_SUCCESS);
        std::cv_status s1 = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_EQ("多频道模式下加入房间【Callback】", "joinChannelMultiMode(cb)", int(s1),
                  int(std::cv_status::no_timeout));
    }

    //选择频道说话
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->speakToChannel(TALK_CHAT_ROOM);
        EXPECT_EQ("多频道模式下，选择一个频道说话", "speakToChannel", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        std::vector<std::string> userlist;
        userlist.push_back(std::string("1234"));
        userlist.push_back(std::string("1235"));
        YouMeErrorCode code = engine->setWhiteUserList(TALK_CHAT_ROOM, userlist);
        EXPECT_EQ("设置用户白名单", "setWhiteUserList", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    engine->unInit();
    cocos2d::log("Finished!");
    endfile();
    //发邮件将本地的 报告递出去
    std::string strVer = verToString(engine->getSDKVersion());
    std::string extentsion = cocos2d::StringUtils::format("Talk_%s", strVer.c_str());
    YouMeHttpRequest::sendFile("http://106.75.7.162:8999/sendreport", ::filename(), extentsion.c_str());

}

void TalkCasesController::actionInit()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->init(this, TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");

    EXPECT_EQ("初始化Talk引擎", "init", code, YOUME_SUCCESS);
}

void TalkCasesController::actionJoinSingleRoom()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->joinChannelSingleMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    EXPECT_EQ("以自由角色、单频道模式加入房间", "joinChannelSingleMode", code, YOUME_SUCCESS);
}

void TalkCasesController::actionJoinMultiRoom()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->joinChannelMultiMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    EXPECT_EQ("加入多房间", "joinChannelMultiMode", code, YOUME_SUCCESS);
}

void TalkCasesController::actionLeaveMultiMode()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->leaveChannelMultiMode(TALK_CHAT_ROOM);
    EXPECT_EQ("离开某个房间", "leaveChannelMultiMode", code, YOUME_SUCCESS);
}

void TalkCasesController::actionLeaveSingleChannel()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->leaveChannelAll();
    EXPECT_EQ("离开所有房间", "leaveChannelAll", code, YOUME_SUCCESS);
}

void TalkCasesController::actionJoinRoomZhubo()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->joinChannelSingleMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_HOST);
    EXPECT_EQ("以主播模式加入单个房间", "joinChannelSingleMode", code, YOUME_SUCCESS);
}


