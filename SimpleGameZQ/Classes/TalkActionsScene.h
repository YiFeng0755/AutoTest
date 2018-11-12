//
//  TalkActionsScene.h
//  SimpleGame
//
//  Created by Micheal Chen on 2017/7/4.
//
//

#ifndef TalkActionsScene_h
#define TalkActionsScene_h

#include "cocos2d.h"

class TalkActionsScene : public cocos2d::Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(TalkActionsScene);
};

#endif /* TalkActionsScene_h */
