//
//  talkservice.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#ifndef talkservice_hpp
#define talkservice_hpp

#include "IYouMeEventCallback.h"
#include "IYouMeVoiceEngine.h"
#include <stdio.h>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
 #include <YouMeCommon/CrossPlatformDefine/IOS/YouMeApplication_iOS.h>
#endif

const static char *TALK_APPKEY = "YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0";
const static char *TALK_APPSECRET = "r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=";

static const char *TALK_USER_A = "OOOO9999"; // Talk最常用userid
static const char *TALK_USER_B = "OOOO8888";
static const char *TALK_ZHUBO_ID = "ZHUBOOOOO10001";
static const char *TALK_CHAT_ROOM = "12345";


class YoumeUtil
{
public:
//    static std::string getLogPath() {
//        std::string appLogPath = "";
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        appLogPath = "/sdcard/YMTest/log";
//        static std::string wav_save_path = "/sdcard/YMTest/data";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        YouMeApplication_iOS iosapp;
//        appLogPath = iosapp.getCachePath();
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//        appLogPath = "./data/log";
//        static std::string wav_save_path = "./datas";
//#else
//        appLogPath = "";
//        static std::string wav_save_path = "";
//#endif
//        return appLogPath;
//    }


    static std::string verNumToString(int number) {
        int main = number >> 28;
        int minor = (number & 0x0FC00000) >> 22;
        int release = (number & 0x003FC000) >> 14;
        int build = (number & 0x00003FFF);

        char str[20] = {0};
        sprintf(str, "%d.%d.%d.%d", main, minor, release, build);
        return std::string(str);
    }

    static std::string getPlatformName() {
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

    static std::string getCurrTime()
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

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
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

    static std::string getReportFilename() {
        std::string path = getPkgFileName();
//        if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
//            cocos2d::FileUtils::getInstance()->createDirectory(path);
//        }
        char filename[1024]; //路径很长，留足够的空间来装文件名，否则crash
        //long timestap = time(0);
        sprintf(filename, "%s/report_ex.html", path.c_str());
        return filename;
    }

    static std::string getRandUserName(int len = 6)
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

    //sdk log dir
    static std::string getPkgFileName() {
        std::string pkg_log_name = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        pkg_log_name = "/sdcard/Android/data/org.cocos2dx.VideoAutoTest/files";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        YouMeApplication_iOS iosapp;
        pkg_log_name = iosapp.getCachePath() + "/../../Documents";

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        pkg_log_name = "c:/Users/pc/AppData/Local";
#endif
        return pkg_log_name;
    }

    static void clear_sdk_log_file()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || \
CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        std::string logfilename = getPkgFileName() +
            "/ymrtc_log.txt";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        std::string logfilename = getPkgFileName() + "/ymrtc_log.txt";
#endif
        std::fstream file(logfilename, std::ios::out);

        file.close();
    }
private:

};

typedef std::vector<std::string> ReportType;
static ReportType results;
static int CaseNum = 1;

