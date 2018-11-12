//
//  autocontroller.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/26.
//
//
#ifndef autocontroller_hpp
#define autocontroller_hpp
#include <list>
#include <vector>
#include <fstream>
#include "imservice.hpp"
#include <iostream>
#include <algorithm>
#include <regex>
#include "cocos2d.h"

typedef std::vector<std::string> ReportType;
static ReportType results;
static int CaseNum = 1;
static std::string filename() {
    std::string path = XStringToLocal(app_log_path);
	
    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
        cocos2d::FileUtils::getInstance()->createDirectory(path);
    }
    char filename[1024]; //路径很长，留足够的空间来装文件名，否则crash
    //long timestap = time(0);
	sprintf(filename, "%s/report_ex.html", path.c_str());
	std::string fullname = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
	return fullname;
}

//获得设备类型

static std::string platformName() {
    cocos2d::Application::Platform platform =
    cocos2d::Application::getInstance()->getTargetPlatform();

    /**< Windows */
    switch (platform) {
        case cocos2d::ApplicationProtocol::Platform::OS_IPHONE:
            return std::string("iPhone");
        case cocos2d::ApplicationProtocol::Platform::OS_IPAD:
            return std::string("iPad");
        case cocos2d::ApplicationProtocol::Platform::OS_ANDROID:
            return std::string("Android");
        case cocos2d::ApplicationProtocol::Platform::OS_BLACKBERRY:
            return std::string("BlackBerry");
        case cocos2d::ApplicationProtocol::Platform::OS_MAC:
            return std::string("Mac OS X");
        case cocos2d::ApplicationProtocol::Platform::OS_TIZEN:
            return std::string("Tizen");
        case cocos2d::ApplicationProtocol::Platform::OS_WINRT:
            return std::string("Windows Runtime Applications");
        case cocos2d::ApplicationProtocol::Platform::OS_LINUX:
            return std::string("Linux");
        case cocos2d::ApplicationProtocol::Platform::OS_WP8:
            return std::string("Windows Phone 8 Applications");
        case cocos2d::ApplicationProtocol::Platform::OS_NACL:
            return std::string("Native Client in Chrome");
        case cocos2d::ApplicationProtocol::Platform::OS_WINDOWS:
            return std::string("Windows");
        default:
            return std::string("UnKnown Device");
            break;
    }
    return "";
}

static std::string getcurrTime()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    struct timeval now;
    struct tm* time;

    gettimeofday(&now, NULL);


    time = localtime(&now.tv_sec);
    //int year = time->tm_year + 1900;

    char date[32] = {0};
    long ms =  now.tv_usec / 1000 ;
    sprintf(date, "%d/%02d/%02d %d:%d:%d.%ld",(int)time->tm_year + 1900,(int)time->tm_mon + 1,
            (int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min,
            (int)time->tm_sec, ms);

    return cocos2d::StringUtils::format("%s",date);

#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

    struct tm* tm;
    time_t timep;
    time(&timep);

    tm = localtime(&timep);
    char date[32] = {0};
    sprintf(date, "%d-%02d-%02d",(int)tm->tm_year + 1900,(int)tm->tm_mon + 1,(int)tm->tm_mday);
    //log("%s",date);
    return cocos2d::StringUtils::format("%s",date);

#endif
    return "";
}

static void clear_sdk_log_file()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || \
CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string logfilename = pkg_log_name + "/YouMeIMLogV2.txt";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string logfilename = app_log_path + "/YouMeIMLogV2.txt";
#endif
    std::fstream file(logfilename, std::ios::out);

    file.close();
}

