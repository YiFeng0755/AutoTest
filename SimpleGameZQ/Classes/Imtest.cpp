//
//  Imtest.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "Imtest.hpp"
#include "imtestcases.hpp"

ImTests::ImTests()
{
    addTest("Im Testcases", []() { return new (std::nothrow) IMTestUiCases();});
}

ImTests::~ImTests()
{
}
