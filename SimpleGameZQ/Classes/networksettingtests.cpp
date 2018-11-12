//
//  networksettingtests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#include "networksettingtests.hpp"
#include "talkservice.hpp"

NetworkSettingTests::NetworkSettingTests()
{
    ADD_TEST_CASE(SetUserMobileNetworkEnabledTest);
    ADD_TEST_CASE(GetUserMobileNetworkEnabledTest);
}

void SetUserMobileNetworkEnabledTest::onEnter()
{
    TestCase::onEnter();
    IYouMeVoiceEngine * engine = TalkService::getInstance()->getTalkEngine();
    CC_ASSERT(engine != nullptr);
    engine->setUseMobileNetworkEnabled(true);

    engine->setUseMobileNetworkEnabled(false);
}

void GetUserMobileNetworkEnabledTest::onEnter()
{
    TestCase::onEnter();
    IYouMeVoiceEngine *engine = TalkService::getInstance()->getTalkEngine();
    bool b = engine->getUseMobileNetworkEnabled();
    cocos2d::log("Result is %d", b);
}
