//
//  AutoRandTest.cpp
//  AutoCaller
//
//  Created by pinky on 2017/7/5.
//
//

#include "AutoRandTest.hpp"
#include "unistd.h"
#include <IYouMeVoiceEngine.h>
#include <YIM.h>
#include <stdlib.h>
#include "YouMeVoiceEngineImp.hpp"
#include <YouMeCommon/XFile.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <YouMeCommon/CrossPlatformDefine/IOS/YouMeApplication_iOS.h>
#endif
std::string getCachePath(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "/sdcard/";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    YouMeApplication_iOS  ios;
    
    return ios.getCachePath();
#endif
}

class RandParam{
public:
    static  bool GetBool(){
        int value = rand() % 2;
        return value == 1 ? true : false;
    }
    
    //min<= x <= max, 2位精度
    static float GetFloat( float min, float max ){
        if( max <= min ){
            return min;
        }
        
        int value = rand() % (int(max - min ) * 100  + 1 );
        
        return min + value /100.0;
    }
    
    //min<= x <= max
    static int GetInt( int min, int max )
    {
        if( max <= min ){
            return min;
        }
        
        int value = rand() % (max - min + 1 );
        return min +  value;
        
    }
};

void randSleep( int min , int max ){
    int wait = RandParam::GetInt( min , max );
    
    usleep( wait * 1000 );
}

void FuncRandTest::StartTest( int count ){
    bool bNeedWait = prepare();
    for( int i = 0 ; i < count ; i++ ){
        std::thread tmp = std::thread( &FuncRandTest::RandTestThreadFunc, this, m_vecFuncs );
        tmp.detach();
    }
    
    if( !bNeedWait ){
        setReady();
    }
}

void FuncRandTest::addFunc( const std::function<void(void)>& func ){
    m_vecFuncs.push_back( func );
}

void FuncRandTest::RandTestThreadFunc( const std::vector< std::function<void(void)>>&  vecFuncs ){
    std::unique_lock <std::mutex> lck(mtx);
    m_cv.wait(lck);
    
    int nTotalCount = vecFuncs.size();
    
    do{
        int index = rand() % nTotalCount;
        
        auto func = vecFuncs[index];
        
        func();
        
    }while(true);
}

void FuncRandTest::setReady(){
    m_cv.notify_all();
}

bool FuncRandTest::prepare(){
    //默认不需要等待准备工作
    return false;
}


IMAutoRandTest* g_imtest = nullptr;
TalkAutoRandTest* g_talktest = nullptr;

IMAutoRandTest::IMAutoRandTest()
{
    m_strRecvPath = youmecommon::CXFile::CombinePath(getCachePath(), "voice.wav" );
    m_strAmrRecvPath = youmecommon::CXFile::CombinePath(getCachePath(), "voice.amr" );
    m_url = "";
    m_recvMessageID = 0;
}

bool IMAutoRandTest::prepare(){
    YouMe_Log("Login")
    YIMManager::CreateInstance()->Login( m_role.c_str(), "123456", "");

    return false;
}

void IMAutoRandTest::registAllTest(){
//    addFunc([=](){
//        YouMe_Log("Login")
//        YIMManager::CreateInstance()->Login( m_role.c_str(), "123456", "");
//    });
//    
//    addFunc([](){
//        YouMe_Log("Logout")
//        YIMManager::CreateInstance()->Logout();
//    });
    
    addFunc([=](){
        YouMe_Log("JoinChatRoom")
        YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom( m_room.c_str() );
    });
    
    addFunc([=](){
        YouMe_Log("LeaveChatRoom")
        YIMManager::CreateInstance()->GetChatRoomManager()->LeaveChatRoom( m_room.c_str() );
    });
    
    registLocationTest();
    registMessageTest();
    registYIMTest();

}

