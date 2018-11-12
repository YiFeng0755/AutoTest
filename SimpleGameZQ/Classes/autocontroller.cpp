//
//  autocontroller.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//

#include "autocontroller.hpp"
#include "talkcases.hpp"
#include "cocos2d.h"
#include "callbackwrapper.hpp"
#include "imcases.hpp"
#include <time.h>
#include <stdio.h>

AutoTestController::AutoTestController()
{
    IMService::getInstance()->init();
    itor = _cases.begin();
    initfile();
}

AutoTestController::~AutoTestController()
{
    for (auto it = _cases.begin(); it != _cases.end(); ++it) {
        delete (*it);
        (*it) = nullptr;
    }
    _cases.clear();
}

AutoTestController *AutoTestController::instance()
{
    static AutoTestController inst;
    return &inst;
}

void AutoTestController::addTest(AutoTestCase *test)
{
    _cases.push_back(test);
}

void AutoTestController::next()
{
//    ++itor;
//    if (itor != _cases.end())
//        (*itor)->run();
//    else {
//        submitReport();
//    }
}

void AutoTestController::start()
{
    //static IMTestController controller;
    //controller.runTests();
    //static TalkCasesController talkcontroller;
    //talkcontroller.runTests();

}

void AutoTestController::previouse()
{
    --itor;
    if (itor != _cases.begin())
        (*itor)->run();
}

AutoTestCase::AutoTestCase()
{
    //_message = "Ok";
    _controller = AutoTestController::instance();
}

void AutoTestCase::writeResult()
{
    std::string path = XStringToLocal(app_log_path);
    char filename[1024]; //路径很长，留足够的空间来装文件名，否则crash
    //long timestap = time(0);
    sprintf(filename, "%s/report.csv", path.c_str());
    //char buffer[1024];
    //sprintf(buffer, "Result:Ok, output: %s", _message.c_str());
    //std::ofstream ofile;
    //ofile.open(filename, std::ios::app);
    FILE * fp = fopen(filename, "a+");
    if (!fp)
        return;
    fwrite(_message.c_str(), _message.length(), 1, fp);
    fclose(fp);
    //ofile << _message;
    //ofile.close();
    //cocos2d::log("%s", cocos2d::FileUtils::getInstance()->fullPathForFilename("report.csv").c_str());
    _message.clear();
}

void AutoTestCase::run()
{
    
}
