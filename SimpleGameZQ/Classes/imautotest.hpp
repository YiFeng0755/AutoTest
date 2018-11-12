//
//  imautotest.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#ifndef imautotest_hpp
#define imautotest_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(IMAutoTests);

class IMAutoTest : public TestCase
{
public:
    CREATE_FUNC(IMAutoTest);
    IMAutoTest();

    void start(cocos2d::Ref * pSender);
private:
    cocos2d::ui::Button *btn_start;
};
#endif /* imautotest_hpp */
