//
//  AutoRandTest.hpp
//  AutoCaller
//
//  Created by pinky on 2017/7/5.
//
//

#ifndef AutoRandTest_hpp
#define AutoRandTest_hpp

#include <stdio.h>
#include <thread>
#include <vector>
#include <string>

using namespace std;
#include <mutex>
#include <condition_variable>

class FuncRandTest{
public:
    //启动N个线程进行随机测试
    void StartTest( int count );
    
    void addFunc( const std::function<void(void)>& func );
    
    void RandTestThreadFunc( const std::vector< std::function<void(void)>>&  vecFuncs );
    
    //表示准备工作已完成，测试线程可以开始执行
    void setReady();
    
    //返回是否需要等待prepare完成，如果返回true,则在判断prepare完成的时候，需要手动调用setReady
    virtual bool prepare();
    
    int timeGapMin = 100;
    int timeGapMax = 10000;
private:
    std::vector< std::function<void(void)>> m_vecFuncs;
    std::condition_variable m_cv;
    std::mutex mtx;
};

class IMAutoRandTest: public FuncRandTest {
public:
    IMAutoRandTest();
    void registAllTest();
    
    void registLocationTest();
    
    void registMessageTest();
    
    void registYIMTest();
    
    virtual bool prepare();
    
public:
    //测试要用到的一些数据
    mutex m_paramMutex;
    string  m_strRecvPath;
    string m_strAmrRecvPath;
    
    string m_url;
    XUINT64 m_recvMessageID;
    
    string m_role;
    string m_roleto;
    string m_room;
};

class TalkAutoRandTest: public FuncRandTest{
public:
    void registAllTest();
    
    string m_role;
    string m_roleto;
    string m_room;
};

#endif /* AutoRandTest_hpp */
