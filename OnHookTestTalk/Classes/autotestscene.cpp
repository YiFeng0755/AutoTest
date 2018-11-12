//
//  autotestscene.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/28.
//
//

#include "autotestscene.hpp"
#include "platform/CCApplication.h"
#include "talkservice.hpp"
#include "VisibleRect.h"

USING_NS_CC;
using namespace ui;
using namespace std;

cocos2d::Scene* AutoTestScene::createScene()
{
    auto scene = Scene::create();
    auto layer = AutoTestScene::create();
    scene->addChild(layer);
    return scene;
}

bool AutoTestScene::init()
{
    if (!LayerColor::initWithColor(Color4B(210, 121, 140, 155))) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

//    _btn_start_1 = Button::create();
//    _btn_start_1->setTitleText("发邮件");
//    _btn_start_1->addClickEventListener(CC_CALLBACK_0(AutoTestScene::sendemail,
//                                                      this));
//    _btn_start_1->setPosition(Vec2(origin.x + visibleSize.width /2.0, origin.y
//                                   + visibleSize.height / 2.0 + 25.0));
//    _btn_start_1->setTitleFontSize(20.0);
//    addChild(_btn_start_1);

    _btn_start_2 = Button::create();
    _btn_start_2->setTitleText("Talk自动化测试");
    _btn_start_2->addClickEventListener(CC_CALLBACK_0(AutoTestScene::startTalkTest,
                                                      this));
    _btn_start_2->setPosition(Vec2(origin.x + visibleSize.width /2.0, origin.y
                                   + visibleSize.height / 2.0 + 45.0));
    _btn_start_2->setTitleFontSize(20.0);
    addChild(_btn_start_2);

    _btn_report = Button::create();
    _btn_report->setTitleText("测试报告目录");
    _btn_report->addClickEventListener(CC_CALLBACK_0(AutoTestScene::report,
                                                     this));
    _btn_report->setPosition(Vec2(origin.x + visibleSize.width/2.0,
                                  origin.y + visibleSize.height / 2.0 - 30));
    _btn_report->setTitleFontSize(20.0);

    addChild(_btn_report);

    _btn_test = Button::create();
    _btn_test->setTitleText("进入房间");
    _btn_test->addClickEventListener(CC_CALLBACK_0(AutoTestScene::joinroom,
                                                     this));
    _btn_test->setPosition(Vec2(origin.x + visibleSize.width/2.0,
                                  origin.y + visibleSize.height / 2.0 - 70));
    //addChild(_btn_test);

    _label = cocos2d::Label::create();
    _label->setPosition(Vec2(origin.x + visibleSize.width / 2.0 - 40,
                             origin.y + visibleSize.height / 2.0 - 50));
    _label->setString("path");
    addChild(_label);

    _label_for_progress = cocos2d::Label::create();
    _label_for_progress->setPosition(Vec2(origin.x + visibleSize.width / 2.0,
                                          origin.y + visibleSize.height / 2.0 - 100));
    addChild(_label_for_progress);

    auto director = Director::getInstance();
    visibleSize = director->getVisibleSize();
    origin = director->getVisibleOrigin();
    log("[orgin]%.2f/%.2f [visibleSize]%.2f/%.2f", origin.x, origin.y,
        visibleSize.width, visibleSize.height);
    this->scheduleUpdate();

//    initData();

    //  本地 yuv 测试
//    RenderTexture * rtLocal = RenderTexture::create(160, 120, Texture2D::PixelFormat::RGB888);
//    rtLocal->setPosition(origin.x + 352 , origin.y + 288);
//    rtLocal->setRotation(270);
//    this->addChild(rtLocal, 1);

    //std::string user01 = "user01";
    //int renderIdLocal = YouMeTalk::getInstance()->bindTexture(user01, 160, 120, rtLocal);

    //  远程 yuv 测试
//    RenderTexture * rtRemote = RenderTexture::create(160, 120, Texture2D::PixelFormat::RGB888);
//    rtRemote->setPosition(origin.x + visibleSize.width - 352 , origin.y + 10);
//    rtRemote->setRotation(270);
//
//    this->addChild(rtRemote, 1);

    return true;
}

void AutoTestScene::joinroom()
{
    //YouMeErrorCode = IYouMeVoiceEngine::getInstance()->;
    IYouMeVoiceEngine::getInstance()->joinChannelSingleMode("AutoRoboter", TALK_CHAT_ROOM, YOUME_USER_HOST);
}

void AutoTestScene::initData()
{
    userRenders.clear();

    render_send = RenderTexture::create(120, 160, Texture2D::PixelFormat::RGB888);
    render_send->getSprite()->setFlippedY(!render_send->getSprite()->isFlippedY());
    render_send->setScale(0.5);
    render_send->setPosition(VisibleRect::center());

    render_recv = RenderTexture::create(120, 160, Texture2D::PixelFormat::RGB888);
    render_recv->setScale(0.5);
    render_recv->setPosition(VisibleRect::right());
    addChild(render_recv);

    render_recv->setScale(0.5);
    render_recv->setPosition(VisibleRect::right());
}

void AutoTestScene::startTalkTest()
{
    //_label_for_progress->setString("running talk autotest... ");
    talk_controller.startTest();
    //_label_for_progress->setString("Finished!");
}

void AutoTestScene::sendemail()
{
    talk_controller.sendemail();
}

void AutoTestScene::report()
{
    std::string filename = YoumeUtil::getReportFilename();
    std::string sdklogfile = YoumeUtil::getPkgFileName();
    cocos2d::log("Local file path: %s", filename.c_str());
    _label->setString(filename + "\n" + sdklogfile);
    //Application::getInstance()->openURL(filename);
}

