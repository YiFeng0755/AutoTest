//
//  IMService.cpp
//  MyCppGame
//
//  Created by 杜红 on 2017/1/22.
//
//

#include "IMService.hpp"
#include <string>
#include <ctime>
#include "cocos2d.h"
USING_NS_CC;
#include  "YouMeVoiceEngineImp.hpp"

IMService* IMService::_instance = NULL;
extern string g_roomName;
extern string g_userID;
#include "AutoRandTest.hpp"
extern IMAutoRandTest* g_imtest ;

IMService * IMService::getInstance()
{
    if(NULL == _instance)
    {
        _instance = new IMService();
    }
    return _instance;
}

IMService::~IMService(){
    
}


void IMService::start(){
    //YOUME
    //注册各个回调代理类
    YIMManager::CreateInstance()->SetLoginCallback(this);
    YIMManager::CreateInstance()->SetMessageCallback(this);
    YIMManager::CreateInstance()->SetChatRoomCallback(this);
    YIMManager::CreateInstance()->SetDownloadCallback(this);
    YIMManager::CreateInstance()->SetContactCallback(this);
    YIMManager::CreateInstance()->SetNoticeCallback( this );
    YIMManager::CreateInstance()->SetLocationCallback( this );
    
    //YOUME
    /*!
     *  初始化
     *  @param appKey 用户游戏产品区别于其它游戏产品的标识，可以在游密官网获取、查看
     *  @param appSecurity 用户游戏产品的密钥，可以在游密官网获取、查看
     *  @param packageName 目前填""就好
     *  @return 错误码
     */
    YIMErrorcode ymErrorcode = YIMManager::CreateInstance()->Init("YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0",                                                "r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=",
                                                "");
    if(ymErrorcode != YIMErrorcode_Success)
    {
        YouMe_Log("初始化失败，错误码：%d\n",  ymErrorcode);
    }
    else{
        YouMe_Log("初始化成功!\n");
    }

}

void IMService::OnLogin(YIMErrorcode errorcode, const XString& userID){
    YouMe_Log("登录回调：%d\n", errorcode );
    
    if( errorcode != 0 ){
        return ;
    }
    
    //登陆成功后，进入房间
    /*!
     *  加入聊天频道
     *  @param roomID 频道ID
     *  @return 错误码
     */
//    YIMErrorcode ymErrorcode;
//    ymErrorcode = YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom( g_roomName.c_str() );
//    if(ymErrorcode != YIMErrorcode_Success)
//    {
//        YouMe_Log("进入聊天频道失败，错误码：%d\n",  ymErrorcode);
//    }
//    else{
//        YouMe_Log("进入聊天频道成功!\n");
//    }
}
//登出回调
void IMService::OnLogout(YIMErrorcode errorcode){
    YouMe_Log("登出回调：%d\n", errorcode );
}

//被踢下线
void IMService::OnKickOff(){
    YouMe_Log("被踢下线通知\n" );
}

void IMService::OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime ){
    YouMe_Log("发送消息回调，请求ID：%llu,错误码：%d\n",requestID, errorcode );
}
//发送语音消息回调
void IMService::OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime ){
    YouMe_Log("发送语音消息回调，请求ID：%llu,错误码：%d\n",requestID, errorcode );
    
    //YIMErrorcode_PTT_ReachMaxDuration，发送语音有时长限制，超长时会自动发消息，此时错误码是ReachMaxDuration
    if( errorcode == YIMErrorcode_Success || errorcode == YIMErrorcode_PTT_ReachMaxDuration  ){
        EventCustom evtVoice("SendVoice");
        Msg* msg = new Msg();
        msg->sender = g_userID ;
        msg->bSelf = true ;
        msg->createTime = time(NULL);
        msg->bTxt = false;
        msg->voicePath = audioPath;
        msg->content = text;
        msg->voiceLen = audioTime;
        evtVoice.setUserData( msg );
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evtVoice);
    }
}
//收到消息
void IMService::OnRecvMessage(std::shared_ptr<IYIMMessage> message){
    YouMe_Log("收到消息回调\n");
    YIMMessageBodyType msgType = message->GetMessageBody()->GetMessageType();
    if( msgType == YIMMessageBodyType::MessageBodyType_TXT ){
        IYIMMessageBodyText* pMsgText = (IYIMMessageBodyText*)message->GetMessageBody();
        
        YouMe_Log("收到文本消息： %s，发送者：%s, 接受者：%s",
                  pMsgText->GetMessageContent(),
                  message->GetReceiveID(),
                  message->GetSenderID() );
        
        EventCustom evtText("RecvText");
        Msg* msg = new Msg();
        msg->msgID = message->GetMessageID();
        msg->sender = message->GetSenderID();
        msg->bSelf = false;
        msg->createTime = message->GetCreateTime();
        msg->bTxt = true;
        msg->content = pMsgText->GetMessageContent();
        evtText.setUserData( msg );
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evtText);
        
    }
    else if ( msgType == YIMMessageBodyType::MessageBodyType_Voice ){
        IYIMMessageBodyAudio* pMsgVoice = (IYIMMessageBodyAudio*)message->GetMessageBody();
        YouMe_Log("收到语音消息，文本内容： %s, 消息ID： %llu, 附加参数： %s ,语音时长： %d\n",
                  pMsgVoice->GetText(),
                  message->GetMessageID(),
                  pMsgVoice->GetExtraParam(),
                  pMsgVoice->GetAudioTime() );
        
        //下载语音文件
//        stringstream fileS;
//        fileS<<strTempDir<<message->GetMessageID()<<".wav";
//        std::string file = fileS.str();
//        YIMManager::CreateInstance()->GetMessageManager()->DownloadFile( message->GetMessageID(), file.c_str() );
        
        std::lock_guard< mutex > lock( g_imtest->m_paramMutex );
        g_imtest->m_recvMessageID = message->GetMessageID();
        
//        //通知场景
//        EventCustom evtVoice("RecvVoice");
//        Msg* msg = new Msg();
//        msg->msgID = message->GetMessageID();
//        msg->sender = message->GetSenderID();
//        msg->bSelf = false ;
//        msg->createTime = message->GetCreateTime();
//        msg->bTxt = false;
//        msg->voicePath = file.c_str();
//        msg->content = pMsgVoice->GetText();
//        msg->voiceLen = pMsgVoice->GetAudioTime();
//        evtVoice.setUserData( msg );
//        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evtVoice);
    }
}

