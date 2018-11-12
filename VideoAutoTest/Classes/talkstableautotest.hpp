//
//  talkstableautotest.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/9/2.
//
//

#ifndef talkstableautotest_hpp
#define talkstableautotest_hpp

#include "testbase/BaseTest.h"
#include "cocos2d.h"
#include "IYouMeVoiceEngine.h"
//#include "IYouMeVideoCallback.h"
#include "IYouMeEventCallback.h"
#include <atomic>


DEFINE_TEST_SUITE(TalkAecStableDemoTests);


class Case1Test : public TestCase, public IYouMeEventCallback
{
public:
    Case1Test();
    ~Case1Test();
    virtual bool init() override;
    void onEvent(const YouMeEvent event, const YouMeErrorCode error, const char * channel, const char * param) override;

    CREATE_FUNC(Case1Test);
    virtual std::string title() const override {
        return "Aec Demo Test: ";
    }
private:
    cocos2d::Label * ver_label;
};
#endif /* talkstableautotest_hpp */
