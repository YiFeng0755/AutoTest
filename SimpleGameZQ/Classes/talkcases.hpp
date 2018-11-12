//
//  talkcases.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/8/2.
//
//

#ifndef talkcases_hpp
#define talkcases_hpp
#include "callbackwrapper.hpp"
#include <atomic>
#include <condition_variable>
#include <mutex>

//接口功能测试
class TalkCasesController : public TalkCallbackWrapper
{
public:
    TalkCasesController();
    ~TalkCasesController();

    void runTests();
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
    void onMemberChange( const char* channel, std::list<MemberChange>& listMemberChange ) override;
    void onBroadcast(const YouMeBroadcast bc, const std::string& channel, const std::string& param1, const std::string& param2, const std::string& strContent) override;
    
private:
    void actionUninit();
    void actionInit();
    void actionJoinSingleRoom();
    void actionLeaveSingleChannel();
    void actionJoinMultiRoom();
    void actionLeaveMultiMode();
    void actionJoinRoomZhubo();

private:
    std::string m_robotuser;

    std::condition_variable  m_cv;
    std::mutex m_mutex;
};
#endif /* talkcases_hpp */
