//
//  settings.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/20.
//
//

#include "settings.hpp"
#include <string>
#include "audio/include/SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

GlobalSetting* GlobalSetting::instance()
{
    static GlobalSetting inst;
    return &inst;
}

GlobalSetting::GlobalSetting()
{
    //init
    this->im_user = "IMXXOOO9999";
    this->talk_user = "TALKOOO9998";
    //default user settings
}
GlobalSetting::~GlobalSetting()
{

}

void GlobalSetting::setIMUser(const std::string &username)
{
    im_user = username;
}

void GlobalSetting::setTalkUser(const std::string &username)
{
    talk_user = username;
}

void setImUser();

void GlobalSetting::controlBackgroundMusic(const std::string & cmd)
{
    const char* musice_res = "mangetunoyoruniodore.mp3";
    if (cmd == "pause") {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    } else if (cmd == "start") {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musice_res);
    } else if (cmd == "resume") {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    } else if (cmd == "stop") {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();    }
}

TestSettings::TestSettings()
{
    ADD_TEST_CASE(UserSetting);
    ADD_TEST_CASE(MusicSetting);
}


UserSetting::UserSetting()
{
    auto btn_1 = RadioButton::create("radio_button_off.png",
                                     "radio_button_on.png");
    auto btn_2 = RadioButton::create("radio_button_off.png",
                                     "radio_button_on.png");
    auto btn_3 = RadioButton::create("radio_button_off.png",
                                     "radio_button_on.png");
    auto radioBtnGroup = RadioButtonGroup::create();


    btn_1->setColor(Color3B(155, 138, 200));
    btn_2->setColor(Color3B(155, 138, 200));
    btn_3->setColor(Color3B(155, 138, 200));

    btn_1->setPosition(Vec2(VisibleRect::center().x - 40, VisibleRect::center().y));
    btn_2->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y));
    btn_3->setPosition(Vec2(VisibleRect::center().x + 40, VisibleRect::center().y));

    //btn_1->addEventListener(const ccRadioButtonCallback &callback)
    // TODO

    radioBtnGroup->addRadioButton(btn_1);
    radioBtnGroup->addRadioButton(btn_2);
    radioBtnGroup->addRadioButton(btn_3);
    radioBtnGroup->setPosition(VisibleRect::center());
    addChild(radioBtnGroup);
    addChild(btn_1);
    addChild(btn_2);
    addChild(btn_3);
}

UserSetting::~UserSetting()
{
    
}


//Musice Setting
MusicSetting::MusicSetting()
{
    auto btn_play = Button::create("btn.png");
    auto btn_stop = Button::create("btn.png");

    btn_play->setPosition(VisibleRect::left());
    btn_stop->setPosition(VisibleRect::right());
    btn_play->setTitleText("Play");
    btn_stop->setTitleText("Stop");
    addChild(btn_play);
    addChild(btn_stop);

    btn_play->addTouchEventListener(CC_CALLBACK_1(MusicSetting::playMusic, this));
    btn_stop->addTouchEventListener(CC_CALLBACK_1(MusicSetting::stopMusic, this));

}

void MusicSetting::stopMusic(cocos2d::Ref *sender)
{
    GlobalSetting::instance()->controlBackgroundMusic("stop");
}

void MusicSetting::playMusic(cocos2d::Ref *sender)
{
    GlobalSetting::instance()->controlBackgroundMusic("start");
}

MusicSetting::~MusicSetting()
{
}





