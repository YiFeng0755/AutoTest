//
//  ZoneSettingTests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/19.
//
//

#ifndef zoneSettingTests_hpp
#define zoneSettingTests_hpp

#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(ZoneTests);

//设置录音时间戳
class SetRecordingTimeMsTest : public TestCase
{
public:
    //SetRecordingTimeTest();
    CREATE_FUNC(SetRecordingTimeMsTest);
    virtual std::string subtitle() const override {
        return "Set Recording Time Ms Test";
    }
};

class SetPlayingTimeMsTest : public TestCase
{
public:
    //SetRecordingTimeTest();
    CREATE_FUNC(SetPlayingTimeMsTest);
    virtual std::string subtitle() const override {
        return "Set Playing Time Ms Test";
    }
};

class ServerZoneSetting : public TestCase
{
public:
    //SetRecordingTimeTest();
    CREATE_FUNC(ServerZoneSetting);
    virtual std::string subtitle() const override {
        return "Server Zone Setting";
    }
};
#endif /* ZoneSettingTests_hpp */
