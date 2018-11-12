//
//  YouMeVoiceEngineImp.cpp
//  TalkCppSample
//
//  Created by  on 2017/3/21.
//
//

#include "YouMeVoiceEngineImp.hpp"
#include <iostream>
#include <string>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

//进入的房间ID
string g_roomName = "";
//登录的频道ID
string g_userID = "" ;
//进入频道的模式
YouMeUserRole g_roleType = YOUME_USER_NONE;



YouMeVoiceEngineImp*  YouMeVoiceEngineImp::getInstance(){
    static YouMeVoiceEngineImp  imp;
    
    return &imp;
}

//向场景发送提示消息用于显示
void addTips( string strTips ){
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        EventCustom evtTips("Tips");
        string* pTips = new string( strTips.c_str() );
    
        evtTips.setUserData( pTips );
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evtTips);
    });
}

void YouMeVoiceEngineImp::onEvent(const YouMeEvent event, const YouMeErrorCode error, const char * channel, const char * param){
    YouMe_Log("事件回调：%d, err:%d, 频道id:%s, param:%s", event, error, channel,param );
    
    stringstream ssTips;
    switch (event) {
        case YOUME_EVENT_INIT_OK:
            YouMe_Log("Talk 初始化成功");
            addTips("初始化成功");
            break;
        case YOUME_EVENT_INIT_FAILED:
            YouMe_Log("Talk 初始化失败");
            addTips("初始化失败");
            break;
        case YOUME_EVENT_JOIN_OK:{
            ssTips<<"进入房间成功，ID："<<channel;
            YouMe_Log("%s", ssTips.str().c_str());
            addTips( ssTips.str() );
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                EventCustom evtEnter("EnterRoom");
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&evtEnter);
            });
        }
            break;
        case YOUME_EVENT_JOIN_FAILED:
            ssTips<<"进入房间成功，ID："<<channel;
            YouMe_Log("%s",ssTips.str().c_str());
            addTips( ssTips.str() );
            break;
        case YOUME_EVENT_LEAVED_ONE:
            YouMe_Log("Talk 离开单个房间");
            addTips( "离开单个房间" );
            break;
        case YOUME_EVENT_LEAVED_ALL:
            YouMe_Log("Talk 离开所有房间");
            addTips( "离开房间" );
            break;
        case YOUME_EVENT_PAUSED:
            YouMe_Log("Talk 暂停");
            addTips( "暂停" );
            break;
        case YOUME_EVENT_RESUMED:
            YouMe_Log("Talk 恢复");
            addTips( "恢复" );
            break;
        case YOUME_EVENT_SPEAK_SUCCESS:///< 切换对指定频道讲话成功（适用于多频道模式）
            break;
        case YOUME_EVENT_SPEAK_FAILED:///< 切换对指定频道讲话失败（适用于多频道模式）
            break;
        case YOUME_EVENT_RECONNECTING:///< 断网了，正在重连
            YouMe_Log("Talk 正在重连");
            break;
        case YOUME_EVENT_RECONNECTED:///< 断网重连成功
            YouMe_Log("Talk 重连成功");
            break;
        case YOUME_EVENT_REC_FAILED:///< 通知录音启动失败（此时不管麦克风mute状态如何，都没有声音输出）
            YouMe_Log("录音启动失败,%s",param);
            break;
        case YOUME_EVENT_BGM_STOPPED:///< 通知背景音乐播放结束
            YouMe_Log("背景音乐播放结束,%s",param);
            addTips( "背景音乐播放结束" );
            break;
        case YOUME_EVENT_BGM_FAILED:///< 通知背景音乐播放失败
            YouMe_Log("背景音乐播放失败,%s",param);
            addTips( "背景音乐播放失败" );
            break;
        case YOUME_EVENT_OTHERS_MIC_ON:///< 其他用户麦克风打开
            YouMe_Log("其他用户麦克风打开,%s",param);
            break;
        case YOUME_EVENT_OTHERS_MIC_OFF:///< 其他用户麦克风关闭
            YouMe_Log("其他用户麦克风关闭,%s",param);
            break;
        case YOUME_EVENT_OTHERS_SPEAKER_ON:///< 其他用户扬声器打开
            YouMe_Log("其他用户扬声器打开,%s",param);
            break;
        case YOUME_EVENT_OTHERS_SPEAKER_OFF: ///< 其他用户扬声器关闭
            YouMe_Log("其他用户扬声器关闭,%s",param);
            break;
        default:
            break;
    }
    
    
}
