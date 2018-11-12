//
//  talkcases.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/8/2.
//
//

#ifndef talkcases_hpp
#define talkcases_hpp
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "IYouMeVoiceEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern void SetServerMode(SERVER_MODE servermode);
#endif


//接口功能测试
class TalkCallbackWrapper :
    public IYouMeEventCallback,
    public IYouMeChannelMsgCallback,
    public IYouMePcmCallback,
    public IYouMeMemberChangeCallback,
    public IRestApiCallback
{
public:
    virtual void onEvent(const YouMeEvent /*event*/, const YouMeErrorCode error, const char * channel, const char * param) override { }

    virtual void onPcmData(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte) override { }

    virtual void onBroadcast(const YouMeBroadcast bc, const char* channel, const char* param1, const char* param2, const char* strContent) override { }

    virtual void onMemberChange( const char* channel, const char* listMemberChange , bool bUpdate ) override { };
private:

};

class TalkCasesController : public TalkCallbackWrapper
{
public:
    void init();
    TalkCasesController();
    ~TalkCasesController();

    void startTest();
    void onEvent(const YouMeEvent event, const YouMeErrorCode error,
                 const char *channel, const char * param) override;
    void onPcmData(int channelNum,
              int samplingRateHz,
              int bytesPerSample,
              void* data,
              int dataSizeInByte) override;
    virtual void onRequestRestAPI( int requestID,
                                  const YouMeErrorCode &iErrorCode,
                                  const char* strQuery,
                                  const char*  strResult ) override;
    
    virtual void onBroadcast(const YouMeBroadcast bc, const char* channel, const char* param1, const char* param2, const char* strContent) override ;

    void sendemail();

    void cases_init();
    void cases_voice_controll();
    void cases_others();
    void cases_room_manager();
    void cases_clear();
    void cases_multi_mode();
    void cases_micgrab();//上下麦
private:
    void actionUninit();
    void actionInit();
    void actionJoinSingleRoom();
    void actionLeaveSingleChannel();
    void actionJoinMultiRoom();
    void actionLeaveMultiMode();
    void actionJoinRoomZhubo();
    void testThreadFunc();

private:
    std::string m_robotuser;

    std::condition_variable  m_cv;
    std::mutex m_mutex;

    std::thread _run_thread;
};
#endif /* talkcases_hpp */