void IMAutoRandTest::registLocationTest(){
    addFunc([](){
        YouMe_Log("GetCurrentLocation")
        YIMManager::CreateInstance()->GetLocationManager()->GetCurrentLocation();
    });
    
    addFunc([](){
        YouMe_Log("GetNearbyObjects")
        YIMManager::CreateInstance()->GetLocationManager()->GetNearbyObjects( 20 , "", DISTRICT_UNKNOW, RandParam::GetBool()  );
    });
    
    addFunc([](){
        YouMe_Log("SetUpdateInterval")
        YIMManager::CreateInstance()->GetLocationManager()->SetUpdateInterval( 30 );
    });
}

void IMAutoRandTest::registMessageTest(){
    addFunc([=](){
        XUINT64 requestID = 0;
        YouMe_Log("SendTextMessage")
        YIMManager::CreateInstance()->GetMessageManager()->SendTextMessage( m_roleto.c_str(), ChatType_PrivateChat, "my message!",  &requestID );
    });

    addFunc([=](){
        YouMe_Log("MultiSendTextMessage")
        std::vector<XString> vecReceiver;
        vecReceiver.push_back( m_roleto);
        vecReceiver.push_back( m_role );
        YIMManager::CreateInstance()->GetMessageManager()->MultiSendTextMessage( vecReceiver, "my multi message" );
    });

    addFunc([=](){
        YouMe_Log("SendCustomMessage")
        XUINT64 requestID = 0;
        char a[10] = {0};
        a[1] = 'a';
        a[2] = 'b';
        YIMManager::CreateInstance()->GetMessageManager()->SendCustomMessage( m_roleto.c_str(), ChatType_PrivateChat, a, 2 , &requestID );
    });

    addFunc([=](){
        YouMe_Log("SendFile,No File")
        XUINT64 requestID = 0;
        std::lock_guard<mutex> lock(m_paramMutex);
        YIMManager::CreateInstance()->GetMessageManager()->SendFile( m_roleto.c_str(), ChatType_PrivateChat, m_strRecvPath.c_str() , &requestID,  "extra" );
    });

    addFunc([=](){
        YouMe_Log("SendAudioMessage")
        XUINT64 requestID = 0;

        YIMManager::CreateInstance()->GetMessageManager()->SendAudioMessage( m_roleto.c_str(), ChatType_PrivateChat, &requestID );
        
        randSleep( 1000 , 5000 );
    });

    addFunc([=](){
        YouMe_Log("SendOnlyAudioMessage")
        XUINT64 requestID = 0;
        
        YIMManager::CreateInstance()->GetMessageManager()->SendOnlyAudioMessage( m_roleto.c_str(), ChatType_PrivateChat, &requestID );
        
        randSleep( 1000 , 5000 );

    });
    
    addFunc([](){
        YouMe_Log("StopAudioMessage")
        YIMManager::CreateInstance()->GetMessageManager()->StopAudioMessage( "extra" );
    });

    addFunc([](){
        YouMe_Log("CancleAudioMessage")
        YIMManager::CreateInstance()->GetMessageManager()->CancleAudioMessage( );
    });


    addFunc([=](){
        YouMe_Log("SendGift")
        XUINT64 requestID = 0;

        YIMManager::CreateInstance()->GetMessageManager()->SendGift( m_roleto.c_str(), m_room.c_str()
                                                                   ,1,  5,
                                                                    "{\"nickname\":\"\",\"server_area\":\"\",\"location\":\"\",\"score\":\"\",\"level\":\"\",\"vip_level\":\"\",\"extra\":\"\"}",
                                                                    &requestID);
    });

    addFunc([=](){
        YouMe_Log("DownloadFile")
        
        std::lock_guard<mutex> lock(m_paramMutex);
        YIMManager::CreateInstance()->GetMessageManager()->DownloadFile( this->m_recvMessageID, m_strRecvPath.c_str() );
    });
    
    addFunc([=](){
        YouMe_Log("DownloadFileUrl")

        std::lock_guard<mutex> lock(m_paramMutex);
        YIMManager::CreateInstance()->GetMessageManager()->DownloadFile( m_url.c_str(), m_strAmrRecvPath.c_str() );
    });

    addFunc([=](){
        YouMe_Log("QueryHistoryMessage")
        
        YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage( m_room.c_str(), ChatType_RoomChat, 0 , 30 , 0 );
    });
    
    addFunc([](){
        YouMe_Log("DeleteHistoryMessage")
        
        YIMManager::CreateInstance()->GetMessageManager()->DeleteHistoryMessage( ChatType_RoomChat,  time(0) );
    });
    addFunc([](){
        YouMe_Log("DeleteHistoryMessageByID")
        
        YIMManager::CreateInstance()->GetMessageManager()->DeleteHistoryMessageByID( 3 );
    });
    addFunc([=](){
        YouMe_Log("QueryRoomHistoryMessageFromServer")
        
        YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer( m_room.c_str() );
    });
    addFunc([](){
        YouMe_Log("StartAudioSpeech")
        XUINT64 requestID = 0;

        YIMManager::CreateInstance()->GetMessageManager()->StartAudioSpeech( &requestID, RandParam::GetBool() );
        
        randSleep( 1000 , 5000 );
    });
    addFunc([](){
        YouMe_Log("StopAudioSpeech")
        
        YIMManager::CreateInstance()->GetMessageManager()->StopAudioSpeech();
    });
    
    
    addFunc([=](){
        YouMe_Log("ConvertAMRToWav")
        std::lock_guard<mutex> lock(m_paramMutex);
        YIMManager::CreateInstance()->GetMessageManager()->ConvertAMRToWav( m_strAmrRecvPath.c_str(), m_strRecvPath.c_str() );
    });
    
    
    addFunc([=](){
        YouMe_Log("SetReceiveMessageSwitch")
        std::vector<XString> vecReceiver;
        vecReceiver.push_back( m_room );
        vecReceiver.push_back(__XT("234") );

        
        YIMManager::CreateInstance()->GetMessageManager()->SetReceiveMessageSwitch(vecReceiver, RandParam::GetBool() );
    });
    
    
    addFunc([=](){
        YouMe_Log("GetNewMessage")
        std::vector<XString> vecReceiver;
        vecReceiver.push_back( m_room );
        vecReceiver.push_back(__XT("w234") );
        
        
        YIMManager::CreateInstance()->GetMessageManager()->GetNewMessage(vecReceiver );
    });
    
    addFunc([=](){
        YouMe_Log("SetRoomHistoryMessageSwitch")
        std::vector<XString> vecReceiver;
        vecReceiver.push_back( m_room );
        vecReceiver.push_back(__XT("w234") );
        
        
        YIMManager::CreateInstance()->GetMessageManager()->SetRoomHistoryMessageSwitch(vecReceiver, RandParam::GetBool() );
    });
    
    addFunc([](){
        YouMe_Log("TranslateText")
        unsigned int requestID = 0;
        
        YIMManager::CreateInstance()->GetMessageManager()->TranslateText( &requestID, "my lover.", LANG_ZH_CN, LANG_AUTO );
    });
    
    
    addFunc([](){
        YouMe_Log("SetSpeechRecognizeParam")
        
        YIMManager::CreateInstance()->GetMessageManager()->SetSpeechRecognizeParam( ACCENT_MANDARIN, RECOGNIZELANG_ZH_CH );
    });
    
    addFunc([=](){
        YouMe_Log("Accusation")
        unsigned int requestID = 0;
        
        YIMManager::CreateInstance()->GetMessageManager()->Accusation( m_roleto.c_str(), ChatType_RoomChat,  2 , "bad boy",
                                                                      "{\"nickname\":\"\",\"server_area\":\"\",\"level\":\"\",\"vip_level\":\"\"}");
    });
    
}

