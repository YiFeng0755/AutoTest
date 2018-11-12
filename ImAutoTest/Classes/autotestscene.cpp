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

	auto *chnStrings = Dictionary::createWithContentsOfFile("language.xml");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    _btn_start_1 = Button::create();
	 const char* starttetst_text = ((String*)chnStrings->objectForKey("start"))->getCString();

	 _btn_start_1->setTitleText(starttetst_text);
    _btn_start_1->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::startImTest, this));
    _btn_start_1->setPosition(Vec2(origin.x + visibleSize.width /2.0, origin.y + visibleSize.height / 2.0));
    _btn_start_1->setTitleFontSize(20.0);
    addChild(_btn_start_1);


    _btn_report = Button::create();
	const char * reportdirstr = ((String*)chnStrings->objectForKey("reportdir"))->getCString();
	_btn_report->setTitleText(reportdirstr);
    _btn_report->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::report, this));
    _btn_report->setPosition(Vec2(origin.x + visibleSize.width/2.0,
                                  origin.y + visibleSize.height / 2.0 - 30));
    _btn_report->setTitleFontSize(20.0);
    addChild(_btn_report);

    _btn_send_email = Button::create();
	const char * sendreportstr = ((String*)chnStrings->objectForKey("sendmail"))->getCString();
	_btn_send_email->setTitleText(sendreportstr);
    _btn_send_email->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::sendEmail, this));
    _btn_send_email->setPosition(Vec2(origin.x + visibleSize.width/2.0,
                                  origin.y + visibleSize.height / 2.0 - 80));
    _btn_send_email->setTitleFontSize(20.0);
    addChild(_btn_send_email);

    _btn_voice = Button::create();
	const char * sendvoice = ((String*)chnStrings->objectForKey("recoder"))->getCString();
	_btn_voice->setTitleText(sendvoice);
    _btn_voice->setPosition(Vec2(origin.x + visibleSize.width / 2.0 - 180, origin.y + visibleSize.height / 2.0 ));
    _btn_voice->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::voiceTest, this));
    _btn_voice->setTitleFontSize(20.0f);
    addChild(_btn_voice);

    _btn_geo = Button::create();
	const char * locationstr = ((String*)chnStrings->objectForKey("location"))->getCString();
	_btn_geo->setTitleText(locationstr);
    _btn_geo->addClickEventListener(CC_CALLBACK_0(IMAutoTestScene::geoTest, this));
    _btn_geo->setPosition(Vec2(origin.x + visibleSize.width / 2.0 + 180,
                               origin.y + visibleSize.height / 2.0));
    _btn_geo->setTitleFontSize(20.0f);
    addChild(_btn_geo);

    _label = Label::createWithTTF("S:", "fonts/arial.ttf", 10.0);
    std::string ver = cocos2d::StringUtils::format("IM Ver: %d", YIMManager::CreateInstance()->GetSDKVersion());


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

