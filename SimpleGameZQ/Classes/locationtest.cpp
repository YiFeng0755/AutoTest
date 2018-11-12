//
//  locationtest.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#include "locationtest.hpp"

LocationTests::LocationTests()
{
    ADD_TEST_CASE(LocationNearByObject);
    ADD_TEST_CASE(CurrentLocation);
    ADD_TEST_CASE(UpdateInterval);
}

LocationTestBase::LocationTestBase()
{

}

LocationTestBase::~LocationTestBase()
{
    
}

LocationNearByObject::LocationNearByObject()
{

}

LocationNearByObject::~LocationNearByObject()
{

}

void LocationNearByObject::onEnter()
{
    LocationTestBase::onEnter();
    //TestCore::createTestCoreInstance()->locationTestSuit()->case_nearby_objects();
}

void CurrentLocation::onEnter()
{
    LocationTestBase::onEnter();
    //TestCore::createTestCoreInstance()->locationTestSuit()->case_current_location();
}

void UpdateInterval::onEnter()
{
    LocationTestBase::onEnter();
    //TestCore::createTestCoreInstance()->locationTestSuit()->case_update_interval();
}
