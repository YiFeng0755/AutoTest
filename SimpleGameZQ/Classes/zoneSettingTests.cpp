//
//  ZoneSettingTests.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/19.
//
//

#include "zoneSettingTests.hpp"

ZoneTests::ZoneTests()
{
    ADD_TEST_CASE(SetRecordingTimeMsTest);
    ADD_TEST_CASE(SetPlayingTimeMsTest);
    ADD_TEST_CASE(ServerZoneSetting);
}

