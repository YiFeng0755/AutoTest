//
//  talkservice.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#ifndef talkservice_hpp
#define talkservice_hpp

#include "IYouMeEventCallback.h"
#include "IYouMeVoiceEngine.h"
#include "YIMPlatformDefine.h"
#include <stdio.h>

//const static char *TALK_APPKEY = "YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0";
//const static char *TALK_APPSECRET = "r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=";

const static XCHAR *TALK_APPKEY = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");

const static XCHAR *TALK_APPSECRET = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");
static const char *TALK_USER_A = "OOOO9999"; // Talk最常用userid
static const char *TALK_USER_B = "OOOO8888";
static const char *TALK_ZHUBO_ID = "ZHUBOOOOO10001";
static const char *TALK_CHAT_ROOM = "1112418";

static std::string verToString(int num) {
    int main = num >> 28;
    int minor = (num & 0x0FC00000) >> 22;
    int release = (num & 0x003FC000) >> 14;
    int build = (num & 0x00003FFF);

    char str[20] = {0};
    sprintf(str, "%d.%d.%d.%d", main, minor, release, build);
    return std::string(str);
}

class TalkService :
    public IYouMeEventCallback, //talk callback
    public IYouMePcmCallback,
    public IRestApiCallback,
    public IYouMeMemberChangeCallback
{
public:
    static TalkService *getInstance();
    IYouMeVoiceEngine* getTalkEngine();
    ~TalkService();

    void init();
    /***talk 回调函数**/
    void onEvent(const YouMeEvent event, const YouMeErrorCode error,
                 const char *channel, const char * param) override;

    
    void onRequestRestAPI( int requestID, const YouMeErrorCode &iErrorCode, const char* strQuery, const char*  strResult ) override;

    //void onMemberChange(const char* channel,
    //                    std::list<MemberChange>& listMemberChange) override;
    void onMemberChange( const char* channel, const char* listMemberChange , bool bUpdate ) override;
    //void onBroadcast(const YouMeBroadcast bc,
    //                         const char* channel,
    //                         const char* param1,
    //                         const char* param2,
    //                         const char* strContent) override;
//    virtual void onMemberChange( const char* channel, std::list<MemberChange>& listMemberChange, bool bUpdate ) {} //七牛

    virtual void onPcmData(int channelNum,
                           int samplingRateHz,
                           int bytesPerSample,
                           void* data,
                           int dataSizeInByte) override;
private:
    TalkService();
};

#endif /* talkservice_hpp */
