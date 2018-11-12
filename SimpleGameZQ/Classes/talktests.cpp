//
//  talktests.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "talktests.hpp"
#include "talkchanneltests.hpp"
#include "devicesettingsuit.hpp"
#include "networksettingtests.hpp"
#include "talkmanagertests.hpp"
#include "talkinittest.hpp"

TalkTests::TalkTests()
{
    addTest("Talk Tests", [](){ return new (std::nothrow) TalkTestUiCases(); });
}

TalkTestUiCases::TalkTestUiCases()
{
    addTest("Talk Channel Test", [](){ return new (std::nothrow) TalkChannelTests(); });
    addTest("Talk Manager Test", [](){ return new (std::nothrow) TalkManagerTests(); });
    addTest("Device Manager Test", [](){ return new (std::nothrow) DeviceManageerTests(); });

    addTest("Network Setting Test", [](){ return new (std::nothrow) NetworkSettingTests(); });
    addTest("Init & Uninit Test", [](){ return new (std::nothrow) TalkInitTests(); });
   // addTest("Talk Channel Test", [](){ return new (std::nothrow) NetworkSettingTests(); });
}


