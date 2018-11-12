//
//  PerformanceMonitor.hpp
//  AutoCaller
//
//  Created by pinky on 2017/7/5.
//
//

#ifndef PerformanceMonitor_hpp
#define PerformanceMonitor_hpp

#include <stdio.h>
#include <YouMeCommon/XFile.h>
#include <mutex>
using namespace youmecommon;

class PerformanceMonitor{
public:
    void startCheck();
    
    void checkOnce();
    void checkThreadFunc();
    
    ~PerformanceMonitor();
    
    void openFile( std::string strPath );
    
    
    int m_timeGap = 1 ;
    CXFile m_file;
    std::string m_path;
    
    std::mutex  m_fileMutex;
    
};

#endif /* PerformanceMonitor_hpp */
