//
//  imcommontests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#ifndef imcommontests_hpp
#define imcommontests_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(IMCommonTests);


class UserCommonTest : public TestCase
{
public:
    CREATE_FUNC(UserCommonTest);
    virtual std::string subtitle() const override {
        return "User Common Test";
    }
    virtual void onEnter() override;
};

class MessageCommonTest : public TestCase
{
public:
    CREATE_FUNC(MessageCommonTest);
    virtual std::string subtitle() const override {
        return "Message Common Test";
    }
    virtual void onEnter() override;
};

class VoiceCommonTest : public TestCase
{
public:
    CREATE_FUNC(VoiceCommonTest);
    virtual std::string subtitle() const override {
        return "Voice Common Test";
    }
    virtual void onEnter() override;
};
#endif /* imcommontests_hpp */