static void initfile() {
    results.clear();
    CaseNum = 1;
    std::string fname = filename();
	//std::string logfilename = pkg_log_name + "/YouMeIMLogV2.txt";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || \
CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string logfilename = pkg_log_name + "/YouMeIMLogV2.txt";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string logfilename = app_log_path + "/YouMeIMLogV2.txt";
#endif
	std::ifstream infile(logfilename.c_str());

	char *buffer = new char[1024 * 1024];
	std::string cpuinfo = "";
	std::string sdkver = "";
	std::string brand = "";
	std::string model = "";
	std::string identifier = "";
	std::string appkey = "";
	std::string	osver = "";

	infile.read(buffer, 1024 * 1024 - 1);
	std::regex cupp("cpuarch.*?\n");
	std::regex sdkverp("sdkver.*?\n");
	std::regex modelp("model.*?\n");
	std::regex idp("identify.*?\n");
	std::regex brandp("brand.*?\n");
	std::regex appkeyp("appkey.*?\n");
	std::regex osverp("iosver.*?\n");

	std::cmatch cm;
	if (std::regex_search(buffer, cm, cupp))cpuinfo = std::string(cm[0]);
	if (std::regex_search(buffer, cm, modelp))model = std::string(cm[0]);
	if (std::regex_search(buffer, cm, sdkverp))sdkver = std::string(cm[0]);
	if (std::regex_search(buffer, cm, brandp))brand = std::string(cm[0]);
	if (std::regex_search(buffer, cm, idp))identifier = std::string(cm[0]);
	if (std::regex_search(buffer, cm, appkeyp))appkey = std::string(cm[0]);
	if (std::regex_search(buffer, cm, osverp))osver = std::string(cm[0]);
	
    std::ofstream outfile(fname.c_str(), std::ios::out);
    //cocos2d::Application::getInstance()->getTargetPlatform();


    outfile << "<!DOCTYPE html>" << std::endl;
    outfile << "<html lang=\"en\">" << std::endl;
    outfile << "<head>" << std::endl;
    outfile << "<meta charset=\"UTF-8\">"<< std::endl;
    outfile << "<title>测试报告</title>"<< std::endl;
    outfile << "<h1>Report</h1>" << std::endl;
    outfile << "<h4>" << getcurrTime() << "</h4>" << std::endl;
    outfile << "<h4>" << platformName() <<"</h4>" << std::endl; //
	outfile << "<h4>" << osver << "</h4>" << std::endl;
	outfile << "<h4>" << appkey << "</h4>" << std::endl;
	outfile << "<h4>" << identifier << "</h4>" << std::endl;
		outfile << "<h4>" << brand << "</h4>"<< std::endl;
		outfile << "<h4>" << sdkver << "</h4>"<< std::endl;
		outfile << "<h4>" << cpuinfo << "</h4>"<< std::endl;
		outfile << "<h4>" << model << "</h4>"<< std::endl; 
    outfile << "</head>" << std::endl;
    outfile << "<body>" << std::endl;
	
					
    outfile << "<table border=\"1.5\"" << std::endl;
    outfile << "<tr>";
    outfile << "<th> 序号 </th>" << std::endl;
    outfile << "<th> 接口名 </th>" << std::endl;
    outfile << "<th> 用例名 </th>" << std::endl;
    outfile << "<th> 测试结果 </th>" << std::endl;
    outfile << "<th> 备注 </th>" << std::endl;
    outfile << "</tr>" << std::endl;

    outfile.close();
	delete [] buffer;
}

static void endfile() {
    std::string fname = filename();

    std::ofstream outfile(fname.c_str(), std::ios::app);
    outfile << "</table>" << std::endl;
    outfile << "</body>" << std::endl;
    outfile.close();
}

