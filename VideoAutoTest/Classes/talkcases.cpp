//
//  talkcases.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/8/2.
//
//
#include "talkcases.hpp"
#include "YouMeHttpRequest.hpp"
#include "YMRenderTexture.h"
#include "YouMeTalk.h"

void TalkCasesController::init()
{
    SetServerMode(SERVER_MODE_TEST);
    
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    //engine->setTCPMode(1);
    engine->init(this, TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");
}

TalkCasesController::TalkCasesController()
{
    m_robotuser = YoumeUtil::getRandUserName();
    //::initfile();
    YoumeUtil::clear_sdk_log_file();


//    const char * appkey = "YOUME5BE427937AF216E88E0F84C0EF148BD29B691556";
//    const char * appsecret = "y1sepDnrmgatu/G8rx1nIKglCclvuA5tAvC0vXwlfZKOvPZfaUYOTkfAdUUtbziW8Z4HrsgpJtmV/RqhacllbXD3abvuXIBlrknqP+Bith9OHazsC1X96b3Inii6J7Und0/KaGf3xEzWx/t1E1SbdrbmBJ01D1mwn50O";
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

void TalkCasesController::OnCustomDataNotify(const void *pData, int iDataLen, unsigned long long ulTimeSpan)
{
    log("recv data : %s", (char *)pData);
    log("time stamp : %lld", ulTimeSpan);
    log("data len: %d", iDataLen);
}

/**
void TalkCasesController::frameRender(int renderId, int nWidth, int nHeight, int nRotationDegree, int nBufSize, const void *buf)
{
    cocos2d::log("call frame rander");
}
 **/

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
        case YOUME_EVENT_JOIN_OK:{
            EXPECT_EQ("加入房间成功", "YOUME_EVENT_JOIN_OK", error, YOUME_SUCCESS);
            YouMeErrorCode code = IYouMeVoiceEngine::getInstance()->startCapture();
            if (code != YOUME_SUCCESS) {
                cocos2d::log("采集失败");
            }
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
        case YOUME_EVENT_SPEAK_FAILED:{
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
            EXPECT_EQ("收到文本消息通知", "回调: YOUME_EVENT_MESSAGE_NOTIFY", error,
                      YOUME_SUCCESS);
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
        case YOUME_EVENT_OTHERS_VIDEO_INPUT_START:
        {
            cocos2d::log("其他用户视频输入开始");
            EXPECT_EQ("其他用户视频输入开始", "回调：YOUME_EVENT_OTHERS_VIDEO_INPUT_START", error, YOUME_SUCCESS);
        }
            break;

        case YOUME_EVENT_OTHERS_VIDEO_INPUT_STOP:
        {
            cocos2d::log("其他用户输入停止");
            EXPECT_EQ("其他用户视频输入开始", "回调：YOUME_EVENT_OTHERS_VIDEO_INPUT_STOP", error, YOUME_SUCCESS);
            break;
        }

        case YOUME_EVENT_MEDIA_DATA_ROAD_PASS:
        {
            cocos2d::log("音视频数据通路连通");
            EXPECT_EQ("音视频数据通路连通", "回调：YOUME_EVENT_MEDIA_DATA_ROAD_PASS", error, YOUME_SUCCESS);
            break;
        }

        case YOUME_EVENT_MEDIA_DATA_ROAD_BLOCK:
        {
            cocos2d::log("音视频数据通路不通");
            EXPECT_EQ("音视频数据通路不通", "回调：YOUME_EVENT_MEDIA_DATA_ROAD_BLOCK", error, YOUME_SUCCESS);
            break;
        }
        case YOUME_EVENT_MASK_VIDEO_FOR_USER:
        {
            //屏蔽了某某的视屏
            cocos2d::log("屏蔽了某某的视屏");
            EXPECT_EQ("屏蔽了某用户的视频", "回调：YOUME_EVENT_RESUME_VIDEO_FOR_USER", error, YOUME_SUCCESS);
            break;
        }

        case YOUME_EVENT_RESUME_VIDEO_FOR_USER:
        {
            //恢复了某用户的视频
            cocos2d::log("恢复了某用户的视频");
            EXPECT_EQ("恢复了某用户的视频", "回调：YOUME_EVENT_RESUME_VIDEO_FOR_USER",
                      error, YOUME_SUCCESS);
            break;
        }

        case YOUME_EVENT_OTHERS_VIDEO_SHUT_DOWN:
        {
            cocos2d::log("视频断开");
            EXPECT_EQ("视频断开回调", "回调：YOUME_EVENT_OTHERS_VIDEO_SHUT_DOWN",
                      error, YOUME_SUCCESS);
            break;
        }

        case YOUME_EVENT_OTHERS_VIDEO_ON:
        {
            //其他用户的视频流打开
            cocos2d::log("其他用户的视频流打开");
            EXPECT_EQ("其他用户的视频流打开", "回调：YOUME_EVENT_OTHERS_VIDEO_ON",0, 0);

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
    _run_thread = std::thread(&TalkCasesController::testThreadFunc, this);
    _run_thread.detach();
}

void TalkCasesController::testTalkMode()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();

    //自由通话模式
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinSingleRoom();
        m_cv.wait_for(lk, std::chrono::seconds(10));

        //设置用户角色
        YouMeErrorCode code = engine->setUserRole(YOUME_USER_TALKER_FREE);

        EXPECT_EQ("设置用户说话时的角色", "setUserRole", code, YOUME_SUCCESS);
        EXPECT_EQ("获取用户说话时的角色", "getUserRole", engine->getUserRole(), YOUME_USER_TALKER_FREE) ;
    }

    // 扬声器控制
    {
        //关闭扬声器
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        std::unique_lock<std::mutex> lk(m_mutex);
        engine->setSpeakerMute(true);

        std::chrono::seconds(5);
        m_cv.wait_for(lk, std::chrono::seconds(5));
        bool status1 = engine->getSpeakerMute(); //获取扬声器状态
        EXPECT_EQ("获取静音状态,关闭扬声器", "getSpeakerMute", status1, true);
        //lk.unlock();
    }

    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        std::unique_lock<std::mutex> lk2(m_mutex);
        engine->setSpeakerMute(false); //打开扬声器
        m_cv.wait_for(lk2, std::chrono::seconds(10));
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
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("nekomimi.mp3").c_str(), true);
        if (code == YOUME_SUCCESS) {
            for (int i = 0; i < 5; ++i) {
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

       // YouMeErrorCode code1 = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("mangetunoyoruniodore.mp3").c_str(), true);
        //EXPECT_EQ("播放背景音乐", "playBackgroundMusic", code1, YOUME_SUCCESS);

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
//    {
//        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
//        std::unique_lock<std::mutex> lk(m_mutex);
//
//        YouMeErrorCode code = engine->playBackgroundMusic(cocos2d::FileUtils::getInstance()->fullPathForFilename("mangetunoyoruniodore.mp3").c_str(),
//                                                          true);
//        engine->setVolume(10);
//        EXPECT_EQ("背景音乐播放", "playBackgroundMusic", code, YOUME_SUCCESS);
//        cocos2d::log("playBackgroundMusic Wait for callback...");
//        m_cv.wait_for(lk, std::chrono::seconds(10));
//
//        std::this_thread::sleep_for(std::chrono::seconds(15));
//        YouMeErrorCode stopcode = engine->stopBackgroundMusic();
//        EXPECT_EQ("停止背景音乐播放", "stopBackgroundMusic", stopcode, YOUME_SUCCESS);
//    }

    //监听设置
//    {
//        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
//        YouMeErrorCode code = engine->setHeadsetMonitorOn(true);
//        EXPECT_EQ("监听控制:打开监听", "setHeadsetMonitorOn", code, YOUME_SUCCESS);
//
//        std::this_thread::sleep_for(std::chrono::seconds(15));
//        YouMeErrorCode code1 = engine->setHeadsetMonitorOn(false);
//        EXPECT_EQ("监听控制：关闭监听", "setHeadsetMonitorOn", code1, YOUME_SUCCESS);
//    }

    //控制别人的麦克风
    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->setOtherMicMute("1234", true);
        EXPECT_EQ("控制他人麦克风，让用户静音", "setHeadsetMonitorOn", code, YOUME_SUCCESS);
        std::unique_lock<std::mutex> lk(m_mutex);
        cocos2d::log("setOtherMicMute Wait for callback...");
        m_cv.wait_for(lk, std::chrono::seconds(5));
    }

    {
        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code1 = engine->setOtherMicMute("1234", false);
        EXPECT_EQ("控制他人麦克风，取消用户静音", "setHeadsetMonitorOn", code1, YOUME_SUCCESS);
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

    {
        //判断用户是否在频道内

        bool b = engine->isInChannel(TALK_CHAT_ROOM);

        log("return code is %d", b);
        EXPECT_EQ("判断用户是否在一个频道内", "isInChannel", b, true);
        bool b2 = engine->isInChannel("1234567890");
        EXPECT_EQ("判断用户是否在一个频道内", "isInChannel", b2, false);

        std::unique_lock<std::mutex> lk(m_mutex);
        YouMeErrorCode code1 = engine->joinChannelSingleMode(m_robotuser.c_str(), "1234567890", YOUME_USER_TALKER_FREE);
        if (code1 == YOUME_SUCCESS) {
            m_cv.wait_for(lk, std::chrono::seconds(10));
        }


        bool b3 = engine->isInChannel("1234567890");
        EXPECT_EQ("判断用户是否在一个频道", "isInChannel", b3, true);

        engine->leaveChannelMultiMode("1234567890");

    }
    //查频道用户列表
    /**
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

        YouMeErrorCode code2 = engine->getChannelUserList(TALK_CHAT_ROOM, 10, false);

        EXPECT_EQ("查询频道用户列表:10个用户, 其他用户进出房间时不通知", "GetChannelUserList", code2, YOUME_SUCCESS);

    }
     */

    //踢人
    {
        //        std::unique_lock<std::mutex> lk(m_mutex);
        //        YouMeErrorCode code = engine->kickOtherFromChannel(USER_ID_B, TALK_CHAT_ROOM, 10);
        //        if (code == YOUME_SUCCESS) {
        //            EXPECT_EQ("[KickOtherFromChannel call]", code,      YOUME_SUCCESS); //机器人造出来后再恢复
        //            m_cv.wait_for(lk, std::chrono::seconds(10));
        //        }
    }
}

void TalkCasesController::testHostMode()
{
//主播通话模式 主播

    IYouMeVoiceEngine *engine = YouMeTalk::getInstance()->getInf();


    engine->setNotifyCallback(this);
    engine->setVideoCallback(this);
    engine->setPcmCallback(this);
    engine->setRestApiCallback(this);
    engine->setAVStatisticCallback(this);
    engine->setRecvCustomDataCallback(this);
    


    {
        engine->unInit();
        std::unique_lock<std::mutex> lk(m_mutex);
        actionInit();
        m_cv.wait_for(lk, std::chrono::seconds(10));
    }

    engine->setTCPMode(1);

    {
        std::unique_lock<std::mutex> lk(m_mutex);
        actionJoinRoomZhubo();

        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
        YouMeErrorCode code = engine->joinChannelSingleMode(m_robotuser.c_str(), "1234", YOUME_USER_TALKER_FREE);
        EXPECT_EQ("以主播模式加入单个房间", "joinChannelSingleMode", code, YOUME_SUCCESS);
        m_cv.wait_for(lk, std::chrono::seconds(10));

        YouMeErrorCode code1 = engine->startCapture();
        EXPECT_EQ("开启摄像头", "startCapture", code1, YOUME_SUCCESS);
        
    }
    //视频测试
    //主播模式，主播端
    {

        IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();

        YouMeErrorCode code3 = engine->inputCustomData(nullptr, 1000, 1529063275);
        EXPECT_EQ("向房间输入自定义数据,传入nullptr值", "inputCustomData", code3, YOUME_ERROR_INVALID_PARAM);
        YouMeErrorCode code1 = engine->inputCustomData(nullptr, 1025, 1529063275);
        EXPECT_EQ("向房间输入自定义数据, 自定义数据超出1024个字节", "inputCustomData", code1, YOUME_ERROR_INVALID_PARAM);


        char * data = (char*) malloc(1025);
        memset(data, 'a', 1025);

        YouMeErrorCode code2 = engine->inputCustomData(data, 1024, 1529063278L);
        EXPECT_EQ("输入自定义数据, 输入正确的参数", "inputCustomData", code2,
                  YOUME_SUCCESS);

        free(data);
    }
    /*抢麦放麦*/

    return;

    {
        //抢麦相关设置

        //std::unique_lock<std::mutex> lk(m_mutex);
        //暂时看不出有回调
        YouMeErrorCode code = engine->setGrabMicOption(TALK_CHAT_ROOM, 1, 10, 30, 3); //
        // 1先到先得
        //
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
        //engine->setVideoNoFrameTimeout(1);
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
}

void TalkCasesController::testFreeVideoMode()
{
    IYouMeVoiceEngine *engine = YouMeTalk::getInstance()->getInf();

	int renderId = engine->createRender(m_robotuser.c_str());
	if (renderId >= 0){
		EXPECT_EQ("创建渲染", "createRender", 0, 0);
	}

	int r1 = engine->deleteRender(renderId);
	EXPECT_EQ("删除渲染", "deleteRender", r1, 0);

	engine->createRender(m_robotuser.c_str());//重新创建渲染

	YouMeErrorCode code1 = engine->openBeautify(true);
	EXPECT_EQ("美颜开关，默认是关闭美颜", "openBeautify", code1, YOUME_SUCCESS);

	YouMeErrorCode code2 = engine->beautifyChanged(0.5);
	EXPECT_EQ("美颜强度参数设置", "beautifyChanged", code2, YOUME_SUCCESS);

	YouMeErrorCode code3 = engine->stretchFace(true);
	EXPECT_EQ("瘦脸开关", "stretchFace", code3, YOUME_SUCCESS);

	YouMeErrorCode code4 = engine->setExternalFilterEnabled(true);
	EXPECT_EQ("打开/关闭 外部扩展滤镜回调", "setExternalFilterEnabled", code4, YOUME_SUCCESS);
	
	YouMeErrorCode code5 = engine->setVideoFrameRawCbEnabled(true);
	EXPECT_EQ("设置视频数据回调方式", "setVideoFrameRawCbEnabled", code5, YOUME_SUCCESS);
}

void TalkCasesController::actionInputData()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    engine->setRecvCustomDataCallback(this);

//    YouMeErrorCode code4 = engine->joinChannelSingleMode(m_robotuser.c_str(), "1234", YOUME_USER_TALKER_FREE);
//
//    std::this_thread::sleep_for(std::chrono::seconds(5));

//    YouMeErrorCode code3 = engine->inputCustomData(nullptr, 1000, 1529063275);
//    EXPECT_EQ("向房间输入自定义数据,传入nulltr值", "inputCustomData", code3, YOUME_ERROR_INVALID_PARAM);
//    YouMeErrorCode code1 = engine->inputCustomData(nullptr, 1025, 1529063275);
//    EXPECT_EQ("向房间输入自定义数据, 自定义数据超出1024个字节", "inputCustomData", code1, YOUME_ERROR_INVALID_PARAM);


    char * data = (char*) malloc(1025);
    memset(data, 'a', 1025);

    for (int i = 0; i < 5; ++i) {
        YouMeErrorCode code = engine->inputCustomData(data, 20, 1529063278L);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    //EXPECT_EQ("输入自定义数据, 输入正确的参数", "inputCustomData", code2,
    //          YOUME_SUCCESS);
    free(data);
}

void TalkCasesController::runTests()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();

    engine->setPcmCallback(this);
    engine->setVideoCallback(this);
    engine->setNotifyCallback(this);
    engine->setRestApiCallback(this);
    engine->setAVStatisticCallback(this);
    engine->setCaptureFrontCameraEnable(true);
    engine->setRecvCustomDataCallback(this);

    initfile();

    actionUninit();

//    {
//        std::unique_lock<std::mutex> lk(m_mutex);
//        actionInit();
//        //cocos2d::log("Wait for callback...");
//        m_cv.wait_for(lk, std::chrono::seconds(10));
//        EXPECT_EQ("是否已经初始化成功", "isInited", engine->isInited(), true);
//    }

    {
        engine->unInit();

		actionInit();
		{
			//进入房间前参数设置
			YouMeErrorCode code1 = engine->setVBR(true);
			EXPECT_EQ("设置视频编码是否采用VBR动态码率方式", "setVBR", code1, YOUME_SUCCESS);

			YouMeErrorCode code2 = engine->setVBRForSecond(true);
			EXPECT_EQ("设置小流视频编码是否采用VBR动态码率方式", "setVBRForSecond", code2, YOUME_SUCCESS);

			engine->setVideoHardwareCodeEnable(false);
			bool code3 = engine->getVideoHardwareCodeEnable();
			if (code3 = false){
				EXPECT_EQ("设置视频数据是否同意开启硬编硬解", "setVideoHardwareCodeEnable", 0, 0);
				EXPECT_EQ("获取视频数据是否同意开启硬编硬解", "getVideoHardwareCodeEnable", 0, 0);
			}
			engine->setVideoHardwareCodeEnable(true);//恢复默认值

			bool code4 = engine->getUseGL();
			if (code4 = true){
				EXPECT_EQ("获取android下是否启用GL", "getUseGL", 0, 0);
			}
		}
        actionJoinRoomZhubo();

        engine->setExternalInputMode(true);
        //engine->setExternalInputMode(false); //SDK内部采集模式
        //EXPECT_EQ("设置视频输入模式：SDK内部采集模式", "setExternalInputMode", code1, YOUME_SUCCESSS);

        YouMeErrorCode code1 = engine->setVideoFps(15.0);//帧率
		EXPECT_EQ("设置帧率", "setVideoFps", code1, YOUME_SUCCESS);

        engine->setSpeakerMute(false);

		YouMeErrorCode code2 = engine->setVideoNetResolutionForSecond(720, 160);
		EXPECT_EQ("设置视频网络传输过程的分辨率，低分辨率", "setVideoNetResolutionForSecond", code2, YOUME_SUCCESS);

		YouMeErrorCode code3 = engine->setExternalInputSampleRate(SAMPLE_RATE_16, SAMPLE_RATE_8);
		EXPECT_EQ("设置外部输入模式的语音采样率", "setExternalInputSampleRate", code3, YOUME_SUCCESS);

        engine->setAudioQuality(HIGH_QUALITY);

		YouMeErrorCode code4 = engine->startCapture();
		EXPECT_EQ("开始camera capture", "startCapture", code4, YOUME_SUCCESS);
    }

	{	YouMeErrorCode code1 = engine->resetCamera();
		EXPECT_EQ("权限检测结束后重置摄像头", "resetCamera", code1, YOUME_SUCCESS);
	}
    
    {
        //摄像头控制
        YouMeErrorCode code1 = engine->switchCamera();
        EXPECT_EQ("切换摄像头", "switchCamera", code1, YOUME_SUCCESS);

        for (int i = 0; i < 10; ++i) {
            engine->switchCamera();
        }
    }

    {
        //是否设置前置摄像头
        YouMeErrorCode code = engine->setCaptureFrontCameraEnable(true);
        EXPECT_EQ("设置前置摄像头true", "setCaptureFrontCameraEnable", code, YOUME_SUCCESS);
    }

    {
        engine->setVideoCodeBitrate(100, 0);
        engine->setAudioQuality(LOW_QUALITY);
        engine->setAudioQuality(HIGH_QUALITY);
    }

    {
        unsigned int sxmalv = engine->getCurrentVideoCodeBitrate();
        cocos2d::log("The video code bit rate is %d", sxmalv);
    }

	testFreeVideoMode();
    testTalkMode();
    testHostMode();
    

    endfile();
    cocos2d::log("Finished!");
}

void TalkCasesController::onVideoFrameCallback(std::string userId, void *data, int len, int width, int height, int fmt, uint64_t timestamp)
{
    cocos2d::log("12221");
    cocos2d::log("%s, %s, %d, %d, %d, %d, %lld", userId.c_str(), data, len, width, height, fmt, timestamp);
    
}

void TalkCasesController::onAudioFrameMixedCallback(void* data, int len, uint64_t timestamp)
{
    cocos2d::log("%s %d %lld", data, len, timestamp);
    //cocos2d::log();
}

void TalkCasesController::sendemail()
{
    IYouMeVoiceEngine *engine = IYouMeVoiceEngine::getInstance();
    std::string strVer = YoumeUtil::verNumToString(engine->getSDKVersion());
    std::string extentsion = cocos2d::StringUtils::format("Vedio_SDK_%s", strVer.c_str());
    YouMeHttpRequest::sendFile("http://106.75.7.162:8890/sendreport",
                               YoumeUtil::getReportFilename(),
                               extentsion.c_str());
//    YouMeHttpRequest::sendFile("http://106.75.7.162:8998/sendreport",
//                               YoumeUtil::getReportFilename(),
//                               extentsion.c_str());
}

void TalkCasesController::actionInit()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->init(this, TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");

    EXPECT_EQ("初始化Talk引擎", "init", code, YOUME_SUCCESS);

    std::this_thread::sleep_for(std::chrono::seconds(5));
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
    EXPECT_EQ("离开房间", "leaveChannelAll", code, YOUME_SUCCESS);
}

void TalkCasesController::actionJoinRoomZhubo()
{
    IYouMeVoiceEngine* engine = IYouMeVoiceEngine::getInstance();
    YouMeErrorCode code = engine->joinChannelSingleMode(m_robotuser.c_str(), TALK_CHAT_ROOM, YOUME_USER_HOST);
    EXPECT_EQ("以主播模式加入单个房间", "joinChannelSingleMode", code, YOUME_SUCCESS);
}

void TalkCasesController::testThreadFunc()
{
    runTests();
    sendemail();
}

