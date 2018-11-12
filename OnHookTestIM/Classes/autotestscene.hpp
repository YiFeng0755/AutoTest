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
#include "autocontroller.hpp"
#include "imcases.hpp"


class IMAutoTestScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    //停止所有后台任务

    CREATE_FUNC(IMAutoTestScene);

    void startImTest();
    void report();
    void voiceTest();
    void geoTest();
    void sendEmail();

protected:
    cocos2d::ui::Button *_btn_start_1;
    cocos2d::ui::Button *_btn_start_2;
    cocos2d::ui::Button *_btn_report;
    cocos2d::ui::Button *_btn_stable_test;
    cocos2d::ui::Button *_btn_send_email;

    cocos2d::ui::Button *_btn_voice;
    cocos2d::ui::Button *_btn_geo;

    cocos2d::Label * _label;
    cocos2d::Label * _label_for_progress;

    IMTestController im_controller;
};

#endif /* autotestscene_hpp */
