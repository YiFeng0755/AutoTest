//
//  IMService.cpp
//  MyCppGame
//
//  Created by 杜红 on 2017/1/22.
//
//

#include "imservice.hpp"
#include <string>
#include <ctime>


USING_NS_CC;

IMService* IMService::_instance = NULL;

IMService * IMService::getInstance()
{
    if(NULL == _instance) {
        _instance = new IMService();
    }
    return _instance;
}

IMService::IMService()
      : strTempDir(XStringToLocal(wav_save_path))
{
    IM_SetMode(2);  // 0：正式服  2：测试服
}

IMService::~IMService()
{
}

void IMService::init()
{
    YIMErrorcode ymErrorcode = YIMManager::CreateInstance()->Init(APPKEY, APPSECRET, __XT(""));
    
    CHECK_RETURN_CODE(ymErrorcode, "Initilize call success", "Initilize call failed");
}