void IMAutoRandTest::registYIMTest(){
    addFunc([](){
        YouMe_Log("GetSDKVersion")
        YIMManager::CreateInstance()->GetSDKVersion();
    });
    
    addFunc([](){
        YouMe_Log("SetServerZone")
        YIMManager::CreateInstance()->SetServerZone( ServerZone_China );
    });
    
    addFunc([](){
        YouMe_Log("SetMode")
        YIMManager::CreateInstance()->SetMode( 0 );
    });
    
    addFunc([](){
        YouMe_Log("SetAudioCacheDir")
        std::string path = getCachePath();
        YIMManager::CreateInstance()->SetAudioCacheDir( path.c_str() );
    });
    

    addFunc([](){
        YouMe_Log("GetRecentContacts")
        YIMManager::CreateInstance()->GetRecentContacts();
    });
    

    
    addFunc([](){
        YouMe_Log("SetUserInfo")
        YIMManager::CreateInstance()->SetUserInfo(
                    "{\"nickname\":\"pinky\",\"server_area_id\":\"asdfad\",\"server_area\":\"adfaasgd\",\"location_id\":\"aa\",\"location\":\"aaa\",\"level\":\"30\",\"vip_level\":\"1\",\"platform_id\":\"\",\"platform\":\"\"}");
    });
    
    addFunc([=](){
        YouMe_Log("GetUserInfo")
        YIMManager::CreateInstance()->GetUserInfo( m_roleto.c_str() );
    });
    
    addFunc([=](){
        YouMe_Log("QueryUserStatus")
        YIMManager::CreateInstance()->QueryUserStatus( m_roleto.c_str());
    });
    
    addFunc([](){
        YouMe_Log("FilterKeyword")
        int level = 0;
        YIMManager::CreateInstance()->FilterKeyword("我日你啊~~~滚你毛泽东", &level );
    });
    
    addFunc([](){
        YouMe_Log("OnPause")
        YIMManager::CreateInstance()->OnPause( );
    });
    
    addFunc([](){
        YouMe_Log("OnResume")
        YIMManager::CreateInstance()->OnResume( );
    });
    
    addFunc([](){
        YouMe_Log("SetVolume")
        YIMManager::CreateInstance()->SetVolume( RandParam::GetFloat( 0.0, 1.0) );
    });
    
    addFunc([=](){
        YouMe_Log("StartPlayAudio")
        YIMManager::CreateInstance()->StartPlayAudio( m_strRecvPath.c_str() );
    });
    
    addFunc([](){
        YouMe_Log("StopPlayAudio")
        YIMManager::CreateInstance()->StopPlayAudio( );
    });
    
    addFunc([](){
        YouMe_Log("IsPlaying")
        YIMManager::CreateInstance()->IsPlaying();
    });
    
    addFunc([](){
        YouMe_Log("GetMicrophoneStatus")
        YIMManager::CreateInstance()->GetMicrophoneStatus();
    });
    
    addFunc([](){
        YouMe_Log("SetKeepRecordModel")
        YIMManager::CreateInstance()->SetKeepRecordModel( RandParam::GetBool() );
    });
    
    addFunc([](){
        YouMe_Log("GetAudioCachePath")
        YIMManager::CreateInstance()->GetAudioCachePath();
    });
    
    addFunc([](){
        YouMe_Log("ClearAudioCachePath")
        YIMManager::CreateInstance()->ClearAudioCachePath();
    });
    
    addFunc([](){
        YouMe_Log("QueryNotice")
        YIMManager::CreateInstance()->QueryNotice();
    });
}


