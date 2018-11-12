//
//  talkchanneltests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//
#include "talkchanneltests.hpp"
#include "imservice.hpp"

USING_NS_CC;

TalkChannelTests::TalkChannelTests()
{
    ADD_TEST_CASE(TalkChannelTest);
    ADD_TEST_CASE(JoinChannelTest);
    ADD_TEST_CASE(LeaveChannelTest);
    ADD_TEST_CASE(JoinChannelMultiModeTest);
    ADD_TEST_CASE(LeaveChannelMultiModeTest);
    ADD_TEST_CASE(ChooseAChannelToSpeak);
    ADD_TEST_CASE(JoinSingleChannelExceptTest);
    ADD_TEST_CASE(JoinMutiChannelExceptTest);
    ADD_TEST_CASE(LeaveChannelExceptTest);
    ADD_TEST_CASE(LeaveChannelAllExceptTest);
    ADD_TEST_CASE(JoinSingleChannelExceptTest2);
}

TalkChannelTest::TalkChannelTest()
{
    //auto label = Label::createWithTTF("Talk Test", "fonts/arial.ttf", 28);
    //addChild(label, 0);
    //label->setPosition(VisibleRect::center().x, VisibleRect::top().y-50);

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(TalkChannelTest::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // create a label to display the tip string
    _label = Label::createWithTTF("Touch the screen to connect", "fonts/arial.ttf", 22);
    _label->setPosition(VisibleRect::center());
    addChild(_label, 0);

    _label->retain();
}

TalkChannelTest::~TalkChannelTest()
{
}

void TalkChannelTest::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
    _label->setString("Talk Test Begin");
}


JoinChannelTest::JoinChannelTest()
{
}

JoinChannelTest::~JoinChannelTest()
{
}

void JoinChannelTest::onEnter()
{
    // Normal case 用户加入对讲频道 听众模式
    TalkChannelTestBase::onEnter();

    IYouMeVoiceEngine* engine = TalkService::getInstance()->getTalkEngine();
    //engine->leaveChannelAll();
    sleep(5);
    YouMeErrorCode errcode1 = engine->joinChannelSingleMode(TALK_USER_A, "testroom001",
                                                            YOUME_USER_TALKER_FREE);
    CHECK_RETURN_CODE(errcode1, "Join channel single mode success", "Join channel single mode  failed");

    //engine->leaveChannelAll();
    YouMeErrorCode errcode2 = engine->joinChannelSingleMode(TALK_USER_A, "testroom001",
                                                            YOUME_USER_LISTENER);
    //engine->leaveChannelAll();
    YouMeErrorCode errcode3 = engine->joinChannelSingleMode(TALK_USER_A, "testroom001",
                                                            YOUME_USER_TALKER_ON_DEMAND);
    //engine->leaveChannelAll();
	YouMeErrorCode errcode4 = engine->joinChannelSingleMode(TALK_USER_A, "testroom001",
                                                        YOUME_USER_COMMANDER);
    //engine->leaveChannelAll();
	YouMeErrorCode errcode5 = engine->joinChannelSingleMode(TALK_USER_A, "testroom001",
                                                        YOUME_USER_HOST);
    //engine->leaveChannelAll();
	YouMeErrorCode errcode6 = engine->joinChannelSingleMode(TALK_USER_A, "testroom001",
                                                            YOUME_USER_GUSET);

    CHECK_RETURN_CODE(errcode2, "Join channel single mode success", "Join channel single mode  failed");
    CHECK_RETURN_CODE(errcode3, "Join channel single mode success", "Join channel single mode  failed");
    CHECK_RETURN_CODE(errcode4, "Join channel single mode success", "Join channel single mode  failed");
    CHECK_RETURN_CODE(errcode5, "Join channel single mode success", "Join channel single mode  failed");
    CHECK_RETURN_CODE(errcode6, "Join channel single mode success", "Join channel single mode  failed");
    //engine->leaveChannelAll();
    //sleep(5);
}

void LeaveChannelTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    //单频道
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);
    sleep(5);
    //engine->joinChannelSingleMode(TALK_USER_A, "roomsingle1001", YOUME_USER_TALKER_FREE);
    YouMeErrorCode errcode1 = engine->leaveChannelAll();

    //多频道
    //engine->joinChannelMultiMode(TALK_USER_A, "room1001");
    //engine->joinChannelMultiMode(TALK_USER_A, "room1002");
    //YouMeErrorCode errcode2 = engine->leaveChannelAll();

    CHECK_RETURN_CODE(errcode1, "leave all channel success", "leave all channel failed");
    //CHECK_RETURN_CODE(errcode2, "leave all channel success", "leave all channel failed");
}

void JoinChannelMultiModeTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);
    sleep(5);

    YouMeErrorCode errcode1 = engine->joinChannelMultiMode(TALK_USER_B, "MutiRoom001", YOUME_USER_HOST);
    CHECK_RETURN_CODE(errcode1, "joinChannelMultiMode success", "joinChannelMultiMode failed");

    YouMeErrorCode errcode2 = engine->joinChannelMultiMode(TALK_USER_B, "TalkMultiRoom001", YOUME_USER_HOST);
    YouMeErrorCode errcode3 = engine->joinChannelMultiMode(TALK_USER_B, "TalkMultiRoom002", YOUME_USER_HOST);

    CHECK_RETURN_CODE(errcode2, "joinChannelMultiMode success", "joinChannelMultiMode failed");
    CHECK_RETURN_CODE(errcode3, "joinChannelMultiMode success", "joinChannelMultiMode failed");

    //engine->leaveChannelAll();

}

void LeaveChannelMultiModeTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);

    engine->joinChannelMultiMode(TALK_USER_A, "testMulti002", YOUME_USER_TALKER_FREE);
	engine->joinChannelMultiMode(TALK_USER_A, "testMulti004", YOUME_USER_TALKER_FREE);

    YouMeErrorCode errcode1 = engine->leaveChannelMultiMode("test001");
    CHECK_RETURN_CODE(errcode1, "leaveChannelMultiMode success", "leaveChannelMultiMode failed");
}

void ChooseAChannelToSpeak::onEnter()
{
    TalkChannelTestBase::onEnter();
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);
    sleep(5);
    engine->joinChannelMultiMode(TALK_USER_B, "talkMutiroom002", YOUME_USER_TALKER_FREE);
	engine->joinChannelMultiMode(TALK_USER_B, "talkMutiroom003", YOUME_USER_TALKER_FREE);

    YouMeErrorCode errcode1 = engine->speakToChannel("talkMutiroom003");
    CHECK_RETURN_CODE(errcode1, "speakToChannel Ok", "speakToChannel Failed");
}


//Except test
JoinSingleChannelExceptTest::JoinSingleChannelExceptTest()
{
}

void JoinSingleChannelExceptTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    cocos2d::log("Test join single room exception");
    const int NUM = 1024;
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine* engine = TalkService::getInstance()->getTalkEngine();

    sleep(5);
    //重复加入
    engine->joinChannelSingleMode(TALK_USER_A, TALK_CHAT_ROOM, YOUME_USER_GUSET);
    engine->joinChannelSingleMode(TALK_USER_A, TALK_CHAT_ROOM, YOUME_USER_GUSET);

    //加入不同的房间
    engine->joinChannelSingleMode(TALK_USER_B, "No1Room1", YOUME_USER_TALKER_FREE);
    engine->joinChannelSingleMode(TALK_USER_B, "No2Room2", YOUME_USER_TALKER_FREE);

    //分别以不同的身份加入同一个房间
    engine->joinChannelSingleMode(TALK_USER_A, "No3Room3", YOUME_USER_HOST);
    engine->joinChannelSingleMode(TALK_USER_B, "No3Room3", YOUME_USER_COMMANDER);

    //异常参数输入 空指针
    engine->joinChannelSingleMode(nullptr, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    engine->joinChannelSingleMode(TALK_USER_B, nullptr, YOUME_USER_TALKER_FREE);
    engine->joinChannelSingleMode(nullptr, nullptr, YOUME_USER_HOST);
    //异常参数输入 长字符
    char longtext[NUM] = {'a'}; //规格未定

    engine->joinChannelSingleMode(longtext, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);

    //中文ID
    engine->joinChannelSingleMode("饕餮", TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    engine->joinChannelSingleMode(TALK_USER_A, "高雅的艺术间", YOUME_USER_COMMANDER);
}

void LeaveChannelExceptTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);

    engine->joinChannelMultiMode(TALK_USER_A, "test001", YOUME_USER_TALKER_FREE);
	engine->joinChannelMultiMode(TALK_USER_A, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
	engine->joinChannelMultiMode(TALK_USER_A, "Hahatest004", YOUME_USER_TALKER_FREE);

    //退出别人加入的频道
    YouMeErrorCode errcode1 = engine->leaveChannelMultiMode("Hahatest004");
    cocos2d::log("Return code is %d", errcode1);

    //退出未知的频道
    YouMeErrorCode errcode2 = engine->leaveChannelMultiMode("@#$%^&*");
    cocos2d::log("Return code is %d", errcode2);

    //空指针传入
    engine->leaveChannelMultiMode(nullptr);

    //大字符串传入
    char bigStr[1024] = {'x'};
    YouMeErrorCode errcode3 = engine->leaveChannelMultiMode(bigStr);
    cocos2d::log("Return code is %d", errcode3);
}

void JoinMutiChannelExceptTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    //空指针传入
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);

    engine->joinChannelMultiMode(nullptr, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    engine->joinChannelMultiMode(TALK_USER_A, nullptr, YOUME_USER_TALKER_FREE);
    engine->joinChannelMultiMode(0, 0, YOUME_USER_TALKER_FREE);

    //long text长字符串
    char logntext[1024];
    for (int i = 0; i < 1024; ++i) {
        logntext[i] = 'x';
    }
    engine->joinChannelMultiMode(logntext, TALK_CHAT_ROOM, YOUME_USER_TALKER_FREE);
    engine->joinChannelMultiMode(TALK_USER_A, logntext, YOUME_USER_TALKER_FREE);

    //房间和用户名字一样
    engine->joinChannelMultiMode("test", "test", YOUME_USER_TALKER_FREE);//允许
}

void LeaveChannelAllExceptTest::onEnter()
{
    TalkChannelTestBase::onEnter();
    //多频道退出 1024个
    IYouMeVoiceEngine::getInstance()->unInit();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    assert(engine != nullptr);

    const int channel_num = 1024;
    char * channel[channel_num];
    char buffer[64];
    for (int i = 0; i < channel_num; ++i) {
        sprintf(buffer, "roommulti%d", 1000 + 1);
        channel[i] = buffer;
    }

    for (int k = 0; k < channel_num; ++k) {
        engine->joinChannelMultiMode(TALK_USER_B, channel[k], YOUME_USER_TALKER_FREE);
    }
   // YouMeErrorCode errcode1 = engine->leaveChannelAll();
   // CHECK_RETURN_CODE(errcode1, "Leave channel success", "leave channel All failed.");

}

void JoinSingleChannelExceptTest2::onEnter()
{
    TalkChannelTestBase::onEnter();

}

void JoinSingleChannelExceptTest2::onExit()
{
    TalkChannelTestBase::onExit();
}

JoinSingleChannelExceptTest2::JoinSingleChannelExceptTest2()
{
    _stop = false;
    _success_num = 0;
    _failed_num = 0;

    _label_faled = Label::createWithTTF("fails:", "fonts/arial.ttf", 8.0);
    _label_success = Label::createWithTTF("success:", "fonts/arial.ttf", 8.0);
    _label_info = Label::createWithTTF("info", "fonts/arial.ttf", 8.0);
    _label_success->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y));
    _label_faled->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y - 20));
    _label_info->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y - 40));
    _label_info->setString("Click Button to start test; Join 100 members in a room on single room");
    _btn_begin = ui::Button::create();
    _btn_begin->setTitleText("Begin");
    _btn_begin->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y - 80));
    _btn_begin->addTouchEventListener(CC_CALLBACK_1(JoinSingleChannelExceptTest2::startCallback, this));

    addChild(_label_success);
    addChild(_label_faled);
    addChild(_label_info);
    addChild(_btn_begin);
}

void JoinSingleChannelExceptTest2::startCallback(cocos2d::Ref *node)
{
    _btn_begin->setEnabled(false);
    _label_info->setString("Running");
    IYouMeVoiceEngine::getInstance()->unInit();
    YouMeErrorCode errcode = IYouMeVoiceEngine::getInstance()->init(this, TALK_APPKEY, TALK_APPSECRET, RTC_CN_SERVER, "");
    CHECK_RETURN_CODE(errcode, "Init success", "Init failed");
}

JoinSingleChannelExceptTest2::~JoinSingleChannelExceptTest2()
{
}

void JoinSingleChannelExceptTest2::onEvent(const YouMeEvent event, const YouMeErrorCode error,
                               const char* channel, const char *param)
{
    if (event == YOUME_EVENT_INIT_OK) {
        exceute();
        //处理完成以后退出所有用户
    } else if (event == YOUME_EVENT_INIT_FAILED) {
        cocos2d::log("Init no ok ! Please check");
    } else if (event == YOUME_EVENT_JOIN_OK) {
        ++_success_num;
    } else if (event == YOUME_EVENT_JOIN_FAILED) {
        ++_failed_num;
    } else if (event == YOUME_EVENT_LEAVED_ALL) {
        
    }

    char labelstring[32];
    sprintf(labelstring, "success: %d", _success_num);

    _label_success->setString(labelstring);
    sprintf(labelstring, "failed: %d", _failed_num);
    _label_faled->setString(labelstring);
    if (_failed_num + _success_num >= 100) {
        IYouMeVoiceEngine::getInstance()->leaveChannelAll();
        _btn_begin->setEnabled(true);
        _label_info->setString("Done!");
    }
}

void JoinSingleChannelExceptTest2::exceute()
{
    char userbuff[32];
    char roomid[] = "singleroommode";
    sprintf(userbuff, "Perfuser%s", "123");
    const int num = 100;
    IYouMeVoiceEngine *engine = IYouMeVoiceEngine::getInstance();
    CCASSERT(engine != nullptr, "");
    _stop = false;
    std::thread t([&](){
        for (int i = 0; i < num; ++i) {
            if (_stop)
                break;
            sprintf(userbuff, "perfuser_%d", i);
            YouMeErrorCode code =
            engine->joinChannelSingleMode(userbuff, roomid, YOUME_USER_TALKER_FREE);
            CHECK_RETURN_CODE(code, "success", "failed");
        }
    });
    t.join();
}
