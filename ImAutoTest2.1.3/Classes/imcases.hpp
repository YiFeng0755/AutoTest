//
//  imcases.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#ifndef imcases_hpp
#define imcases_hpp

#include "autocontroller.hpp"
#include "callbackwrapper.hpp"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <map>
#include <atomic>
#include <thread>

#include "network/HttpRequest.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace network;
#define CURVER (YIMM)
static const XCHAR* PROCOLOL_FROM_PREFIX = __XT("AutoTestFromRobot");//收到消息的检验头
static const XCHAR* PROTOCOL_TO_PREFIX = __XT("AutoTestToRobot"); //发出消息的检验头

static const XCHAR* ROOM_ID = __XT("2019899");
static const XCHAR* ROBOT_TARHET_ID = __XT("AutoRoboter");

static long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static bool isStartWith(const std::string & sub, const std::string& mainStr) {
    auto len_sub = sub.size();
    auto len_p = mainStr.size();

    if (len_sub > len_p)
        return false;

    std::string::size_type i = 0;
    for (; i < len_sub; ++i) {
        if (sub[i] != mainStr[i])
            return false;
    }
    return true;
}

struct Conifg
{
    int timeout;//等待回调的超时时间 s
};

class IMTestController: public IMCallbackWrapper
{
public:
    IMTestController();
    ~IMTestController();

    void runTests();

    //group 1
    void init_cases(); //设置回调

    //group 2
    void message_cases(); //文本消息
    void user_mgr_cases();
    //group 3
    void voice_cases(); //voice

    //group 4
    void leave_cases(); //clear
    
   // void haoyou_cases();  // 好友管理
   // void userinfo_cases();  // 用户资料管理
	void voiceRecord_cases();//录音
    void actionJoinroom();
    void actionLogout();
    void actionLeaveChatRoom();

    void requestSendMsg1(); //调http发普通文本
    void requestSendMsg2(); //调http发脏词
    void requestSendMsg3(); //调http发翻译文本
    void requestEnterRoom();
    void requestLeaveRoom();
    void requestNotice();
    void requestCancelNotice(XUINT64 id);

    void actionSetOpenSwitch();
    void actionSetgetUserinfo();
    void actionGetrecentcontact(); //获取最近联系人
    void actionQuerymessage();
    void actionJubao();
    void requestForbid(); //禁言请求
    void translate(const XCHAR* content);

    //语音
    //voices
    void voicesetting();
    void actionSendVoiceMsgChat();
    void actionSendVoiceMsgPrivate();
    void actionPlayAudio();
    void actionSendVoiceSpeech();
    //lbs位置
    void actionGetCurrentLocation();
    void actionGetNearbyObjects();

    void sendemail();
public:
    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID) override;
    virtual void OnLogout(YIMErrorcode errorcode) override;
    //virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) override;
    virtual void OnRecvNotice(YIMNotice* notice) override;
    virtual void OnCancelNotice(XUINT64 noticeID, const XString& channelID) override;
    virtual void OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString& text, LanguageCode srcLangCode, LanguageCode destLangCode) override;
    virtual void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime ) override;
    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) override;
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) override;

    virtual void OnGetRecentContacts(YIMErrorcode errorcode,
                             std::list<XString>& contactList) override;

    virtual void OnGetRoomMemberCount(YIMErrorcode errorcode, const XString& chatRoomID,
                                      unsigned int count) override;

    virtual void OnQueryHistoryMessage(YIMErrorcode errorcode, const XString& targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList) override;
    //获取房间历史纪录回调
    virtual void OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode,
                                                     const XString& roomID, int remain, std::list<std::shared_ptr<IYIMMessage> >& messageList) override;
    virtual void OnGetUserInfo(YIMErrorcode errorcode, const XString& userID, const XString&  userInfo) override;

    virtual void OnRecvMessage(std::shared_ptr<IYIMMessage> message) override;

    void OnReceiveMessageNotify(YIMChatType chatType,  const XString&  targetID) override;

    virtual void OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID) override;
    virtual void OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID) override;
    virtual void OnAccusationResultNotify(AccusationDealResult result, const XString& userID, unsigned int accusationTime) override;
    
    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime) override;
    //发送语音消息回调
    virtual void OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime,
                                          bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime) override;
    //语音上传后回调
    virtual void OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo) override;

    // 获取自己位置回调
    virtual void OnUpdateLocation(YIMErrorcode errorcode, std::shared_ptr<GeographyLocation> location) override;
    // 获取附近目标回调
    virtual void OnGetNearbyObjects(YIMErrorcode errorcode, std::list< std::shared_ptr<RelativeLocation> > neighbourList, unsigned int startDistance, unsigned int endDistance) override;

    virtual void OnDownload( YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> msg, const XString& savePath )override;

    virtual void OnDownloadByUrl(YIMErrorcode errorcode, const XString& strFromUrl, const XString& savePath )override;

    virtual void OnPlayCompletion(YIMErrorcode errorcode,
                                  const XString& path) override;


    virtual void OnBlockUser(YIMErrorcode errorcode, const XString& userID, bool block) override;
    virtual void OnUnBlockAllUser(YIMErrorcode errorcode) override;
    virtual void OnGetBlockUsers(YIMErrorcode errorcode, std::list<XString> userList) override;
    virtual void OnKickOff() override;
	virtual void OnRecordVolumeChange(float volume) override;
    
    // **好友管理**
