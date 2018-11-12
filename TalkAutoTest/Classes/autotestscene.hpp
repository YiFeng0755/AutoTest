//
//  autotestscene.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/28.
//
//

#ifndef autotestscene_hpp
#define autotestscene_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <map>
#include "talkcases.hpp"
#include "ui/CocosGUI.h"
#include <thread>


class AutoTestScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(AutoTestScene)
    
    virtual bool init();
    //停止所有后台任务
    void startTalkTest();
    void report();
    void sendemail();

    void joinroom(); //target roboter

protected:
    void initData();

    cocos2d::ui::Button *_btn_start_1;
    cocos2d::ui::Button *_btn_start_2;
    cocos2d::ui::Button *_btn_report;

    cocos2d::ui::Button * _btn_test;

    cocos2d::Label * _label;
    cocos2d::Label * _label_for_progress;

    TalkCasesController talk_controller;

    //加入视频的渲染框
    std::map<cocos2d::RenderTexture *, int> renderTextures;
    std::map<std::string, cocos2d::RenderTexture *>  userRenders;
    cocos2d::RenderTexture * render_recv;
    cocos2d::RenderTexture * render_send;
};

#endif /* autotestscene_hpp */
