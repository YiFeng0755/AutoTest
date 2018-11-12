//
//  PerformanceMonitor.cpp
//  AutoCaller
//
//  Created by pinky on 2017/7/5.
//
//

#include "PerformanceMonitor.hpp"
#include "unistd.h"
#include <YouMeCommon/get_sys_info.h>
#include <thread>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <YouMeCommon/CrossPlatformDefine/IOS/YouMeApplication_iOS.h>
#endif


std::string getLogPath(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "/sdcard/";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    YouMeApplication_iOS  ios;
    
    return ios.getCachePath();
#endif
}

using namespace std;

PerformanceMonitor::~PerformanceMonitor(){
    if( m_file.IsOpen() ){
        m_file.Close();
    }
}

void PerformanceMonitor::openFile( std::string strPath ){
    m_file.LoadFile( strPath, CXFile::Mode_CREATE_ALWAYS );
    string head = "time,core,cpu,mem\n";
    if( m_file.IsOpen() ){
        m_file.Write( (const byte*)head.c_str(),  head.length()   );
    }
}


void PerformanceMonitor::startCheck(){
    m_path =  CXFile::CombinePath(getLogPath(), "log.txt" );
    
    openFile( m_path );
    
    std::thread tmp = std::thread( &PerformanceMonitor::checkThreadFunc, this );
    tmp.detach();
}

void PerformanceMonitor::checkThreadFunc()
{
    do{
        checkOnce();
        
        sleep( m_timeGap );
    
    }while(true);
}

void PerformanceMonitor::checkOnce(){
    unsigned int curTime = time(0);
    int nCore = youmecommon::getCountOfCores();
    float cpu = youmecommon::getCurrentProcessCPUUsed();
    float mem = youmecommon::getCurrentProcessMemoryUsed();
    
    std::stringstream ss;
    ss<<curTime<<","<<nCore<<","<<cpu<<","<<mem<<std::endl;
    
    
    string s = ss.str();
    
    m_fileMutex.lock();
    if( m_file.IsOpen() ){
        m_file.Write( (const byte*)s.c_str(),  s.length() );
    }
    
    m_fileMutex.unlock();
    
}
