//
//  YouMeVoiceEngineImp.hpp
//  TalkCppSample
//
//  Created by 杜红 on 2017/3/21.
//
//

#ifndef YouMeVoiceEngineImp_hpp
#define YouMeVoiceEngineImp_hpp

#include <stdio.h>
#include "IYouMeEventCallback.h"

#define YouMe_Log( ... ) 
//printf("_______________YouMe:");printf( __VA_ARGS__ );printf("\n");

class YouMeVoiceEngineImp : public IYouMeEventCallback{
public:
    static YouMeVoiceEngineImp* getInstance();
    void onEvent(const YouMeEvent event, const YouMeErrorCode error, const char * channel, const char * param) ;
};

#endif /* YouMeVoiceEngineImp_hpp */
