//
//
//  IMService.hpp
//  MyCppGame
//
//  Created by 杜红 on 2017/1/22.
//
//

#ifndef IMService_hpp
#define IMService_hpp

#include <stdio.h>
#include "YIM.h"
#include <string>
using namespace std;

struct Msg{
public:
    unsigned long long msgID = 0;   //消息ID，自己发送的没有消息ID
    string sender ="";              //发送者
    bool  bSelf = false ;           //是否自己发送
    unsigned int createTime = 0 ;   //发送时间
    bool bTxt = false ;             //是否文本
    string content = "";            //如果是文本，则是文本内容，如果是语音，则是语音检测出的文本
    string voicePath ="";           //语音的保存地址
    int voiceLen = 0 ;              //语音长度
};

class IMService : public IYIMLoginCallback,  public IYIMMessageCallback,  public IYIMChatRoomCallback, public IYIMDownloadCallback, public IYIMContactCallback,
public IYIMLocationCallback, public IYIMNoticeCallback
{
public:
    static IMService * getInstance();
    static IMService* _instance;
    
    virtual ~IMService();
    
    //
    void start();
    
    //登录回调
    void OnLogin(YIMErrorcode errorcode, const XString&  userID)override;
    //登出回调
    void OnLogout(YIMErrorcode errorcode)override ;
    //被踢下线
    void OnKickOff()override;
    
    
    void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime )override ;
    //发送语音消息回调
    void OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime) override;
    //收到消息
    void OnRecvMessage( std::shared_ptr<IYIMMessage> message)override ;
    
    void OnQueryRoomHistoryMessage(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> > messageList)override;
    //语音上传后回调
    void OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo)override;
    
    //新消息通知（只有SetReceiveMessageSwitch设置为不自动接收消息，才会收到该回调）
    void OnReceiveMessageNotify(YIMChatType chatType,  const XString&  targetID)override;
    //获取消息历史纪录回调
    void OnQueryHistoryMessage(YIMErrorcode errorcode,const XString& targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList)override ;
    
    
    //加入频道回调
    void OnJoinChatRoom(YIMErrorcode errorcode,const XString&  chatRoomID)override ;
    //离开频道回调
    void OnLeaveChatRoom(YIMErrorcode errorcode, const XString&  chatRoomID) override;
    
    //获取最近联系人回调
    void OnGetRecentContacts(YIMErrorcode errorcode, std::list<XString>& contactList)override;
    //获取用户信息回调(用户信息为JSON格式)
    void OnGetUserInfo(YIMErrorcode errorcode, const XString& userID, const XString&  userInfo)override;
    
    
    void OnDownload( YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> msg, const XString& savePath )override;
    
    void OnDownloadByUrl( YIMErrorcode errorcode, const XString& strFromUrl, const XString& savePath )override;
    
    virtual void OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID)override;
    //其他用户退出频道通知
    virtual void OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID) override ;
    
    virtual void OnAccusationResultNotify(AccusationDealResult result, const XString& userID, unsigned int accusationTime)override;
    
    //语音文件保存的路径
    string strTempDir;

};



#endif /* IMService_hpp */
