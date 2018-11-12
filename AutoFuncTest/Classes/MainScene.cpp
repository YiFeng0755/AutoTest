//
//  MainScene.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/4.
//
//

#include "MainScene.hpp"
#include "cocostudio/CocoStudio.h" 
#include "ui/CocosGUI.h"

extern "C"
{
    void IM_SetLogLevel(int iLogLevel);
}

USING_NS_CC;
using namespace ui;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    layer->setColor(Color3B::WHITE);
    //layer->setPosition(400, 400);
    scene->addChild(layer, 0);
    return scene;
}

bool MainScene::init()
{
    if (!LayerColor::init()) {
        return false;
    }
    
   // this->addChild(nullptr);
    this->setColor(Color3B(255, 0, 128));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
 
    
<<<<<<< .mine
||||||| .r14357
    auto btnTabAuto2 = Button::create();
    btnTabAuto2->setTitleText("IM 2");
    btnTabAuto2->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabAuto2->setTitleFontSize(40.0f);
    //btnTabTalk->setAnchorPoint(Vec2(0.5, 0.5));
    btnTabAuto2->setPosition(Vec2(origin.x + visibleSize.width / 2  - 120, origin.y + visibleSize.height - 200));
    btnTabAuto2->addClickEventListener(CC_CALLBACK_1(MainScene::startIMAutoTestCallback, this));

    

    
    auto btnTabAutoTalk = Button::create();
    btnTabAutoTalk->setTitleText("Talk 1");
    btnTabAutoTalk->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabAutoTalk->setTitleFontSize(40.0f);
    btnTabAutoTalk->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120 , origin.y + visibleSize.height - 100));
    btnTabAutoTalk->addClickEventListener(CC_CALLBACK_1(MainScene::startTalkAutoTestCallback, this));
    
    auto btnTabAutoTalk2 = Button::create();
    btnTabAutoTalk2->setTitleText("Talk 2");
    btnTabAutoTalk2->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabAutoTalk2->setTitleFontSize(40.0f);
    
    btnTabAutoTalk2->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120 , origin.y + visibleSize.height - 200));
    btnTabAutoTalk2->addClickEventListener(CC_CALLBACK_1(MainScene::startTalkAutoTestCallback, this));

    
    auto btnSendLog = Button::create();
    btnSendLog->setTitleText("SendLog");
    btnSendLog->setTitleAlignment(TextHAlignment::RIGHT);
    btnSendLog->setTitleFontSize(40.0f);
    
    btnSendLog->setPosition(Vec2(visibleSize.width / 2 , origin.y + visibleSize.height - 300));
    btnSendLog->addClickEventListener(CC_CALLBACK_1(MainScene::sendLogCallback, this));



    addChild(btnTabAuto);
    addChild(btnTabAuto2);
    addChild( btnTabAutoTalk );
    addChild( btnTabAutoTalk2 );
    addChild( btnSendLog );
    
    checker.startCheck();
=======
    auto btnTabAuto2 = Button::create();
    btnTabAuto2->setTitleText("IM 2");
    btnTabAuto2->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabAuto2->setTitleFontSize(40.0f);
    //btnTabTalk->setAnchorPoint(Vec2(0.5, 0.5));
    btnTabAuto2->setPosition(Vec2(origin.x + visibleSize.width / 2  - 120, origin.y + visibleSize.height - 200));
    btnTabAuto2->addClickEventListener(CC_CALLBACK_1(MainScene::startIMAutoTestCallback, this));

    

    
    auto btnTabAutoTalk = Button::create();
    btnTabAutoTalk->setTitleText("Talk 1");
    btnTabAutoTalk->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabAutoTalk->setTitleFontSize(40.0f);
    btnTabAutoTalk->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120 , origin.y + visibleSize.height - 100));
    btnTabAutoTalk->addClickEventListener(CC_CALLBACK_1(MainScene::startTalkAutoTestCallback, this));
    
    auto btnTabAutoTalk2 = Button::create();
    btnTabAutoTalk2->setTitleText("Talk 2");
    btnTabAutoTalk2->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabAutoTalk2->setTitleFontSize(40.0f);
    
    btnTabAutoTalk2->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120 , origin.y + visibleSize.height - 200));
    btnTabAutoTalk2->addClickEventListener(CC_CALLBACK_1(MainScene::startTalkAutoTestCallback, this));
    
    auto btnSendLog = Button::create();
    btnSendLog->setTitleText("SendLog");
    btnSendLog->setTitleAlignment(TextHAlignment::RIGHT);
    btnSendLog->setTitleFontSize(40.0f);
    
    btnSendLog->setPosition(Vec2(visibleSize.width / 2 , origin.y + visibleSize.height - 300));
    btnSendLog->addClickEventListener(CC_CALLBACK_1(MainScene::sendLogCallback, this));
    
    
    auto btnOpenLog = Button::create();
    btnOpenLog->setTitleText("开日志");
    btnOpenLog->setTitleAlignment(TextHAlignment::RIGHT);
    btnOpenLog->setTitleFontSize(40.0f);
    btnOpenLog->setPosition(Vec2(origin.x + visibleSize.width / 2 - 120 , origin.y + visibleSize.height - 400));
    btnOpenLog->addClickEventListener(CC_CALLBACK_1(MainScene::openLogCallback, this));
    
    auto btnCloseLog = Button::create();
    btnCloseLog->setTitleText("关日志");
    btnCloseLog->setTitleAlignment(TextHAlignment::RIGHT);
    btnCloseLog->setTitleFontSize(40.0f);
    
    btnCloseLog->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120 , origin.y + visibleSize.height - 400));
    btnCloseLog->addClickEventListener(CC_CALLBACK_1(MainScene::closeLogCallback, this));




    addChild(btnTabAuto);
    addChild(btnTabAuto2);
    addChild( btnTabAutoTalk );
    addChild( btnTabAutoTalk2 );
    addChild( btnSendLog );
    
    addChild( btnOpenLog );
    addChild( btnCloseLog );
    
    checker.startCheck();