void TalkAutoRandTest::registAllTest()
{
    addFunc([](){
        YouMe_Log("setToken")
        IYouMeVoiceEngine::getInstance()->setToken("123456");
    });
    
    addFunc([](){
        YouMe_Log("setToken")
        IYouMeVoiceEngine::getInstance()->setToken("");
    });

    
    addFunc([](){
        YouMe_Log("setServerRegion")
        IYouMeVoiceEngine::getInstance()->setServerRegion( RTC_CN_SERVER, "asdf", RandParam::GetBool() );
    });
    
    addFunc([](){
        YouMe_Log("setSpeakerMute")
        IYouMeVoiceEngine::getInstance()->setSpeakerMute(RandParam::GetBool());
    });
    
    addFunc([](){
        YouMe_Log("getSpeakerMute")
        IYouMeVoiceEngine::getInstance()->getSpeakerMute();
    });
    
    addFunc([](){
        YouMe_Log("setMicrophoneMute")
        IYouMeVoiceEngine::getInstance()->setMicrophoneMute(RandParam::GetBool());
    });
    
    addFunc([](){
        YouMe_Log("getMicrophoneMute")
        IYouMeVoiceEngine::getInstance()->getMicrophoneMute();
    });
    
    addFunc([](){
        YouMe_Log("setAutoSendStatus")
        IYouMeVoiceEngine::getInstance()->setAutoSendStatus(RandParam::GetBool());
    });
    
    addFunc([](){
        YouMe_Log("getVolume")
        IYouMeVoiceEngine::getInstance()->getVolume();
    });
    
    addFunc([](){
        YouMe_Log("setVolume")
        IYouMeVoiceEngine::getInstance()->setVolume( RandParam::GetInt(0, 100 ) );
    });
    
    
    addFunc([](){
        YouMe_Log("getUseMobileNetworkEnabled")
        IYouMeVoiceEngine::getInstance()->getUseMobileNetworkEnabled();
    });
    
    addFunc([](){
        YouMe_Log("setUseMobileNetworkEnabled")
        IYouMeVoiceEngine::getInstance()->setUseMobileNetworkEnabled(RandParam::GetBool());
    });
    
    addFunc([=](){
        YouMe_Log("joinChannelSingleMode")
        IYouMeVoiceEngine::getInstance()->joinChannelSingleMode( m_role.c_str() , m_room.c_str(), YOUME_USER_COMMANDER);
    });
    
    addFunc([=](){
        YouMe_Log("joinChannelMultiMode")
        IYouMeVoiceEngine::getInstance()->joinChannelMultiMode(m_role.c_str() , m_room.c_str());
    });
    
    
    addFunc([=](){
        YouMe_Log("speakToChannel")
        IYouMeVoiceEngine::getInstance()->speakToChannel( m_room.c_str());
    });
    
    addFunc([=](){
        YouMe_Log("leaveChannelMultiMode")
        IYouMeVoiceEngine::getInstance()->leaveChannelMultiMode(m_room.c_str());
    });
    
    addFunc([](){
        YouMe_Log("leaveChannelAll")
        IYouMeVoiceEngine::getInstance()->leaveChannelAll();
    });
    
    
    addFunc([=](){
        YouMe_Log("getChannelUserList")
        IYouMeVoiceEngine::getInstance()->getChannelUserList(m_room.c_str(), RandParam::GetInt(5, 20 ), RandParam::GetBool() );
    });
    
    addFunc([=](){
        YouMe_Log("setOtherMicMute")
        IYouMeVoiceEngine::getInstance()->setOtherMicMute( m_roleto.c_str(), RandParam::GetBool() );
    });
    
    addFunc([=](){
        YouMe_Log("setOtherSpeakerMute")
        IYouMeVoiceEngine::getInstance()->setOtherSpeakerMute(m_roleto.c_str(), RandParam::GetBool() );
    });
    
    
    addFunc([=](){
        YouMe_Log("setListenOtherVoice")
        IYouMeVoiceEngine::getInstance()->setListenOtherVoice(m_roleto.c_str(), RandParam::GetBool());
    });
    
    addFunc([](){
        YouMe_Log("playBackgroundMusic")
        IYouMeVoiceEngine::getInstance()->playBackgroundMusic("path", RandParam::GetBool());
    });
    
    addFunc([](){
        YouMe_Log("pauseBackgroundMusic")
        IYouMeVoiceEngine::getInstance()->pauseBackgroundMusic();
    });
    
    
    addFunc([](){
        YouMe_Log("resumeBackgroundMusic")
        IYouMeVoiceEngine::getInstance()->resumeBackgroundMusic();
    });

    
    addFunc([](){
        YouMe_Log("stopBackgroundMusic")
        IYouMeVoiceEngine::getInstance()->stopBackgroundMusic();
    });
    
    addFunc([](){
        YouMe_Log("setBackgroundMusicVolume")
        IYouMeVoiceEngine::getInstance()->setBackgroundMusicVolume( RandParam::GetInt(0, 100 ) );
    });
    
    
    addFunc([](){
        YouMe_Log("setHeadsetMonitorOn")
        IYouMeVoiceEngine::getInstance()->setHeadsetMonitorOn( RandParam::GetBool() );
    });
    
    addFunc([](){
        YouMe_Log("setReverbEnabled")
        IYouMeVoiceEngine::getInstance()->setReverbEnabled(RandParam::GetBool() );
    });
    
    addFunc([](){
        YouMe_Log("setVadCallbackEnabled")
        IYouMeVoiceEngine::getInstance()->setVadCallbackEnabled(RandParam::GetBool());
    });
    
    
    addFunc([](){
        YouMe_Log("setMicLevelCallback")
        IYouMeVoiceEngine::getInstance()->setMicLevelCallback(RandParam::GetInt(1, 100 ));
    });
    
    addFunc([](){
        YouMe_Log("setReleaseMicWhenMute")
        IYouMeVoiceEngine::getInstance()->setReleaseMicWhenMute(RandParam::GetBool() );
    });
    
    addFunc([](){
        YouMe_Log("pauseChannel")
        IYouMeVoiceEngine::getInstance()->pauseChannel();
    });
    
    addFunc([](){
        YouMe_Log("resumeChannel")
        IYouMeVoiceEngine::getInstance()->resumeChannel();
    });
    
    addFunc([](){
        YouMe_Log("setRecordingTimeMs")
        IYouMeVoiceEngine::getInstance()->setRecordingTimeMs( RandParam::GetInt( 10 , 100 ) );
    });
    
    addFunc([](){
        YouMe_Log("setPlayingTimeMs")
        IYouMeVoiceEngine::getInstance()->setPlayingTimeMs(20);
    });
    
    addFunc([](){
        YouMe_Log("getSDKVersion")
        IYouMeVoiceEngine::getInstance()->getSDKVersion();
    });

    
    addFunc([](){
        YouMe_Log("requestRestApi")
        int requestID = 0;
        IYouMeVoiceEngine::getInstance()->requestRestApi("", "", & requestID);
    });
    
    addFunc([=](){
        YouMe_Log("setGrabMicOption")
        IYouMeVoiceEngine::getInstance()->setGrabMicOption( m_room.c_str() ,1,3, 20, 2);
    });
    
    addFunc([=](){
        YouMe_Log("startGrabMicAction")
        IYouMeVoiceEngine::getInstance()->startGrabMicAction( m_room.c_str(),"my message");
    });
    
    addFunc([=](){
        YouMe_Log("stopGrabMicAction")
        IYouMeVoiceEngine::getInstance()->stopGrabMicAction( m_room.c_str(),"my message");
    });
    
    addFunc([=](){
        YouMe_Log("requestGrabMic")
        IYouMeVoiceEngine::getInstance()->requestGrabMic( m_room.c_str(),10, RandParam::GetBool(), "my content");
    });

    
    addFunc([=](){
        YouMe_Log("releaseGrabMic")
        IYouMeVoiceEngine::getInstance()->releaseGrabMic(m_room.c_str());
    });
    
    addFunc([=](){
        YouMe_Log("setInviteMicOption")
        IYouMeVoiceEngine::getInstance()->setInviteMicOption(m_room.c_str(), 10, 20);
    });

    
    addFunc([=](){
        YouMe_Log("requestInviteMic")
        IYouMeVoiceEngine::getInstance()->requestInviteMic(m_room.c_str(), m_role.c_str(), "content");
    });

    
    addFunc([=](){
        YouMe_Log("responseInviteMic")
        IYouMeVoiceEngine::getInstance()->responseInviteMic(m_role.c_str(), RandParam::GetBool(), "content");
    });

    addFunc([](){
        YouMe_Log("stopInviteMic")
        IYouMeVoiceEngine::getInstance()->stopInviteMic();
    });

    
    

    
    

    
    
}

