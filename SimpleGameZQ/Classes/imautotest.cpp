//
//  imautotest.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#include "imautotest.hpp"
#include "autocontroller.hpp"
#include "imcases.hpp"


USING_NS_CC;
using namespace ui;

IMAutoTests::IMAutoTests()
{
    ADD_TEST_CASE(IMAutoTest);
}

IMAutoTest::IMAutoTest()
{
    btn_start =  Button::create();
    btn_start->setPosition(VisibleRect::center());
    btn_start->setTitleText("Start");
    btn_start->addClickEventListener(CC_CALLBACK_1(IMAutoTest::start, this));
    addChild(btn_start);
}

void IMAutoTest::start(cocos2d::Ref *pSender)
{
    AutoTestController::instance()->start();
}
