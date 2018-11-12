//
//  talkcases.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/8/2.
//
//

#include "talkcases.hpp"
#include "YouMeHttpRequest.hpp"
#include <SimpleAudioEngine.h>

TalkCasesController::TalkCasesController()
{
    m_robotuser = YoumeUtil::getRandUserName();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SetServerMode(SERVER_MODE_TEST);
#endif

    IYouMeVoiceEngine::getInstance()->init(this, "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804", "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=", RTC_CN_SERVER, "");
}

TalkCasesController::~TalkCasesController()
{
    if (_run_thread.joinable())
        _run_thread.join();
}

void TalkCasesController::onPcmData(int channelNum, int samplingRateHz, int bytesPerSample, void *data, int dataSizeInByte)
{
}

void TalkCasesController::onRequestRestAPI(int requestID,
                                           const YouMeErrorCode &iErrorCode,
                                           const char* strQuery,
                                           const char*  strResult)
{
}

void TalkCasesController::onBroadcast(const YouMeBroadcast bc, const char *channel, const char *param1, const char *param2, const char *strContent)
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
        case YOUME_EVENT_JOIN_OK: {
            EXPECT_EQ("加入房间成功", "YOUME_EVENT_JOIN_OK", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;
        }
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
            cocos2d::log("断网重连成功");

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
        {
            EXPECT_EQ("网络在重连", "回调： YOUME_EVENT_RECONNECTING", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_SPEAK_SUCCESS:
        {
            EXPECT_EQ("切换对指定频道讲话成功", "回调:YOUME_EVENT_SPEAK_SUCCESS", error, YOUME_SUCCESS);
            cocos2d::log("[Youme Test]Callback: switch channel to speak success!");
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_SPEAK_FAILED: {
            cocos2d::log("[Youme Test]Callback: switch channel to speak failed!");
            EXPECT_EQ("切换对指定频道讲话失败", "回调:YOUME_EVENT_SPEAK_FAILED", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;
        }

        case YOUME_EVENT_OTHERS_MIC_OFF:{
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
        case YOUME_EVENT_OTHERS_SPEAKER_OFF:
            cocos2d::log("Others speaker closed! [[====]]");
            break;
        case YOUME_EVENT_SEND_MESSAGE_RESULT:
        {
            EXPECT_EQ("发送文本消息结果通知", "回调: YOUME_EVENT_SEND_MESSAGE_RESULT", error, YOUME_SUCCESS);
           
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_MESSAGE_NOTIFY:
        {
            EXPECT_EQ("收到文本消息通知", "回调: YOUME_EVENT_MESSAGE_NOTIFY", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_INVITEMIC_SETOPT_OK:
        {
            //主播身份才能连麦设置成功
            EXPECT_EQ("连麦设置成功，回调", "回调：YOUME_EVENT_INVITEMIC_SETOPT_OK", error, YOUME_SUCCESS);
            cocos2d::log("连麦设置成功了");
            m_cv.notify_one();
            break;
        }

        case YOUME_EVENT_INVITEMIC_SETOPT_FAILED:
        {
            EXPECT_EQ("连麦设置失败", "回调：YOUME_EVENT_INVITEMIC_SETOPT_FAILED", error, YOUME_SUCCESS);
            cocos2d::log("连麦设置失败了");
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_GRABMIC_START_OK: //33
        {
            EXPECT_EQ("抢麦活动开始", "回调：YOUME_EVENT_GRABMIC_START_OK", error, YOUME_SUCCESS);
            cocos2d::log("抢麦活动开始");
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_GRABMIC_STOP_OK:
        {
            EXPECT_EQ("抢麦活动停止", "回调：YOUME_EVENT_GRABMIC_STOP_OK", error, YOUME_SUCCESS);
            cocos2d::log("抢麦活动停止");
            m_cv.notify_one();
            break;
        }

        case YOUME_EVENT_INVITEMIC_REQUEST_OK:
        {
            EXPECT_EQ("请求连麦成功", "回调：YOUME_EVENT_INVITEMIC_REQUEST_OK", error, YOUME_SUCCESS);
            cocos2d::log("请求连麦成功");
            m_cv.notify_one();
            break;
        }

        case YOUME_EVENT_GRABMIC_STOP_FAILED:
        {
            EXPECT_EQ("请求连麦失败", "回调：YOUME_EVENT_GRABMIC_STOP_FAILED", error, YOUME_SUCCESS);
            cocos2d::log("请求连麦失败");
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_GRABMIC_REQUEST_WAIT:
        {
            EXPECT_EQ("进入抢麦等待队列（权重模式）", "回调：YOUME_EVENT_GRABMIC_REQUEST_WAIT", error, YOUME_SUCCESS);
        }
            break;
        case YOUME_EVENT_INVITEMIC_RESPONSE_OK:
        {
            cocos2d::log("响应连麦成功");
        }
            break;

        case YOUME_EVENT_INVITEMIC_RESPONSE_FAILED:
        {
            cocos2d::log("响应连麦失败");
        }
            break;
        case YOUME_EVENT_INVITEMIC_STOP_OK:
        {
            cocos2d::log("停止连麦成功");
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_INVITEMIC_STOP_FAILED:
        {
            cocos2d::log("停止连麦失败");
            EXPECT_EQ("停止连麦失败", "回调：YOUME_EVENT_INVITEMIC_STOP_FAILED", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_INVITEMIC_CAN_TALK:
        {
            cocos2d::log("双方可以通话了");
            EXPECT_EQ("双方可以通话了，回调", "回调：YOUME_EVENT_INVITEMIC_CAN_TALK", error, YOUME_SUCCESS);
            m_cv.notify_one();
            break;
        }
        case YOUME_EVENT_INVITEMIC_CANNOT_TALK :
        {
            cocos2d::log("双方不能通话了");
            EXPECT_EQ("通话停止回调", "回调：YOUME_EVENT_INVITEMIC_CANNOT_TALK", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_INVITEMIC_NOTIFY_CALL:
        {
            cocos2d::log("有人请求与你连麦");
            EXPECT_EQ("有人请求与你连麦", "回调：YOUME_EVENT_INVITEMIC_NOTIFY_CALL", error, YOUME_SUCCESS);
        }
            break;
        case YOUME_EVENT_INVITEMIC_NOTIFY_ANSWER:
        {
            cocos2d::log("对方对你的连麦请求作出了响应");
            EXPECT_EQ("对方对你的连麦请求作出了响应", "回调：YOUME_EVENT_INVITEMIC_NOTIFY_CALL", error, YOUME_SUCCESS);
        }
            break;

        case YOUME_EVENT_INVITEMIC_NOTIFY_CANCEL:
        {
            cocos2d::log("连麦过程中，对方结束了连麦或者连麦时间到");
            EXPECT_EQ("连麦过程中，对方结束了连麦或者连麦时间到", "回调：YOUME_EVENT_INVITEMIC_NOTIFY_CANCEL", error, YOUME_SUCCESS);
            break;
        }
		//case YOUME_EVENT_REC_PERISSION_STATUS:
		//{
		//}
		//	break;
        case YOUME_EVENT_GRABMIC_NOTIFY_START:
        {
            cocos2d::log("抢麦活动开始");
            EXPECT_EQ("抢麦活动开始", "回调：YOUME_EVENT_GRABMIC_NOTIFY_START", error, YOUME_SUCCESS);
        }
            break;
        case YOUME_EVENT_GRABMIC_NOTIFY_STOP:
        {
            cocos2d::log("抢麦活动结束");
            EXPECT_EQ("抢麦活动结束", "回调：YOUME_EVENT_GRABMIC_NOTIFY_STOP", error, YOUME_SUCCESS);
        }
            break;
        case YOUME_EVENT_LOCAL_SPEAKER_OFF:
        {
            cocos2d::log("我方扬声器关闭");
            EXPECT_EQ("我方扬声器关闭", "回调：YOUME_EVENT_LOCAL_SPEAKER_OFF", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;
        case YOUME_EVENT_LOCAL_SPEAKER_ON:
        {
            cocos2d::log("我方扬声器打开");
            EXPECT_EQ("我方扬声器打开", "回调：YOUME_EVENT_LOCAL_SPEAKER_ON", error, YOUME_SUCCESS);
            m_cv.notify_one();
        }
            break;

        case YOUME_EVENT_KICK_NOTIFY:
        {
            cocos2d::log("recv: %s", param);
            break;
        }

        default:
            break;
    }
}

void TalkCasesController::actionUninit()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->unInit();
    
    EXPECT_EQ("反初始化", "unInit", code, YOUME_SUCCESS);
}

void TalkCasesController::startTest()
{
	//YoumeUtil::clear_sdk_log_file();
    _run_thread = std::thread(&TalkCasesController::testThreadFunc, this);
    _run_thread.join();
    cocos2d::log("Finished!");
}

void TalkCasesController::cases_init()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    engine->setMemberChangeCallback(this);

    engine->setNotifyCallback(this);
    engine->setRestApiCallback(this);
    engine->setPcmCallback(this, true);

	creatfile();
    initfile();
    if (engine->isInited())
        actionUninit();

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionInit();
        //cocos2d::log("Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_EQ("是否已经初始化成功", "isInited", engine->isInited(), true);
    }
}

void TalkCasesController::cases_room_manager()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinSingleRoom();
        cocos2d::log("[joinSingleRoom] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));

        //设置用户角色
        YouMeErrorCode code = engine->setUserRole(YOUME_USER_TALKER_FREE);

        EXPECT_EQ("设置用户说话时的角色", "setUserRole", code, YOUME_SUCCESS);
        EXPECT_EQ("获取用户说话时的角色", "getUserRole", engine->getUserRole(), YOUME_USER_TALKER_FREE) ;
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionLeaveSingleChannel();

        cocos2d::log("[LeaveSingleChannel] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinRoomZhubo();
        cocos2d::log("[LeaveSingleChannel] Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

	{
		std::unique_lock<std::mutex> lk(m_mutex);
		actionLeaveSingleChannel();

		cocos2d::log("[LeaveSingleChannel] Wait for callback...");
		m_cv.wait_for(lk, std::chrono::seconds(10));
	}


    //异常参数设置
    {
        YouMeErrorCode code1 = engine->joinChannelSingleMode("", "", YOUME_USER_TALKER_FREE);
        EXPECT_EQ("加入单个房间，用户参数传入空字符串", "joinChannelSingleMode", code1, YOUME_ERROR_INVALID_PARAM);

        YouMeErrorCode code2 = engine->joinChannelSingleMode(nullptr, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
        EXPECT_EQ("加入单个房间，用户参数传入空指针", "joinChannelSingleMode", code2, YOUME_ERROR_INVALID_PARAM);

        YouMeErrorCode code3 = engine->joinChannelSingleMode(m_robotuser.c_str(), nullptr, YOUME_USER_TALKER_FREE);
        EXPECT_EQ("加入单个房间，房间参数传入空指针", "joinChannelSingleMode", code3, YOUME_ERROR_INVALID_PARAM);

    }
}

void TalkCasesController::cases_voice_controll()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
	engine->joinChannelSingleMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_HOST);
    // 扬声器控制
    {
        //关闭扬声器
        std::unique_lock<std::mutex> lk(m_mutex);
        engine->setSpeakerMute(true);
        m_cv.wait_for(lk, std::chrono::seconds(10));
        bool status1 = engine->getSpeakerMute(); //获取扬声器状态
        EXPECT_EQ("获取静音状态,关闭扬声器", "getSpeakerMute", status1, true);
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        engine->setSpeakerMute(false); //打开扬声器
        m_cv.wait_for(lk, std::chrono::seconds(10));
        std::this_thread::sleep_for(std::chrono::seconds(10));
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
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        EXPECT_EQ("麦克风控制：关闭麦克风", "setMicrophoneMute", engine->getMicrophoneMute(), true);

        engine->setMicrophoneMute(false); //打开麦克风
        std::this_thread::sleep_for(std::chrono::seconds(10));
        EXPECT_EQ("麦克风控制：打开麦克风", "setMicrophoneMute", engine->getMicrophoneMute(), false);

    }

    //音量控制
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("nekomimi.mp3").c_str());
//        YouMeErrorCode code = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("nekomimi.mp3").c_str(), true);

        for (int i = 0; i < 5; ++i) {
            engine->setVolume(i + 5);
            std::this_thread::sleep_for(std::chrono::microseconds(50));
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

        for (int i = 0; i < 3; ++i) {
            YouMeErrorCode code1 = engine->setReverbEnabled(false);
            EXPECT_EQ("混响控制：打开混响","setReverbEnabled", code1, YOUME_SUCCESS);

            std::this_thread::sleep_for(std::chrono::seconds(15));
            YouMeErrorCode code2 = engine->setReverbEnabled(true);
            EXPECT_EQ("混响控制：打开混响", "setReverbEnabled", code2, YOUME_SUCCESS);
        }
    }

    {
        engine->setAutoSendStatus(true);
        EXPECT_EQ("设置是否通知其他人自己的开关麦克风和扬声器的状态:是", "setAutoSendStatus", 0, 0);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        engine->setAutoSendStatus(false);
        EXPECT_EQ("设置是否通知其他人自己的开关麦克风和扬声器的状态:否", "setAutoSendStatus", 0, 0);
    }
    //网络
    {
        engine->setUseMobileNetworkEnabled(true);
        EXPECT_EQ("启用移动网络", "setUseMobileNetworkEnabled", 0, 0);
		bool status1 = engine->getUseMobileNetworkEnabled();
		EXPECT_EQ("获取移动网络状态，开", "getUseMobileNetworkEnabled", status1, true);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        engine->setUseMobileNetworkEnabled(false);
        EXPECT_EQ("禁用移动网络", "setUseMobileNetworkEnabled", 0, 0);
		bool status2 = engine->getUseMobileNetworkEnabled();
		EXPECT_EQ("获取移动网络状态，关", "getUseMobileNetworkEnabled", status2, false);
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

        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
	engine->leaveChannelAll();
}

void TalkCasesController::cases_others()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
	engine->joinChannelSingleMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_HOST);
    //背景音乐播放
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("mangetunoyoruniodore.mp3").c_str(),
                                                          true);
        engine->setVolume(10);
        EXPECT_EQ("背景音乐播放", "playBackgroundMusic", code, YOUME_SUCCESS);
        cocos2d::log("playBackgroundMusic Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(10));

		bool playingcode = engine->isBackgroundMusicPlaying();
		EXPECT_EQ("背景音乐播放,正在播放", "playingBackgroundMusic", playingcode, true);

		YouMeErrorCode setcode = engine->setBackgroundMusicVolume(50);
		EXPECT_EQ("设置背景音乐音量", "setBackgroundMusicVolume", setcode, YOUME_SUCCESS);
		std::this_thread::sleep_for(std::chrono::seconds(3));

		YouMeErrorCode pausecode = engine->pauseBackgroundMusic();
		EXPECT_EQ("暂停背景音乐播放", "pauseBackgroundMusic", pausecode, YOUME_SUCCESS);
		bool noplayingcode = engine->isBackgroundMusicPlaying();
		EXPECT_EQ("背景音乐播放,没有播放", "noplayingBackgroundMusic", noplayingcode, false);

		YouMeErrorCode resumecode = engine->resumeBackgroundMusic();
		EXPECT_EQ("恢复背景音乐播放", "resumeBackgroundMusic", resumecode, YOUME_SUCCESS);

        //std::this_thread::sleep_for(std::chrono::seconds(15));
        YouMeErrorCode stopcode = engine->stopBackgroundMusic();
        EXPECT_EQ("停止背景音乐播放", "stopBackgroundMusic", stopcode, YOUME_SUCCESS);
    }

    //监听设置和各种参数设置
    {
        YouMeErrorCode code = engine->setHeadsetMonitorOn(true);
        EXPECT_EQ("监听控制:监听麦克风，监听背景音乐", "setHeadsetMonitorOn", code, YOUME_SUCCESS);

        std::this_thread::sleep_for(std::chrono::seconds(15));
        YouMeErrorCode code1 = engine->setHeadsetMonitorOn(false);
        EXPECT_EQ("监听控制：不监听麦克风，监听背景音乐", "setHeadsetMonitorOn", code1, YOUME_SUCCESS);
		
		std::this_thread::sleep_for(std::chrono::seconds(10));
		YouMeErrorCode code2 = engine->setHeadsetMonitorOn(false,false);
        EXPECT_EQ("监听控制：不监听麦克风，不监听背景音乐", "setHeadsetMonitorOn", code2, YOUME_SUCCESS);
		
		YouMeErrorCode code3 = engine->setHeadsetMonitorOn(true,false);
        EXPECT_EQ("监听控制：监听麦克风，不监听背景音乐", "setHeadsetMonitorOn", code3, YOUME_SUCCESS);

		YouMeErrorCode code4 = engine->setReverbEnabled(true);
		EXPECT_EQ("开启主播混响模式", "setReverbEnabled", code4, YOUME_SUCCESS);

		YouMeErrorCode code5 = engine->setReverbEnabled(false);
		EXPECT_EQ("关闭主播混响模式", "setReverbEnabled", code5, YOUME_SUCCESS);

		YouMeErrorCode code6 = engine->setVadCallbackEnabled(true);
		EXPECT_EQ("开启语音检测回调", "setVadCallbackEnabled", code6, YOUME_SUCCESS);

		YouMeErrorCode code7 = engine->setVadCallbackEnabled(true);
		EXPECT_EQ("关闭语音检测回调", "setVadCallbackEnabled", code7, YOUME_SUCCESS);

		YouMeErrorCode code8 = engine->setMicLevelCallback(10);
		EXPECT_EQ("开启讲话音量回调", "setMicLevelCallback", code8, YOUME_SUCCESS);

		YouMeErrorCode code9 = engine->setMicLevelCallback(0);
		EXPECT_EQ("关闭讲话音量回调", "setMicLevelCallback", code9, YOUME_SUCCESS);

		YouMeErrorCode code10 = engine->setReleaseMicWhenMute(true);
		EXPECT_EQ("麦克风静音时，释放麦克风设备", "setReleaseMicWhenMute", code10, YOUME_SUCCESS);

		YouMeErrorCode code11 = engine->setReleaseMicWhenMute(false);
		EXPECT_EQ("麦克风静音时，不释放麦克风设备", "setReleaseMicWhenMute", code11, YOUME_SUCCESS);

		YouMeErrorCode code12 = engine->pauseChannel();
		EXPECT_EQ("暂停通话", "pauseChannel", code12, YOUME_SUCCESS);

		YouMeErrorCode code13 = engine->resumeChannel();
		EXPECT_EQ("恢复通话", "resumeChannel", code13, YOUME_SUCCESS);

		float volume = engine->getSoundtouchPitchSemiTones();
		bool code14;
		if (volume >= -12 && volume <= 12)
		{
			bool code14 = true;
		}
		else
		{
			bool code14 = false;
		}
		EXPECT_EQ("获取变声音调", "getSoundtouchPitchSemiTones", code14, true);

		YouMeErrorCode code15 = engine->setSoundtouchPitchSemiTones(0);
		EXPECT_EQ("设置变声音调", "setSoundtouchPitchSemiTones", code15, YOUME_SUCCESS);


    }

    //控制别人的麦克风
    {
        YouMeErrorCode code = engine->setOtherMicMute("1234", true);
        EXPECT_EQ("控制他人麦克风，让用户静音", "setOtherMicMute", code, YOUME_SUCCESS);
        std::unique_lock<std::mutex> lk(m_mutex);
        cocos2d::log("setOtherMicMute Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(5));

    }
    {

        YouMeErrorCode code1 = engine->setOtherMicMute("1234", false);
        EXPECT_EQ("控制他人麦克风，取消用户静音", "setOtherMicMute", code1, YOUME_SUCCESS);
        std::unique_lock<std::mutex> lk1(m_mutex);
        cocos2d::log("setOtherMicMute Wait for callback...");
        m_cv.wait_for(lk1, std::chrono::seconds(5));
    }

	//控制别人的扬声器
	{
		YouMeErrorCode code = engine->setOtherSpeakerMute("1234", true);
		EXPECT_EQ("控制他人扬声器，屏蔽他人语音", "setOtherSpeakerMute", code, YOUME_SUCCESS);
		std::unique_lock<std::mutex> lk(m_mutex);
		cocos2d::log("setOtherMicMute Wait for callback...");
		m_cv.wait_for(lk, std::chrono::seconds(5));

	}
	{

		YouMeErrorCode code1 = engine->setOtherSpeakerMute("1234", false);
		EXPECT_EQ("控制他人扬声器，取消屏蔽他人语音", "setOtherMicMute", code1, YOUME_SUCCESS);
		std::unique_lock<std::mutex> lk1(m_mutex);
		cocos2d::log("setOtherMicMute Wait for callback...");
		m_cv.wait_for(lk1, std::chrono::seconds(5));
	}

    //广播
    {
        //YouMeErrorCode  sendMessage( const char* pChannelID,  const char* pContent, int* requestID );
        int reqNumber;
        YouMeErrorCode code = engine->sendMessage(TALK_CHAT_ROOM,
                                                  "Test Test Test Test Test", &reqNumber);
        EXPECT_EQ("发出广播消息", "sendMessage", code, YOUME_SUCCESS);
    }

	//设置白名单
	{
		std::vector<std::string> vecWhiteUserList;
		vecWhiteUserList.push_back("liuyang");
		vecWhiteUserList.push_back(m_robotuser.c_str());
		YouMeErrorCode code = engine->setWhiteUserList(TALK_CHAT_ROOM, vecWhiteUserList);
		EXPECT_EQ("对频道设置白名单用户", "setWhiteUserList", code, YOUME_SUCCESS);
	}
	//麦克风释放
	{
		bool code = engine->releaseMicSync();
		if (code = true){
			EXPECT_EQ("同步完成麦克风释放,成功", "releaseMicSync", 0, 0);
		}
		else{
			EXPECT_EQ("同步完成麦克风释放,失败", "releaseMicSync", 0, 0);
		}
	}

	//恢复麦克风到释放前的状态
	{
		bool code = engine->resumeMicSync();
		if (code = true){
			EXPECT_EQ("恢复麦克风到释放前的状态,成功", "resumeMicSync", 0, 0);
		}
		else{
			EXPECT_EQ("恢复麦克风到释放前的状态,失败", "resumeMicSync", 0, 0);
		}
	}
	engine->leaveChannelAll();
}

void TalkCasesController::cases_multi_mode()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    engine->unInit();
    cocos2d::log("Finished! Single Mode;Begin muti mode");
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
                                                           TALK_CHAT_ROOM, YOUME_USER_HOST);
        EXPECT_EQ("多频道主播模式下离开房间", "joinChannelMultiMode", code, YOUME_SUCCESS);

        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    //离开频道
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->leaveChannelMultiMode(TALK_CHAT_ROOM);
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
        YouMeErrorCode code = engine->joinChannelMultiMode(m_robotuser.c_str(), TALK_CHAT_ROOM
                                                           , YOUME_USER_TALKER_FREE);
        EXPECT_EQ("多频道模式下加入房间", "joinChannelMultiMode", code, YOUME_SUCCESS);
        std::cv_status s = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_EQ("", "joinChannelMultiMode(cb)", int(s), int(std::cv_status::no_timeout));
        EXPECT_NO_TIMEOUT("多频道模式下加入房间【Callback】", s);
    }
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code1 = engine->joinChannelMultiMode(m_robotuser.c_str(), "robot001",
                                                            YOUME_USER_TALKER_FREE);
        EXPECT_EQ("多频道模式下加入房间", "joinChannelMultiMode", code1, YOUME_SUCCESS);
        std::cv_status s1 = m_cv.wait_for(lk, std::chrono::seconds(10));
        EXPECT_NO_TIMEOUT("多频道模式下加入房间【Callback】", s1);
    }

    //选择频道说话
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->speakToChannel(TALK_CHAT_ROOM);
        EXPECT_EQ("多频道模式下，选择一个频道说话", "speakToChannel", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
}

void TalkCasesController::cases_micgrab()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    //多频道模式
    if (engine->isInited())
        engine->unInit();

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionInit();
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinRoomZhubo();
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    /*抢麦放麦*/
    {
        //抢麦相关设置
        /*
         */
        //std::unique_lock<std::mutex> lk(m_mutex);
        //暂时看不出有回调
        YouMeErrorCode code = engine->setGrabMicOption(TALK_CHAT_ROOM, 1, 10, 30, 3); //
        // 1先到先得
        EXPECT_EQ("抢麦设置", "setGrabMicOption", code, YOUME_SUCCESS);
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->startGrabMicAction(TALK_CHAT_ROOM, "Begin to grab mic action");
        EXPECT_EQ("开始抢麦活动", "startGrabMicAction", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code = engine->stopGrabMicAction(TALK_CHAT_ROOM, "Begin to grab mic action");
        EXPECT_EQ("停止抢麦活动", "stopGrabMicAction", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    {
        YouMeErrorCode code1 = engine->requestGrabMic(TALK_CHAT_ROOM, 5, true, "请求连麦");
        EXPECT_EQ("发起抢麦请求, 抢麦成功后是自动开启麦克风权限", "startGrabMicAction", code1, YOUME_SUCCESS);

        YouMeErrorCode code2 = engine->releaseGrabMic(TALK_CHAT_ROOM);
        EXPECT_EQ("释放麦", "releaseGrabMic", code2, YOUME_SUCCESS);

        YouMeErrorCode code3 = engine->requestGrabMic(TALK_CHAT_ROOM, 3, false, "请求连麦");
        EXPECT_EQ("发起抢麦请求, 抢麦成功后是不自动开启麦克风权限", "startGrabMicAction", code3, YOUME_SUCCESS);
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);

        YouMeErrorCode code3 = engine->setInviteMicOption("", 50, 20);

        EXPECT_EQ("主播连麦设置,参数为空", "setInviteMicOption", code3, YOUME_SUCCESS);

        YouMeErrorCode code1 = engine->setInviteMicOption(TALK_CHAT_ROOM, 100, 100);
        EXPECT_EQ("主播连麦设置", "setInviteMicOption", code1, YOUME_SUCCESS);

        m_cv.wait_for(lk, std::chrono::seconds(10));

        lk.unlock();


        YouMeErrorCode code2 = engine->setInviteMicOption(TALK_CHAT_ROOM, -1, -1);
        EXPECT_EQ("主播连麦设置，waitTimeout=-1， maxTalkTime=-1", "setInviteMicOption", code2, YOUME_SUCCESS);
    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);

        YouMeErrorCode code1 = engine->requestInviteMic("", "", "");
        EXPECT_EQ("主播与用户发起连麦请求，房间等参数为空值", "requestInviteMic", code1,
                  YOUME_ERROR_INVALID_PARAM);

        YouMeErrorCode code2 = engine->requestInviteMic(TALK_CHAT_ROOM,
                                                        TALK_USER_A,
                                                        "content");
        EXPECT_EQ("主播与用户发起连麦请求，房间等参数不为空值", "requestInviteMic", code2,
                  YOUME_SUCCESS);

        m_cv.wait_for(lk, std::chrono::seconds(10));

    }

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code1 = engine->responseInviteMic(TALK_USER_A, true, "responese");
        EXPECT_EQ("主播应答连麦请求，参数为true，同意连麦", "responseInviteMic", code1, YOUME_SUCCESS);

        std::this_thread::sleep_for(std::chrono::seconds(5));//实际测试可以加长一点

        YouMeErrorCode code2 = engine->stopInviteMic();
        EXPECT_EQ("停止连麦请求", "stopInviteMic", code2, YOUME_SUCCESS);

        m_cv.wait_for(lk, std::chrono::seconds(10));
    }
    engine->unInit();
}

void TalkCasesController::sendemail()
{
    IYouMeVoiceEngine *engine = IYouMeVoiceEngine::getInstance();
    std::string strVer = YoumeUtil::verNumToString(engine->getSDKVersion());
    std::string extentsion = cocos2d::StringUtils::format("Talk_%s", strVer.c_str());
//    YouMeHttpRequest::sendFile("http://106.75.7.162:8998/sendreport",
//                               YoumeUtil::getReportFilename(),
//                               extentsion.c_str());

    /*Jenkins*/
    YouMeHttpRequest::sendFile("http://106.75.7.162:8890/sendreport",
                               YoumeUtil::getReportFilename(),
                               extentsion.c_str());
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
	bool status1 = engine->isInChannel(TALK_CHAT_ROOM);
	EXPECT_EQ("单频道模式查询是否在房间中，是", "isInChannel", status1, true);
	YouMeErrorCode code1 = engine->getChannelUserList(TALK_CHAT_ROOM,-1,false);
	EXPECT_EQ("单频道模式查询用户列表", "getChannelUserList", code1, YOUME_SUCCESS);
}

void TalkCasesController::actionJoinMultiRoom()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->joinChannelMultiMode(m_robotuser.c_str(), TALK_CHAT_ROOM
                                                       , YOUME_USER_TALKER_FREE);
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

void TalkCasesController::cases_clear()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();

    endfile();
    //发邮件将本地的 报告递出去
    engine->unInit();
    cocos2d::log("Finished!");
    //YoumeUtil::clear_sdk_log_file();
}

void TalkCasesController::testThreadFunc()
{
    cases_init();
    cases_room_manager();
    cases_voice_controll();
	cases_others();
    cases_multi_mode();
    cases_micgrab();
    cases_clear();
    sendemail();
}

