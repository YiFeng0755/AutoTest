//
//  locationtest.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/18.
//
//

#ifndef locationtest_hpp
#define locationtest_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(LocationTests);

class LocationTestBase : public TestCase
{
public:
    //CREATE_FUNC(LocationTest);
    LocationTestBase();
    ~LocationTestBase();
    virtual std::string title() const override {
        return "Location Test";
    }
protected:
    //cocos2d::Label *_label;
};

class LocationNearByObject : public LocationTestBase
{
public:
    LocationNearByObject();
    ~LocationNearByObject();
    CREATE_FUNC(LocationNearByObject);
    virtual void onEnter() override;
    virtual std::string subtitle() const override
    {
        return "Get Location Nearby Objects";
    }
private:

};

class CurrentLocation : public LocationTestBase
{
public:
    CREATE_FUNC(CurrentLocation);
    virtual void onEnter() override;
    virtual std::string subtitle() const override
    {
        return "Get CurrentLocation";
    }
};

class UpdateInterval : public LocationTestBase
{
public:
    CREATE_FUNC(UpdateInterval);
    virtual void onEnter() override;
    virtual std::string subtitle() const override
    {
        return "Update location interval";
    }
};
#endif /* locationtest_hpp */