//获得设备类型
static void initfile() {
    results.clear();
    CaseNum = 1;
    std::string fname = YoumeUtil::getReportFilename();
    std::string logfilename = YoumeUtil::getPkgFileName() + "/ymrtc_log.txt";
    std::ifstream infile(logfilename.c_str());

    char *buffer = new char[1024 * 1024];
    std::string cpuinfo = "";
    std::string sdkver = "";
    std::string brand = "";
    std::string model = "";
    std::string uuid = "";
    std::string appkey = "";
    std::string osver = "";
    std::string package = "";
    std::string ffsurport = "";
    std::string commonlibver = "";

    infile.read(buffer, 1024 * 1024 - 1);
    std::regex cupp("CPU.*?\n");
    std::regex sdkverp("sdkver.*?\n");
    std::regex modelp("Model.*?\n");
    std::regex uuidp("UUID.*?\n");
    std::regex brandp("Brand.*?\n");
    std::regex appkeyp(" YOUME.*? ");
    std::regex osverp("sysver.*?\n");
    std::regex pkgp("package.*?\n");
    std::regex commonlibp("commonLibVer.*?\n");
    std::regex ffsurpp("ffmpeg-support.*?\n");

    std::cmatch cm;
    if (std::regex_search(buffer, cm, cupp))cpuinfo = std::string(cm[0]);
    if (std::regex_search(buffer, cm, modelp))model = std::string(cm[0]);
    if (std::regex_search(buffer, cm, sdkverp))sdkver = std::string(cm[0]);
    if (std::regex_search(buffer, cm, brandp))brand = std::string(cm[0]);
    if (std::regex_search(buffer, cm, uuidp))uuid = std::string(cm[0]);
    if (std::regex_search(buffer, cm, appkeyp))appkey = "appkey:" + std::string(cm[0]);
    if (std::regex_search(buffer, cm, osverp))osver = std::string(cm[0]);
    if (std::regex_search(buffer, cm, pkgp))
        package = std::string(cm[0]);
    if (std::regex_search(buffer, cm, commonlibp))
        commonlibver = std::string(cm[0]);
    if (std::regex_search(buffer, cm, ffsurpp))
        ffsurport = std::string(cm[0]);


    std::ofstream outfile(fname.c_str(), std::ios::out);
    //cocos2d::Application::getInstance()->getTargetPlatform();


    outfile << "<!DOCTYPE html>" << std::endl;
    outfile << "<html lang=\"en\">" << std::endl;
    outfile << "<head>" << std::endl;
    outfile << "<meta charset=\"UTF-8\">"<< std::endl;
    outfile << "<title>测试报告</title>"<< std::endl;
    outfile << "<h1>Report</h1>" << std::endl;
    outfile << "<h4>" << YoumeUtil::getCurrTime() << "</h4>" << std::endl;
    outfile << "<h4>" << YoumeUtil::getPlatformName() <<"</h4>" << std::endl; //
    outfile << "<h4>" << osver << "</h4>" << std::endl;
    outfile << "<h4>" << appkey << "</h4>" << std::endl;
    outfile << "<h4>" << model << "</h4>" << std::endl;
    outfile << "<h4>" << uuid << "</h4>" << std::endl;
    outfile << "<h4>" << brand << "</h4>"<< std::endl;
    outfile << "<h4>" << sdkver << "</h4>"<< std::endl;
    outfile << "<h4>" << cpuinfo << "</h4>"<< std::endl;
    outfile << "<h4>" << package << "</h4>"<< std::endl;
    outfile << "<h4>" << commonlibver << "</h4>"<< std::endl;
    outfile << "<h4>" << ffsurport << "</h4>"<< std::endl;
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
    std::string fname = YoumeUtil::getReportFilename();

    std::ofstream outfile(fname.c_str(), std::ios::app);
    outfile << "</table>" << std::endl;
    outfile << "</body>" << std::endl;
    outfile.close();
}

static std::mutex mtx;
static void write_a_message(const std::string & message)
{
    //std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
    //static std::mutex mtx;
    std::string fname = YoumeUtil::getReportFilename();
    mtx.lock();
    std::ofstream outfile(fname.c_str(), std::ios::app);
    outfile << message << std::endl;
    outfile.close();
    mtx.unlock();
}

//需要锁住，做成thread-safe
template <class T>
static void EXPECT_EQ(const std::string&casename, const std::string & suitname,
                      T actual, T expect,
                      const std::string& comment=std::string())
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

static void EXPECT_EQ_STR(const std::string&casename,
                          const std::string& suitname,
                          const char* actual,
                          const char* expect,
                          const std::string& comment = "") {
    EXPECT_EQ(casename, suitname, std::string(actual), std::string(expect), comment);
}

static void EXPECT_NO_TIMEOUT(const std::string &casename,
                              std::cv_status s)
{
    if (s == std::cv_status::no_timeout) {
        EXPECT_EQ(casename, "消息接收成功", 0, 0);
    } else {
        EXPECT_EQ(casename, "消息未确认接收", 0, 1);
    }
}


#endif /* talkservice_hpp */
