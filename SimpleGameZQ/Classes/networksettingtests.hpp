//
//  networksettingtests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#ifndef networksettingtests_hpp
#define networksettingtests_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(NetworkSettingTests);

class SetUserMobileNetworkEnabledTest : public TestCase
{
public:
    CREATE_FUNC(SetUserMobileNetworkEnabledTest);
    virtual std::string title() const override{
        return "Networking seting test";
    }
    virtual std::string subtitle() const override {
        return "Set User Mobile Network Enabled Test";
    }

    virtual void onEnter() override;
};

class GetUserMobileNetworkEnabledTest : public TestCase
{
public:
    CREATE_FUNC(GetUserMobileNetworkEnabledTest);
    virtual std::string title() const override {
        return "Networking seting test";
    }
    virtual std::string subtitle() const override {
        return "Get User Mobile Network Enabled Test";
    }
    virtual void onEnter() override;
};
#endif /* commonsettingsuit_hpp */
