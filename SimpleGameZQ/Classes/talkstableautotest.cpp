//
//  talkstableautotest.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/9/2.
//
//

#include "talkstableautotest.hpp"


TalkAecStableDemoTests::TalkAecStableDemoTests()
{
    ADD_TEST_CASE(Case1Test);
}

Case1Test::Case1Test()
{
    
}

Case1Test::~Case1Test()
{

}

bool Case1Test::init()
{

}

void Case1Test::onEvent(const YouMeEvent event, const YouMeErrorCode error, const char *channel, const char *param)
{

}
