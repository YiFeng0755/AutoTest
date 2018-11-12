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

#include "network/HttpClient.h"
using namespace cocos2d::network;

class MainScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    
    void startIMAutoTestCallback2( cocos2d::Ref * pSender );
    void startTalkAutoTestCallback2( cocos2d::Ref * pSender );
    
    
    void openLogCallback( cocos2d::Ref * pSender );
    void closeLogCallback( cocos2d::Ref * pSender );
    
    void sendLogCallback( cocos2d::Ref * pSender );
    
private:
    CREATE_FUNC(MainScene);
};


#endif /* MainScene_hpp */
