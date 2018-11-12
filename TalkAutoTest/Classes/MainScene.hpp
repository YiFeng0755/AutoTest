//
//  MainScene.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/4.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <cocos2d.h>
//#include "AutoRandTest.hpp"
#include "ui/CocosGUI.h"

#include "network/HttpClient.h"
using namespace cocos2d::network;

class MainScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void switchToImTestCallback(cocos2d::Ref *pSender);
    void switchToTalkTestCallback(cocos2d::Ref *pSender);
	void switchToPerfCallback(cocos2d::Ref *pSender);
    void startIMAutoTestCallback( cocos2d::Ref * pSender );
    void startTalkAutoTestCallback( cocos2d::Ref * pSender );
    void sendLogCallback(cocos2d::Ref *pSender);

    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
private:
    //IMAutoRandTest imTest;
    //TalkAutoRandTest talkTest;

    CREATE_FUNC(MainScene);
};


#endif /* MainScene_hpp */
