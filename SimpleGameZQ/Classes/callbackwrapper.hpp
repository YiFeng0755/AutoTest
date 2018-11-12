//
//  callbackwrapper.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#ifndef callbackwrapper_hpp
#define callbackwrapper_hpp

#include "YIM.h"
#include "IYouMeEventCallback.h"

class IMCallbackWrapper :
  public IYIMLoginCallback
, public IYIMMessageCallback
, public IYIMContactCallback
, public IYIMChatRoomCallback
, public IYIMLocationCallback
, public IYIMNoticeCallback
, public IYIMAudioPlayCallback
, public IYIMDownloadCallback
{
    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID) {}
    //登出回调
    virtual void OnLogout(YIMErrorcode errorcode) {}
    //被踢下线
    virtual void OnKickOff() {}
    // 收到公告
    virtual void OnRecvNotice(YIMNotice* notice) {}
    // 撤销公告（置顶公告）
    virtual void OnCancelNotice(XUINT64 noticeID, const XString& channelID)  {}

    // 获取自己位置回调
    virtual void OnUpdateLocation(YIMErrorcode errorcode, std::shared_ptr<GeographyLocation> location) {}
    // 获取附近目标回调
    virtual void OnGetNearbyObjects(YIMErrorcode errorcode, std::list< std::shared_ptr<RelativeLocation> > neighbourList, unsigned int startDistance, unsigned int endDistance) {}

    //加入频道回调
    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {}
    //离开频道回调
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {}
    //其他用户加入频道通知
    virtual void OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID)  {}
    //其他用户退出频道通知
    virtual void OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID) {}

    virtual void OnGetRoomMemberCount(YIMErrorcode errorcode, const XString& chatRoomID,
                                      unsigned int count){}
    //获取最近联系人回调
    virtual void OnGetRecentContacts(YIMErrorcode errorcode, std::list<XString>& contactList) {}
    //获取用户信息回调(用户信息为JSON格式)
    virtual void OnGetUserInfo(YIMErrorcode errorcode, const XString& userID, const XString&  userInfo) {}
    //查询用户状态回调
    virtual void OnQueryUserStatus(YIMErrorcode errorcode, const XString&  userID, YIMUserStatus status){}

    //发送消息回调
    virtual void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime ) {}
    //停止语音回调（调用StopAudioMessage停止语音之后，发送语音消息之前）
    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime) {}
    //发送语音消息回调
    virtual void OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime,
                                          bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime) {}
    //收到消息
    virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) {}
    //获取消息历史纪录回调
    virtual void OnQueryHistoryMessage(YIMErrorcode errorcode, const XString& targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList) {}
    //获取房间历史纪录回调
    virtual void OnQueryRoomHistoryMessage(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> > messageList) {}
    //语音上传后回调
    virtual void OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo) {}

    //新消息通知（只有SetReceiveMessageSwitch设置为不自动接收消息，才会收到该回调）
    virtual void OnReceiveMessageNotify(YIMChatType chatType,  const XString&  targetID) {}

    //文本翻译完成回调
    virtual void OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString& text, LanguageCode srcLangCode, LanguageCode destLangCode) {}
    //举报处理结果通知
    virtual void OnAccusationResultNotify(AccusationDealResult result, const XString& userID, unsigned int accusationTime) {}

    virtual void OnGetForbiddenSpeakInfo( YIMErrorcode errorcode, std::vector< std::shared_ptr<IYIMForbidSpeakInfo> > vecForbiddenSpeakInfos ) {}

    //播放完成
    virtual void OnPlayCompletion(YIMErrorcode errorcode, const XString& path) {}
    //获取麦克风状态回调
    virtual void OnGetMicrophoneStatus(AudioDeviceStatus status)  {}

    virtual void OnDownload( YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> msg, const XString& savePath ){}

    virtual void OnDownloadByUrl( YIMErrorcode errorcode, const XString& strFromUrl, const XString& savePath ){}


    virtual void OnBlockUser(YIMErrorcode errorcode, const XString& userID, bool block) {}

    virtual void OnUnBlockAllUser(YIMErrorcode errorcode) {};

    virtual void OnGetBlockUsers(YIMErrorcode errorcode, std::list<XString> userList) {}
    
};

class TalkCallbackWrapper :
  public IRestApiCallback
, public IYouMeEventCallback
, public IYouMePcmCallback
, public IYouMeMemberChangeCallback
//, public IYouMeChannelMsgCallback
{
public:
    virtual void onBroadcast(const YouMeBroadcast bc, const std::string& channel, const std::string& param1, const std::string& param2, const std::string& strContent) {}
    virtual void onPcmData(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte) {}
    virtual void onMemberChange( const char* channel, std::list<MemberChange>& listMemberChange ) {}
    virtual void onEvent(const YouMeEvent event, const YouMeErrorCode error, const char * channel, const char * param) {}
    virtual void onRequestRestAPI( int requestID, const YouMeErrorCode &iErrorCode, const char* strQuery, const char*  strResult ){}
//    virtual void onMemberChange( const char* channel, std::list<MemberChange>& listMemberChange, bool bUpdate ) {} ;
    virtual void onMemberChange( const char* channel, const char* listMemberChange , bool bUpdate ) {};
};







#endif /* callbackwrapper_hpp */