void IMService::OnQueryRoomHistoryMessage(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> > messageList){
    //暂时不需要，所以没处理
}

#include <iostream>
using namespace std;

void IMService::OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo>  audioSpeechInfo){
    if( errorcode == 0 ){
        cout<<"id:"<<audioSpeechInfo->GetRequestID()<<endl;
        cout<<"text:"<<audioSpeechInfo->GetText()<<endl;
        cout<<"time:"<<audioSpeechInfo->GetAudioTime()<<endl;
        cout<<"url:"<<audioSpeechInfo->GetDownloadURL()<<endl;
    }
    
    std::lock_guard< mutex > lock( g_imtest->m_paramMutex );
    g_imtest->m_url = audioSpeechInfo->GetDownloadURL();
    
//    //下载语音文件
//    stringstream fileS;
//    fileS<<strTempDir<<audioSpeechInfo->GetRequestID()<<".wav";
//    std::string file = fileS.str();
//    YIMManager::CreateInstance()->GetMessageManager()->DownloadFile( audioSpeechInfo->GetDownloadURL(), file.c_str() );
    
    
}

void IMService::OnReceiveMessageNotify(YIMChatType chatType,  const XString&  targetID){
    //暂时不需要，所以没处理
}

void IMService::OnQueryHistoryMessage(YIMErrorcode errorcode, const XString& targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList){
    //暂时不需要，所以没处理
}


//加入频道回调
void IMService::OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID){
    YouMe_Log("进入聊天频道回调\n");
    if( errorcode == YIMErrorcode_Success ){
        YouMe_Log("进入聊天频道 %s 成功 \n", chatRoomID.c_str() );
    }
    else{
        YouMe_Log("进入聊天频道 %s 失败, 错误码： %d \n", chatRoomID.c_str(), errorcode );
    }
    
}
//离开频道回调
void IMService::OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID){
    YouMe_Log("离开聊天频道回调\n");
    if( errorcode == YIMErrorcode_Success ){
        YouMe_Log("离开聊天频道 %s 成功 ", chatRoomID.c_str() );
    }
    else{
        YouMe_Log("离开聊天频道 %s 失败, 错误码 ： %d\n ", chatRoomID.c_str(), errorcode );
    }
    
}

void IMService::OnDownload( YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> message, const XString& savePath ){
    YouMe_Log("下载文件回调\n"  );
    if( errorcode == YIMErrorcode_Success ){
        YouMe_Log("下载文件 %s 成功 \n", savePath.c_str() );
        
        IYIMMessageBodyAudio* pMsgVoice = (IYIMMessageBodyAudio*)message->GetMessageBody();
        YouMe_Log("收到语音消息，文本内容： %s, 消息ID： %llu, 附加参数： %s ,语音时长： %d\n",
                  pMsgVoice->GetText(),
                  message->GetMessageID(),
                  pMsgVoice->GetExtraParam(),
                  pMsgVoice->GetAudioTime() );
        
        //通知场景
        EventCustom evtDownloadOK("DownloadOK");
        Msg* msg = new Msg();
        msg->msgID = message->GetMessageID();
        msg->sender = message->GetSenderID();
        msg->bSelf = false ;
        msg->createTime = message->GetCreateTime();
        msg->bTxt = false;
        msg->voicePath = savePath.c_str();
        msg->content = pMsgVoice->GetText();
        msg->voiceLen = pMsgVoice->GetAudioTime();
        evtDownloadOK.setUserData( msg );
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent( &evtDownloadOK );
    }
    else{
        YouMe_Log("下载文件 %s 失败, error = %d \n ", savePath.c_str(), errorcode );
    }
    
}

void IMService::OnDownloadByUrl( YIMErrorcode errorcode, const XString& strFromUrl, const XString& savePath ){
    YouMe_Log("下载文件回调,err:%d, url:%s\n", errorcode, strFromUrl.c_str() );
    
    YouMe_Log("下载文件回调,path:%s\n", savePath.c_str()   );
}

//获取最近联系人回调
void IMService::OnGetRecentContacts(YIMErrorcode errorcode, std::list<XString>& contactList){
    //暂时不需要，所以没处理
    
}
//获取用户信息回调(用户信息为JSON格式)
void IMService::OnGetUserInfo(YIMErrorcode errorcode, const XString& userID, const XString&  userInfo){
    //暂时不需要，所以没处理
}

void IMService::OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID)
{
    
}
//其他用户退出频道通知
void IMService::OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID)
{
    
}

void IMService::OnAccusationResultNotify(AccusationDealResult result, const XString& userID, unsigned int accusationTime)
{
    
}
