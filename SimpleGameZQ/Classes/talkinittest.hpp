//
//  talkinittest.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/25.
//
//

#ifndef talkinittest_hpp
#define talkinittest_hpp

#include "testbase/BaseTest.h"
#include "cocos2d.h"
#include "IYouMeVoiceEngine.h"
#include <atomic>
#include <condition_variable>
#include <mutex>


DEFINE_TEST_SUITE(TalkInitTests);

/**
 这里测试初始化和反初始化的稳定性demo
 */


/**
 初始化测试
 */
class InitTest : public TestCase, public IYouMeEventCallback
{
public:
    InitTest();
    ~InitTest();
    virtual bool init() override;
    virtual void onEvent(const YouMeEvent event, const YouMeErrorCode error,
                         const char * channel, const char * param) override;

    CREATE_FUNC(InitTest);
    void start();
private:
    cocos2d::ui::Button *_btn_test;
    cocos2d::Label * _label;
};


/**
 不断地初始化反初始化，测试其稳定性
 */
class UnInitTest: public TestCase, public IYouMeEventCallback
{
public:
    UnInitTest();
    virtual void onEvent(const YouMeEvent event, const YouMeErrorCode error,
                         const char * channel, const char * param) override;
    CREATE_FUNC(UnInitTest);
    void case1_test(); //不断调用uninit
    void case2_test(); //不断掉init和uninit 交替进行

    void case1_stop();
    void case2_stop();
private:
    cocos2d::ui::Button *btn_case1_test;
    cocos2d::ui::Button *btn_case2_test;
    cocos2d::ui::Button *btn_case1_stop;
    cocos2d::ui::Button *btn_case2_stop;

    cocos2d::Label * _label;
    bool m_case1_stop;
    bool m_case2_stop;

    std::atomic_int _success_stat;
    std::atomic_int _failed_stat;
};

class JoinLeavRoomTest : public TestCase, public IYouMeEventCallback
{
public:
    JoinLeavRoomTest();
    virtual void onEvent(const YouMeEvent event, const YouMeErrorCode error,
                         const char * channel, const char * param) override;
    std::string subtitle() const override {
        return "Join | Leave room";
    }
    void start();
    void stop();
    CREATE_FUNC(JoinLeavRoomTest);
    virtual bool init() override;
private:
    cocos2d::ui::Button *btn_start;
    cocos2d::ui::Button *btn_stop;
  //  cocos2d::ui::Button *btn_case1_stop;
   // cocos2d::ui::Button *btn_case2_stop;

    cocos2d::Label * _label;
    bool m_case1_stop;

    std::atomic_int _success_stat;
    std::atomic_int _failed_stat;

    std::condition_variable cv;
    std::mutex m_mutex;
};
#endif /* talkinittest_hpp */
