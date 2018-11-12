//
//  autotestscene.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/28.
//
//

#include "autotestscene.hpp"
#include "autocontroller.hpp"
#include "platform/CCApplication.h"
#include "imcases.hpp"
#include "talkservice.hpp"

USING_NS_CC;
using namespace ui;
using namespace std;

cocos2d::Scene* IMAutoTestScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IMAutoTestScene::create();
    scene->addChild(layer);
    return scene;
}

bool IMAutoTestScene::init()
{
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    _btn_start_1 = Button::create();
    _btn_start_1->setTitleText("IM自动化测试");
    _btn_start_1->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::startImTest, this));
    _btn_start_1->setPosition(Vec2(origin.x + visibleSize.width /2.0, origin.y + visibleSize.height / 2.0));
    _btn_start_1->setTitleFontSize(20.0);
    addChild(_btn_start_1);

    _btn_start_2 = Button::create();
    _btn_start_2->setTitleText("Talk自动化测试");
    _btn_start_2->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::startTalkTest, this));
    _btn_start_2->setPosition(Vec2(origin.x + visibleSize.width /2.0, origin.y + visibleSize.height / 2.0 + 25.0));
    _btn_start_2->setTitleFontSize(20.0);
    addChild(_btn_start_2);

    _btn_report = Button::create();
    _btn_report->setTitleText("测试报告目录");
    _btn_report->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::report, this));
    _btn_report->setPosition(Vec2(origin.x + visibleSize.width/2.0,
                                  origin.y + visibleSize.height / 2.0 - 30));
    _btn_report->setTitleFontSize(20.0);
    addChild(_btn_report);

    _btn_send_email = Button::create();
    _btn_send_email->setTitleText("发送IM报告邮件");
    _btn_send_email->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::sendEmail, this));
    _btn_send_email->setPosition(Vec2(origin.x + visibleSize.width/2.0,
                                  origin.y + visibleSize.height / 2.0 - 80));
    _btn_send_email->setTitleFontSize(20.0);
    addChild(_btn_send_email);

    _btn_stable_test = Button::create();
    _btn_stable_test->setTitleText("稳定性测试Demo");
    _btn_stable_test->setPosition(Vec2(origin.x + visibleSize.width /2.0,
                                       origin.y + visibleSize.height/2.0 + 55));
    _btn_stable_test->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::stableTest, this));
    _btn_stable_test->setTitleFontSize(20.0);
    addChild(_btn_stable_test);

    _btn_voice = Button::create();
    _btn_voice->setTitleText("发送语音");
    _btn_voice->setPosition(Vec2(origin.x + visibleSize.width / 2.0 - 180, origin.y + visibleSize.height / 2.0 ));
    _btn_voice->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::voiceTest, this));
    _btn_voice->setTitleFontSize(20.0f);
    addChild(_btn_voice);

    _btn_geo = Button::create();
    _btn_geo->setTitleText("获取地理位置");
    _btn_geo->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::geoTest, this));
    _btn_geo->setPosition(Vec2(origin.x + visibleSize.width / 2.0 + 180,
                               origin.y + visibleSize.height / 2.0));
    _btn_geo->setTitleFontSize(20.0f);
    addChild(_btn_geo);

    _label = Label::createWithTTF("S:", "fonts/arial.ttf", 10.0);
    std::string ver = cocos2d::StringUtils::format("IM Ver: %d; Talk Ver: %s", YIMManager::CreateInstance()->GetSDKVersion(), verToString(IYouMeVoiceEngine::getInstance()->getSDKVersion()).c_str());


    _label->setString(ver);
    _label->setPosition(Vec2(origin.x + visibleSize.width /2.0,
                                       origin.y + visibleSize.height/2.0 + 70.0));
    addChild(_label);


    _label_for_progress = Label::createWithTTF("Progress", "fonts/arial.ttf", 12.0);
    _label_for_progress->setString("Click button to begin...");
    _label_for_progress->setPosition(Vec2(origin.x + visibleSize.width / 3,
                                          origin.y + visibleSize.height/2.0 + 80.0));
    addChild(_label_for_progress);
    return true;
}

void IMAutoTestScene::startImTest()
{
    _label_for_progress->setString("running im autotest... ");
    im_controller.runTests();
    _label_for_progress->setString("Finished!");
}

void IMAutoTestScene::startTalkTest()
{
    _label_for_progress->setString("running talk autotest... ");
    talk_controller.runTests();
    _label_for_progress->setString("Finished!");
}

void IMAutoTestScene::geoTest()
{
    im_controller.actionGetCurrentLocation();
}

void IMAutoTestScene::voiceTest()
{
    im_controller.actionSendVoiceMsgChat();
}

void IMAutoTestScene::report()
{
    std::string filename = ::filename();
    cocos2d::log("Local file path : %s", filename.c_str());
    _label->setString(filename);
    Application::getInstance()->openURL(filename);
}

void IMAutoTestScene::sendEmail()
{
    im_controller.sendemail();
}

#include "testbase/controller.h"
void IMAutoTestScene::stableTest()
{
    static bool flag = false;
    if (!flag) {
        TestController::getInstance();
        flag = true;
    }
    else {
        TestController::destroyInstance();
        TestController::getInstance();
    }
}
