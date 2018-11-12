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
#include <IYouMeVideoCallback.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern void SetServerMode(SERVER_MODE servermode);
#endif

//接口功能测试
class TalkCallbackWrapper :
    public IYouMeEventCallback,
    public IYouMeAudioFrameCallback,
    public IYouMeVideoFrameCallback,
    public IYouMeChannelMsgCallback,
    public IYouMePcmCallback,
    public IRestApiCallback,
    public IYouMeAVStatisticCallback,
    public IYouMeCustomDataCallback
{
public:
    virtual void onEvent(const YouMeEvent /*event*/, const YouMeErrorCode error, const char * channel, const char * param) override { }

    virtual void onAudioFrameCallback(std::string userId, void* data, int len, uint64_t timestamp) override { }

    virtual void onAudioFrameMixedCallback(void* data, int len, uint64_t timestamp) override { };

    virtual void onVideoFrameCallback(std::string userId, void * data, int len, int width, int height, int fmt, uint64_t timestamp) override { }

    virtual void onVideoFrameMixedCallback(void * data, int len, int width, int height, int fmt, uint64_t timestamp) override { }

    virtual void onPcmData(int channelNum, int samplingRateHz, int bytesPerSample, void* data, int dataSizeInByte) override { }

    virtual void onBroadcast(const YouMeBroadcast bc, const char* channel, const char* param1, const char* param2, const char* strContent) override { }

    virtual void onAVStatistic( YouMeAVStatisticType type,  const char* userID,  int value ) override { }

//    virtual void frameRender(int renderId, int nWidth, int nHeight, int nRotationDegree, int nBufSize, const void * buf) override { }

    virtual void onRequestRestAPI( int requestID, const YouMeErrorCode &iErrorCode, const char* strQuery, const char*  strResult ) override { }

    virtual void OnCustomDataNotify(const void*pData, int iDataLen, unsigned long long ulTimeSpan) override { }
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

//    virtual void frameRender(int renderId, int nWidth, int nHeight, int nRotationDegree, int nBufSize, const void * buf) override;

    virtual void onVideoFrameCallback(std::string userId, void * data, int len, int width, int height, int fmt, uint64_t timestamp) override;

    virtual void onAudioFrameMixedCallback(void* data, int len, uint64_t timestamp) override;

    virtual void OnCustomDataNotify(const void*pData, int iDataLen, unsigned long long ulTimeSpan) override;

    void runTests();

    void actionInputData();

    void sendemail();

    std::string getRobotUser() {
        return m_robotuser;
    }

private:
    void actionUninit();
    void actionInit();
    void actionJoinSingleRoom();
    void actionLeaveSingleChannel();
    void actionJoinMultiRoom();
    void actionLeaveMultiMode();
    void actionJoinRoomZhubo();
    void testThreadFunc();

    void testTalkMode(); //通话接口 音量调节等等
    void testFreeVideoMode();
    void testHostMode();
private:
    std::string m_robotuser;

    std::condition_variable  m_cv;
    std::mutex m_mutex;

    std::thread _run_thread;
};
#endif /* talkcases_hpp */
