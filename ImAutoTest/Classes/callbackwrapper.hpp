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

class IMCallbackWrapper :
  public IYIMLoginCallback
, public IYIMMessageCallback
, public IYIMContactCallback
, public IYIMChatRoomCallback
, public IYIMLocationCallback
, public IYIMNoticeCallback
, public IYIMAudioPlayCallback
, public IYIMDownloadCallback
, public IYIMFriendCallback
, public IYIMUserProfileCallback
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
    virtual void OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> > messageList) {}
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

    // **好友管理**
    virtual void OnFindUser(YIMErrorcode errorcode, std::shared_ptr<std::shared_ptr<IYIMUserBriefInfo> >& users ){}
    
    // 请求添加好友
    virtual void OnRequestAddFriend(YIMErrorcode errorcode, const XString& userID ) {}
    
    // 被邀请添加好友通知（需要验证）
    virtual void OnBeRequestAddFriendNotify(const XString& userID, const XString& comments ) {}
    
    // 被添加为好友通知（不需要验证）
    virtual void OnBeAddFriendNotify(const XString& userID, const XString& comments ) {}
    
    // 处理被请求添加好友结果回调
    virtual void OnDealBeRequestAddFriend(YIMErrorcode errorcode, const XString& userID, const XString& comments, int dealResult){}
    
    // 请求添加好友结果回调
    virtual void OnRequestAddFriendResultNotify(const XString& userID, const XString& comments, int dealResult){}
    
    // 删除好友结果回调
    virtual void OnDeleteFriend(YIMErrorcode errorcode, const XString& userID){}
    
    // 被好友删除通知
    virtual void OnBeDeleteFriendNotify(const XString& userID){}
    
    // 拉黑或解除拉黑好友回调
    virtual void OnBlackFriend(YIMErrorcode errorcode, int type, const XString& userID){}
        
    // 查询我的好友回调
    virtual void OnQueryFriends(YIMErrorcode errorcode, int type, int startIndex, std::list<std::shared_ptr<IYIMUserBriefInfo> >& friends){}
    
    // 查询好友请求列表回调
    virtual void OnQueryFriendRequestList(YIMErrorcode errorcode, int startIndex, std::list<std::shared_ptr<IYIMFriendRequestInfo> >& requestList){}
    
    // **资料管理**
    // 功能：查询用户信息回调
    virtual void OnQueryUserInfo(YIMErrorcode errorcode, const IMUserProfileInfo &userInfo) {}
    
    // 功能：设置用户信息回调
    virtual void OnSetUserInfo(YIMErrorcode errorcode) {}
    
    // 功能：切换用户在线状态回调
    virtual void OnSwitchUserOnlineState(YIMErrorcode errorcode) {}
    
    // 功能：设置头像回调
    virtual void OnSetPhotoUrl(YIMErrorcode errorcode, const XString &photoUrl) {}
    
    // 功能：用户信息变更通知
    virtual void OnUserInfoChangeNotify(const XString& userID){}

};








#endif /* callbackwrapper_hpp */
