#ifndef ImWrapper_H
#define ImWrapper_H

#include "YIM.h"

class ImWrapper : public IYIMChatRoomCallback,
        public IYIMContactCallback,
        public IYIMDownloadCallback,
        public IYIMLoginCallback,
        public IYIMMessageCallback,
        public IYIMNoticeCallback
{
public:
    ImWrapper();
    ~ImWrapper();

    virtual void OnLogin(YIMErrorcode, const XString&){}
    //登出回调
    virtual void OnLogout(YIMErrorcode){ }
    //被踢下线
    virtual void OnKickOff(){ }

    //加入频道回调
    virtual void OnJoinChatRoom(YIMErrorcode, const XString) { }
    //离开频道回调
    virtual void OnLeaveChatRoom(YIMErrorcode , const XString& ) { }
    //其他用户加入频道通知
    virtual void OnUserJoinChatRoom(const XString&, const XString&) { }
    //其他用户退出频道通知
    virtual void OnUserLeaveChatRoom(const XString&, const XString&) { }

    virtual void OnGetRoomMemberCount(YIMErrorcode, const XString&,
                                      unsigned int) { }

    //获取最近联系人回调
    virtual void OnGetRecentContacts(YIMErrorcode,
                                     std::list<std::shared_ptr<IYIMContactsMessageInfo> >& /*contactList*/) { }
    //获取用户信息回调(用户信息为JSON格式)
    virtual void OnGetUserInfo(YIMErrorcode,
                               const XString& /*userID*/, const XString&  /*userInfo*/) { }
    //查询用户状态回调
    virtual void OnQueryUserStatus(YIMErrorcode,
                                   const XString&  userID, YIMUserStatus status) { }
    //发送消息回调
    virtual void OnSendMessageStatus(XUINT64 /*requestID*/, YIMErrorcode,
                                     bool /*isForbidRoom*/,  int /*reasonType*/, XUINT64 /*forbidEndTime*/ ) { }
    //停止语音回调（调用StopAudioMessage停止语音之后，发送语音消息之前）
    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode,
                                         const XString& text, const XString& audioPath, unsigned int audioTime) { }
    //发送语音消息回调
    virtual void OnSendAudioMessageStatus(XUINT64 /*requestID*/, YIMErrorcode,
                                          const XString& /*text*/, const XString& /*audioPath*/,
                                          unsigned int /*audioTime*/,
                                          bool /*isForbidRoom*/,
                                          int /*reasonType*/, XUINT64 /*forbidEndTime*/) { }
    //收到消息
    virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) { }
    //获取消息历史纪录回调
    virtual void OnQueryHistoryMessage(YIMErrorcode,
                                       const XString& /*targetID*/,
                                       int /*remain*/, std::list<std::shared_ptr<IYIMMessage> > /*messageList*/) { }
    //获取房间历史纪录回调
    virtual void OnQueryRoomHistoryMessage(YIMErrorcode,
                                           std::list<std::shared_ptr<IYIMMessage> > /*messageList*/) { }
    //语音上传后回调
    virtual void OnStopAudioSpeechStatus(YIMErrorcode,
                                         std::shared_ptr<IAudioSpeechInfo> /*audioSpeechInfo*/) { }

    //新消息通知（只有SetReceiveMessageSwitch设置为不自动接收消息，才会收到该回调）
    virtual void OnReceiveMessageNotify(YIMChatType /*chatType*/,
                                        const XString&  /*targetID*/) { }

    //文本翻译完成回调
    virtual void OnTranslateTextComplete(YIMErrorcode,
                                         unsigned int /*requestID*/, const XString& /*text*/,
                                         LanguageCode /*srcLangCode*/, LanguageCode /*destLangCode*/) { }
    //举报处理结果通知
    virtual void OnAccusationResultNotify(AccusationDealResult /*result*/,
                                          const XString& /*userID*/, unsigned int /*accusationTime*/) { }

    virtual void OnGetForbiddenSpeakInfo( YIMErrorcode,
                                          std::vector< std::shared_ptr<IYIMForbidSpeakInfo> > /*vecForbiddenSpeakInfos*/ ) { }

    virtual void OnRecvNotice(YIMNotice * /*notice*/) {}

    /*
    * 功能：屏蔽/解除屏蔽用户消息回调
    * @param errorcode：错误码
    * @param userID
    * @param block true-屏蔽 false-解除屏蔽
    */
    virtual void OnBlockUser(YIMErrorcode,
                             const XString& /*userID*/, bool /*block*/) { }

    /*
    * 功能：解除所有已屏蔽用户回调
    * @param errorcode 错误码
    */
    virtual void OnUnBlockAllUser(YIMErrorcode) { }

    /*
    * 功能：获取被屏蔽消息用户回调
    * @param errorcode：错误码
    * @param userList userID集
    */
    virtual void OnGetBlockUsers(YIMErrorcode, std::list<XString>) { }

private:

};

#endif // ImWrapper_H
