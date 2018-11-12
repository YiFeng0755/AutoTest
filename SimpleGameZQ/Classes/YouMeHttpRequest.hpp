//
//  YouMeHttpRequest.hpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/27.
//
//

#ifndef YouMeHttpRequest_hpp
#define YouMeHttpRequest_hpp

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "imservice.hpp"
#include "sha1.h"
#include <sstream>
#include <vector>
using std::stringstream;
using std::ios;
using std::vector;

USING_NS_CC;
using namespace network;

static std::string URL = "http://106.75.14.78:18080/v1/im/";

class YouMeHttpRequest
{
public:

    static void request(const char* uri, const std::string& data) {
        HttpRequest* request = new (std :: nothrow) HttpRequest();

        stringstream ss;
        ss << time(0);
        std::string currenttime = ss.str();
        ss.str("");

        //char checksum[64];
        SHA1 sha1;
        std::string src = (XStringToUTF8(XString(APPSECRET)) + currenttime);
        sha1.update(src);
        const std::string checksum = sha1.result();
        //ss.setf(ios::hex|ios::uppercase,std::ios::basefield);
        //ss << digest[0] << digest[1] << digest[2] << digest[3] << digest[4];
        //checksum = ss.str();

        std::string fullurl;
        fullurl += URL;
        fullurl += uri;

        char strbuf[1024];
        sprintf(strbuf, "?appkey=%s&curtime=%s&checksum=%s&identifier=admin", APPKEY, currenttime.c_str(), checksum.c_str());

        fullurl += strbuf;

        request->setUrl(fullurl);
        request->setRequestType(HttpRequest::Type::POST);
        vector<std::string> headers;
        headers.push_back("Content-Type:application/json; charset=utf-8");
        request->setHeaders(headers);

        request->setResponseCallback(YouMeHttpRequest::onCompletedHttpResponseCallback);
        request->setRequestData(data.c_str(), data.length());
        HttpClient::getInstance()->send(request);
    }

    static void onCompletedHttpResponseCallback(HttpClient *sender, HttpResponse *response)
    {
        if (!response)
        {
            return;
        }

        // You can get original request type from: response->request->reqType
        if (0 != strlen(response->getHttpRequest()->getTag()))
        {
            log("%s completed", response->getHttpRequest()->getTag());
        }
        long statusCode = response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());

        log("response code: %ld", statusCode);
        if (!response->isSucceed())
        {
            log("response failed");
            log("error buffer: %s", response->getErrorBuffer());
            return;
        }
        // dump data
        std::vector<char> *buffer = response->getResponseData();

        printf("%s, Http Test, dump data: ", response->getHttpRequest()->getUrl());
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            printf("%c", (*buffer)[i]);
        }
        printf("\n");
    }

    //std::string url = "http://106.75.7.162:8999/sendreport"; 正式环境，测试时务必使用测试环境
    //std::string filename = ::filename();
    static void sendFile(const char* url, const std::string& filename, const char* extentsion) {

        HttpRequest *request = new (std::nothrow) HttpRequest();

        Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
        unsigned char * pBuffer = data.getBytes();
        const char* fileBinary = nullptr;
        if (pBuffer == nullptr) {
            cocos2d::log("Data is null");
            fileBinary = "NULL";
        } else {
            fileBinary = (const char*)pBuffer;
        }

        cocos2d::log("fileBinary %s = ", fileBinary);


        //设置 http headers  参数 和  数据流
        std::string boundary = "----WebKitFormBoundaryd9HxN63p8xHAov31";
        std::vector<std::string> headers;
        headers.push_back("Content-Type:multipart/form-data; boundary=" + boundary); //=号要靠近

        std::string strdata(fileBinary);
        std::string payload = "\r\n--" + boundary + "\r\n";

        std::string content = cocos2d::StringUtils::format("Content-Disposition: form-data; name=\"file\"; filename=\"%s_report.html\"\r\n", extentsion);
        payload += content;
        payload += "Content-Type: application/octet-stream\r\n\r\n";

        payload += strdata;

        payload += "\r\n--" + boundary + "--\r\n";

        request->setUrl(url);
        request->setHeaders(headers);
        request->setRequestType(HttpRequest::Type::POST);
        //cocos2d::log("data: %s", str.c_str());
        request->setRequestData(payload.data(), payload.size());
        request->setResponseCallback(YouMeHttpRequest::onCompletedHttpResponseCallback);

        //request->setRequestData(data.c_str(), data.length());
        HttpClient::getInstance()->send(request);
        // 设置设置Header

        request->release();
    }
};
#endif /* YouMeHttpRequest_hpp */
