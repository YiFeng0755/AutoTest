//
//  settings.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/20.
//
//

#ifndef settings_hpp
#define settings_hpp

#include "cocos2d.h"
#include "testbase/BaseTest.h"

DEFINE_TEST_SUITE(TestSettings);


class GlobalSetting
{
public:
    static GlobalSetting* instance();
    ~GlobalSetting();
    void controlBackgroundMusic(const std::string& cmd);
    void setIMUser(const std::string& username);
    std::string getIMUser() const {
        return im_user;
    }

    void setTalkUser(const std::string & username);
    std::string getTalkuser() const {
        return talk_user;
    }
private:
    GlobalSetting();

    std::string im_user;
    std::string talk_user;
};

class UserSetting : public TestCase
{
public:
    CREATE_FUNC(UserSetting);
    UserSetting();
    ~UserSetting();
private:
    std::string im_user[3];
    std::string talk_user[3];
};

class MusicSetting : public TestCase
{
public:
    CREATE_FUNC(MusicSetting);
    MusicSetting();
    ~MusicSetting();
    void stopMusic(cocos2d::Ref *sender);
    void playMusic(cocos2d::Ref *sender);
private:
   
};

#endif /* settings_hpp */