static XString getRandUserName(int len = 6)
{
    XString result {__XT("ROBOT")};
    char src[]= {'a', 'b', 'c', 'd', 'e', 'f', 'g',
     'h','i','j','k','l','m','n','o','p','q','r','s','t',
        'u','v','w','x','y','z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	
    int l = (sizeof(src) / sizeof(char));
    for (int i = 0; i < len; ++i) {
		int num = cocos2d::random() % l;
        result.push_back(src[num]);
    }
    result.push_back('\0');
    return result;
}

static std::string getRandUserName_Talk(int len = 6)
{
    std::string result {"Robot"};
    char src[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h','i','j','k','l','m','n','o','p','q','r','s','t',
        'u','v','w','x','y','z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    int l = (sizeof(src) / sizeof(char));
    for (int i = 0; i < len; ++i) {
        int num = cocos2d::random() % l;
        result.push_back(src[num]);
    }
    result.push_back('\0');
    return result;
}

static void submitReport()
{

    std::string fname = filename();

    std::ofstream outfile(fname.c_str(), std::ios::app);

    for (auto it = results.begin(); it != results.end(); ++it) {
        outfile << (*it) << std::endl;
        std::cout << (*it) << std::endl;
    }
    outfile.close();
}

static std::mutex mtx;
static void write_a_message(const std::string & message)
{
    //std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
    //static std::mutex mtx;
    std::string fname = filename();
    mtx.lock();
    std::ofstream outfile(fname.c_str(), std::ios::app);
    //outfile << cocos2d::StringUtils::format("[%s] ", getcurrTime().c_str());
    outfile << message << std::endl;
    outfile.close();
    mtx.unlock();
    //cocos2d::log("Log message %s", message.c_str());
    //cocos2d::log("file path is %s", fname.c_str());
}

//需要锁住，做成thread-safe
template <class T>
static void EXPECT_EQ(const XString &casename, const XString & suitname,
                      T actual, T expect,
               const XString& comment=XString())
{
    //std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
	mtx.lock();
    std::string message;
    std::stringstream ss;
    ss << actual;
    std::string actual_str = ss.str();
    ss.str("");
    ss << expect;
    std::string expect_str = ss.str();
    std::string placehold(' ', 8);

//    if (casename.length() < 32) {
//        for (std::string::size_type i = 0; i < casename.size(); ++i){
//            placehold[i] = casename[i];
//        }
//    }

    std::string result;
    if (actual == expect) {
        result = "\t"
        "[        <font color=\"green\">OK</font>           ]";
    } else {
        result = cocos2d::StringUtils::format("\t"
        "[        <font color=\"red\">Failed</font>        ]"
        " ======= Expect is %s"
        ", But actual is %s", expect_str.c_str(), actual_str.c_str());
    }

    std::string row = cocos2d::StringUtils::format("<tr>"
                                                   "<td>%d</td>"
                                                   "<td>%s</td>"
                                                   "<td>%s</td>"
                                                   "<td>%s</td>"
                                                   "<td>%s</td>"
                                                   "</tr>", CaseNum++, suitname.c_str(),
                                                   casename.c_str(), result.c_str(),
                                                   comment.c_str());

    results.push_back(row);
	mtx.unlock();
    write_a_message(row);
}

static void EXPECT_EQ_STR(const XString &casename,
                          const XString & suitname,
                          const char* actual,
                   const char* expect,
                   const XString & comment = __XT("")) {
    EXPECT_EQ(casename, suitname, std::string(actual), std::string(expect), comment);
}

static void EXPECT_NO_TIMEOUT(const XString &casename,
                              std::cv_status s)
{
    if (s == std::cv_status::no_timeout) {
        EXPECT_EQ(casename, __XT("回调成功"), 0, 0);
    } else {
        EXPECT_EQ(casename, __XT("回调超时"), 0, 1);
    }
}

class AutoTestCase;
class AutoTestController
{
public:
    typedef std::list<AutoTestCase*> TestCases;
    static AutoTestController* instance();
    ~AutoTestController();

    void addTest(AutoTestCase *test);
    void start();
    void next();
    void previouse();
private:
    AutoTestController();
    void init();
    TestCases _cases;
    AutoTestCase *_current_test;
    TestCases::iterator itor;
};

class AutoTestCase
{
public:
    AutoTestCase();
    virtual void run();
    void writeResult();
    virtual std::string name() const {
        return _name;
    }
protected:
    std::string _message; //测试结果的字符串
    std::string _name;
    AutoTestController *_controller;
};

#endif /* autocontroller_hpp */