>>>>>>> .r15282
    return true;
}




<<<<<<< .mine
||||||| .r14357
}

void MainScene::startTalkAutoTestCallback2( cocos2d::Ref * pSender ){
    talkTest.registAllTest();
    imTest.m_role = "456";
    imTest.m_roleto = "123";
    imTest.m_room = "w123";
    talkTest.StartTest(3);
    
}


#include <YouMeCommon/DownloadUploadManager.h>
#include <iostream>
using namespace std;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif

void Log( const std::string&  str ){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    int
//    int iAndroidLogLevel = LogLevelToAndroidLevel (dLevel);
    __android_log_write (ANDROID_LOG_DEBUG, "YOUMEIM", str.c_str() );
#else
    printf("%s",str.c_str());
#endif
}

void MainScene::sendLogCallback( cocos2d::Ref * pSender ){
    
    checker.m_fileMutex.lock();
    if( checker.m_file.IsOpen() ){
        checker.m_file.Close();
    }
    
    std::map<std::string, std::string> httpHead;
    
    std::string strResponse;
    
    cout<<checker.m_path<<endl;
    
    auto ret = CDownloadUploadManager::UploadFileWithForm( "http://106.75.7.162:8999/sendreport", checker.m_path, httpHead,  strResponse );
    if( !ret ){
        cout<<"upload file"<<endl;
    }
    
    Log("上传日志结果");
    Log(strResponse);
    
    checker.openFile( checker.m_path );
    
    checker.m_fileMutex.unlock();




}




=======
}

void MainScene::startTalkAutoTestCallback2( cocos2d::Ref * pSender ){
    talkTest.registAllTest();
    imTest.m_role = "456";
    imTest.m_roleto = "123";
    imTest.m_room = "w123";
    talkTest.StartTest(3);
    
}

void MainScene::openLogCallback( cocos2d::Ref * pSender ){
    IM_SetLogLevel( 100 );
}
void MainScene::closeLogCallback( cocos2d::Ref * pSender ){
    IM_SetLogLevel( 0 );
}


#include <YouMeCommon/DownloadUploadManager.h>
#include <iostream>
using namespace std;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif

void Log( const std::string&  str ){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    int
//    int iAndroidLogLevel = LogLevelToAndroidLevel (dLevel);
    __android_log_write (ANDROID_LOG_DEBUG, "YOUMEIM", str.c_str() );
#else
    printf("%s",str.c_str());
#endif
}

void MainScene::sendLogCallback( cocos2d::Ref * pSender ){
    
    checker.m_fileMutex.lock();
    if( checker.m_file.IsOpen() ){
        checker.m_file.Close();
    }
    
    std::map<std::string, std::string> httpHead;
    
    std::string strResponse;
    
    cout<<checker.m_path<<endl;
    
    auto ret = CDownloadUploadManager::UploadFileWithForm( "http://106.75.7.162:8999/sendreport", checker.m_path, httpHead,  strResponse );
    if( !ret ){
        cout<<"upload file"<<endl;
    }
    
    Log("上传日志结果");
    Log(strResponse);
    
    checker.openFile( checker.m_path );
    
    checker.m_fileMutex.unlock();




}




>>>>>>> .r15282
