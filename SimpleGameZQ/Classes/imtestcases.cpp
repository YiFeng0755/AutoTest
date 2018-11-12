//
//  imtestcases.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "imtestcases.hpp"

//#include "joinroom/channeltest.hpp"
#include "channeltest.hpp"
#include "sendmsgtests.hpp"
#include "sendvoicetests.hpp"
#include "imcommontests.hpp"
#include "locationtest.hpp"
#include "autosendrecvmsg.hpp"

IMTestUiCases::IMTestUiCases()
{
    addTest("Channel Manager", [](){return new (std::nothrow) ChannelTests(); });
    addTest("Send Message Test", [](){ return new (std::nothrow) SendMsgTests(); });
    addTest("Send Voice Test", [](){ return new (std::nothrow) SendVoiceTests(); });
    addTest("Common Setting Test", [](){ return new (std::nothrow) IMCommonTests(); });
    addTest("Location Test", [](){ return new (std::nothrow) LocationTests();});
    addTest("Auto Send Recv Msg Test", [](){ return new (std::nothrow) SendMsgDemoTests();});
}
