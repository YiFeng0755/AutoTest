﻿#ifndef TEST_API_SUIT_1
#define TEST_API_SUIT_1

#include <QObject>
#include "YIM.h"
#include "YIMPlatformDefine.h"


class ImWrapper :
        public IYIMLocationCallback,
        public IYIMAudioPlayCallback,
        public IYIMContactCallback,
        public IYIMLoginCallback,
        public IYIMDownloadCallback,
        public IYIMMessageCallback,
        public IYIMReconnectCallback,
        public IYIMNoticeCallback,
        public IYIMChatRoomCallback

{

public:
    ImWrapper() {}

    // 获取自己位置回调
    virtual void OnUpdateLocation(YIMErrorcode errorcode, std::shared_ptr<GeographyLocation> location) {}
    // 获取附近目标回调
    virtual void OnGetNearbyObjects(YIMErrorcode errorcode, std::list< std::shared_ptr<RelativeLocation> > neighbourList, unsigned int startDistance, unsigned int endDistance) {}

    /*
    * 功能：获取与指定用户距离回调
    * @param errorcode：错误码
    * @param userID 用户ID
    * @param distance 距离（米）
    */
    virtual void OnGetDistance(YIMErrorcode errorcode, const XString& userID, unsigned int distance){}
    //播放完成
    virtual void OnPlayCompletion(YIMErrorcode errorcode, const XString& path) {}
    //获取麦克风状态回调
    virtual void OnGetMicrophoneStatus(AudioDeviceStatus status)  {}

    virtual void OnGetRecentContacts(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMContactsMessageInfo> >& contactList) {}
    //获取用户信息回调(用户信息为JSON格式)
    virtual void OnGetUserInfo(YIMErrorcode errorcode, const XString& userID, const XString&  userInfo) {}
    //查询用户状态回调
    virtual void OnQueryUserStatus(YIMErrorcode errorcode, const XString&  userID, YIMUserStatus status){}

    //登录回调
    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID) {}
    //登出回调
    virtual void OnLogout(YIMErrorcode errorcode) {}
    //被踢下线
    virtual void OnKickOff() {}

    //virtual void OnDownload(XUINT64 messageID, YIMErrorcode errorcode, const XString& savePath) {}
    virtual void OnDownload( YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> msg, const XString& savePath ){}

    virtual void OnDownloadByUrl( YIMErrorcode errorcode, const XString& strFromUrl, const XString& savePath ){}

    // 收到公告
    virtual void OnRecvNotice(YIMNotice* notice) {}
    // 撤销公告（置顶公告）
    virtual void OnCancelNotice(XUINT64 noticeID, const XString& channelID)  {}

    // 开始重连
    virtual void OnStartReconnect() {}
    // 收到重连结果
    virtual void OnRecvReconnectResult(ReconnectResult result) {}

    //加入频道回调
    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {}
    //离开频道回调
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {}
    //其他用户加入频道通知
    virtual void OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID)  {}
    //其他用户退出频道通知
    virtual void OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID) {}
    /*
    * 功能：获取房间成员数量回调
    * @param errorcode：错误码
    * @param chatRoomID：房间ID
    * @param count：成员数量
    */
    virtual void OnGetRoomMemberCount(YIMErrorcode errorcode, const XString& chatRoomID, unsigned int count){}

    //发送消息回调
    virtual void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, unsigned int sendTime, bool isForbidRoom, int reasonType, XUINT64 forbidEndTime) {}
    //停止语音回调（调用StopAudioMessage停止语音之后，发送语音消息之前）
    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime) {}
    //发送语音消息回调
    virtual void OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime, unsigned int sendTime,bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime) {}
    //收到消息
    virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) {}
    //获取消息历史纪录回调
    virtual void OnQueryHistoryMessage(YIMErrorcode errorcode, const XString& targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList) {}

    /*
    * 功能：从服务器查询房间历史消息回调（每次最多30条）
    * @param errorcode：错误码
    * @param roomID 房间ID
    * @param remain 剩余消息数量
    * @param messageList 消息列表
    */
    virtual void OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode, const XString& roomID, int remain, std::list<std::shared_ptr<IYIMMessage> >& messageList){}

    /*
     * 功能：从服务器查询私聊历史消息回调（每次最多20条）
     * @param errorcode：错误码
     * @param userID 私聊用户ID
     * @param messageList 消息列表
     */
    virtual void OnQuerySessionHistoryMsgFromServer(YIMErrorcode errorcode, const XString& userID, std::list<std::shared_ptr<IYIMMessage> >& messageList){}

    //语音上传后回调
    virtual void OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo) {}

    //新消息通知（只有SetReceiveMessageSwitch设置为不自动接收消息，才会收到该回调）
    virtual void OnReceiveMessageNotify(YIMChatType chatType,  const XString&  targetID) {}

    //文本翻译完成回调
    virtual void OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString& text, LanguageCode srcLangCode, LanguageCode destLangCode) {}
    //举报处理结果通知
    virtual void OnAccusationResultNotify(AccusationDealResult result, const XString& userID, unsigned int accusationTime) {}

    virtual void OnGetForbiddenSpeakInfo( YIMErrorcode errorcode, std::vector< std::shared_ptr<IYIMForbidSpeakInfo> > vecForbiddenSpeakInfos ) {}

    //语音文本的识别回调
    virtual void OnGetRecognizeSpeechText(XUINT64 requestID, YIMErrorcode errorcode, const XString& text) {}

    /*
    * 功能：屏蔽/解除屏蔽用户消息回调
    * @param errorcode：错误码
    * @param userID
    * @param block true-屏蔽 false-解除屏蔽
    */
    virtual void OnBlockUser(YIMErrorcode errorcode, const XString& userID, bool block) {}

    /*
    * 功能：解除所有已屏蔽用户回调
    * @param errorcode 错误码
    */
        virtual void OnUnBlockAllUser(YIMErrorcode errorcode) {}

    /*
    * 功能：获取被屏蔽消息用户回调
    * @param errorcode：错误码
    * @param userList userID集
    */
    virtual void OnGetBlockUsers(YIMErrorcode errorcode, std::list<XString> userList) {}

    /*
     * 功能：带文字识别录音音量变化回调
     * @param volume：音量值(0到30)，频率1s 12~13次
     */
    virtual void OnRecordVolume(int volume) {}
};


#endif // TEST_API_SUIT_1

