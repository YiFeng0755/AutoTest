//
//  sendmsgtests.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#ifndef sendmsgtests_hpp
#define sendmsgtests_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(SendMsgTests);

class SendTextMsgTest : public TestCase
{
public:
    virtual bool init() override ;
    CREATE_FUNC(SendTextMsgTest);
    virtual std::string subtitle() const override {
        return "Send Text Message Test";
    }
    void sendTextMsg();
    virtual void onEnter() override;
private:
    
};

class SendCuostomMsgTest : public TestCase
{
public:
    virtual bool init() override;
    CREATE_FUNC(SendCuostomMsgTest);
    virtual std::string subtitle() const override {
        return "Send Custom Message Test";
    }
    virtual void onEnter() override;
    void sendCustomMsg();
};

class SendGiftMsgTest : public TestCase
{
public:
    virtual bool init() override;
    CREATE_FUNC(SendGiftMsgTest);
    virtual std::string subtitle() const override {
        return "Send Gift Message Test";
    }
    virtual void onEnter() override;
    void sendGiftMsg();
};

class SendMessageTestsNoCrash : public TestCase
{
public:
    CREATE_FUNC(SendMessageTestsNoCrash);
    virtual bool init() override;
    virtual std::string subtitle() const override {
        return "Should not be crash";
    }
    virtual void onEnter() override;
private:
    cocos2d::Label *_label;
};


#endif /* sendmsgtests_hpp */
