//
//  imcommontests.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "imcommontests.hpp"

IMCommonTests::IMCommonTests()
{
    ADD_TEST_CASE(UserCommonTest);
    ADD_TEST_CASE(MessageCommonTest);
    ADD_TEST_CASE(VoiceCommonTest);
}

void UserCommonTest::onEnter()
{
    TestCase::onEnter();
    //TestCore::createTestCoreInstance()->commonTestSuit()->case_user_common();
}

void MessageCommonTest::onEnter()
{
    TestCase::onEnter();
    //TestCore::createTestCoreInstance()->commonTestSuit()->case_msg_common();
}

void VoiceCommonTest::onEnter()
{
    TestCase::onEnter();
    //TestCore::createTestCoreInstance()->commonTestSuit()->case_audio_common();
}