//    //查找用户回调
//    virtual void OnFindUser(YIMErrorcode errorcode, std::shared_ptr<std::shared_ptr<IYIMUserBriefInfo> >& users ) override;
//    // 请求添加好友回调
//    virtual void OnRequestAddFriend(YIMErrorcode errorcode, const XString& userID ) override;
//    // 被邀请添加好友通知（需要验证）
//    virtual void OnBeRequestAddFriendNotify(const XString& userID, const XString& comments ) override;
//    // 被添加为好友通知（不需要验证）
//    virtual void OnBeAddFriendNotify(const XString& userID, const XString& comments ) override;
//
//    // 处理被请求添加好友回调
//    virtual void OnDealBeRequestAddFriend(YIMErrorcode errorcode, const XString& userID, const XString& comments, int dealResult) override;
//
//    // 请求添加好友结果回调
//    virtual void OnRequestAddFriendResultNotify(const XString& userID, const XString& comments, int dealResult) override;
//
//    // 功能：删除好友结果回调
//    virtual void OnDeleteFriend(YIMErrorcode errorcode, const XString& userID) override;
//
//    // 功能：被好友删除通知
//    virtual void OnBeDeleteFriendNotify(const XString& userID) override;
//
//    // 功能：拉黑或解除拉黑好友回调
//    virtual void OnBlackFriend(YIMErrorcode errorcode, int type, const XString& userID) override;
//
//    // 功能：查询我的好友回调
//    virtual void OnQueryFriends(YIMErrorcode errorcode, int type, int startIndex, std::list<std::shared_ptr<IYIMUserBriefInfo> >& friends) override;
//
//    // 功能：查询好友请求列表回调
//    virtual void OnQueryFriendRequestList(YIMErrorcode errorcode, int startIndex, std::list<std::shared_ptr<IYIMFriendRequestInfo> >& requestList) override;
//
//
//    // **资料管理**
//    // 功能：查询用户信息回调
//    virtual void OnQueryUserInfo(YIMErrorcode errorcode, const IMUserProfileInfo &userInfo) override;
//
//    // 功能：设置用户信息回调
//    virtual void OnSetUserInfo(YIMErrorcode errorcode) override;
//
//    // 功能：切换用户在线状态回调
//    virtual void OnSwitchUserOnlineState(YIMErrorcode errorcode) override;
//
//    // 功能：设置头像回调
//    virtual void OnSetPhotoUrl(YIMErrorcode errorcode, const XString &photoUrl) override;
//
//    // 功能：用户信息变更通知
//    virtual void OnUserInfoChangeNotify(const XString& userID) override;

    

private:
    void runCaseThreadFunc();

    Conifg * m_config;
    XString m_robotUser;
    int m_time_cnt;
    
    std::condition_variable m_cv;
    std::mutex m_mutex;

    std::thread m_exe_thread;
};

//
//class VoiceCaseController : public IMCallbackWrapper
//{
//public:
//    VoiceCaseController();
//    ~VoiceCaseController();
//
//    void case_send_voice_private();
//    void case_send_voice_chatroom();
//    void case_send_voice_65s_chatroom();
//    void case_send_voidce_1s_chatroom();
//
//public:
//    virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) override;
//    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime) override;
//    //发送语音消息回调
//    virtual void OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime,
//                                          bool isForbidRoom,  int reasonType, XUINT64 forbidEndTime) override;
//
//private:
//    std::condition_variable m_cv;
//    std::mutex m_mutex;
//};

#endif /* imcases_hpp */
