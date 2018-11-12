//
//  talkservice.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#include "talkservice.hpp"
#include "imservice.hpp"

TalkService* TalkService::getInstance()
{
    static TalkService inst;
    return &inst;
}

IYouMeVoiceEngine* TalkService::getTalkEngine()
{
    IYouMeVoiceEngine *engine = IYouMeVoiceEngine::getInstance();
    CC_ASSERT(engine != nullptr);
    engine->init(this, "YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0", "r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=",
                                           RTC_CN_SERVER, "");
    return engine;
}

TalkService::TalkService()
{
   // IYouMeVoiceEngine *voice_engine =  IYouMeVoiceEngine::getInstance();
    //voice_engine->init(this, "", "", "", "");
    YouMeErrorCode talkerrcode =
    IYouMeVoiceEngine::getInstance()->init(this, "YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0", "r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=",
                                           RTC_CN_SERVER, "");
    CHECK_RETURN_CODE(talkerrcode, "Initialize talk engine success", "Initialize talk engine failed");
}

TalkService::~TalkService()
{
    //IYouMeVoiceEngine::getInstance()->unInit();
    //delete this;
}

void TalkService::init()
{
    YouMeErrorCode errcode = IYouMeVoiceEngine::getInstance()->init(this, TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");
    CHECK_RETURN_CODE(errcode, "Initialize success", "Initialize failed");
}

void TalkService::onPcmData(int channelNum,
                          int samplingRateHz,
                          int bytesPerSample,
                          void* data,
                          int dataSizeInByte)
{

}

void TalkService::onRequestRestAPI(int requestID, const YouMeErrorCode &iErrorCode, const char *strQuery, const char *strResult)
{

}

//void TalkService::onMemberChange(const char *channel
//                                 , std::list<MemberChange> &listMemberChange)
//{
//
//}

void TalkService::onMemberChange(const char *channel, const char *listMemberChange, bool bUpdate)

{

}


//void TalkService::onBroadcast(const YouMeBroadcast bc, const char *channel, const char *param1, const char *param2, const char *strContent)
//{
//    
//}

//talk callback
//这里是talk的回调函数
void TalkService::onEvent(const YouMeEvent event,
                        const YouMeErrorCode error,
                        const char *channel,
                        const char *param)
{
    cocos2d::log("Callback onevent !");
    cocos2d::log("channel is %s", channel);
    switch (event) {
        case YOUME_EVENT_INIT_OK:
        {
            cocos2d::log("Talk init OK");
        }
            break;
        case YOUME_EVENT_JOIN_OK:
            cocos2d::log("[Youme Test] Join channel Ok!");
            break;
        case YOUME_EVENT_JOIN_FAILED:
            cocos2d::log("[Youme Test] Join channel Failed !");
            break;
        case YOUME_EVENT_RESUMED:
            break;
        case YOUME_EVENT_LEAVED_ALL:
            break;
        case YOUME_EVENT_BGM_FAILED:
            break;
        case YOUME_EVENT_MIC_CTR_ON:
            break;
        case YOUME_EVENT_LEAVED_ONE:
            break;
        case YOUME_EVENT_PAUSED:
            break;
        case YOUME_EVENT_REC_FAILED:
            break;
        case YOUME_EVENT_BGM_STOPPED:
            break;
        case YOUME_EVENT_INIT_FAILED:
            break;

        case YOUME_EVENT_MIC_CTR_OFF:
            break;
        case YOUME_EVENT_RECONNECTED:
            break;
        case YOUME_EVENT_MY_MIC_LEVEL:
            break;
        case YOUME_EVENT_OTHERS_MIC_ON:
            break;
        case YOUME_EVENT_RECONNECTING:
            break;
        case YOUME_EVENT_SPEAK_SUCCESS:
            cocos2d::log("[Youme Test]Callback: switch channel to speak success!");
            break;
        case YOUME_EVENT_SPEAK_FAILED:
            cocos2d::log("[Youme Test]Callback: switch channel to speak failed!");
            break;

        case YOUME_EVENT_OTHERS_MIC_OFF:
            break;
        case YOUME_EVENT_SPEAKER_CTR_ON:
            break;
        case YOUME_EVENT_LISTEN_OTHER_ON:
            break;
        case YOUME_EVENT_OTHERS_VOICE_ON:
            break;
        case YOUME_EVENT_SPEAKER_CTR_OFF:
            break;
        case YOUME_EVENT_LISTEN_OTHER_OFF:
            break;
        case YOUME_EVENT_OTHERS_VOICE_OFF:
            break;
        case YOUME_EVENT_OTHERS_SPEAKER_ON:
            break;
        case YOUME_EVENT_OTHERS_SPEAKER_OFF:
            break;
        default:
            break;
    }
}
