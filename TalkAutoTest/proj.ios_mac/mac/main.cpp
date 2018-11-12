/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "cocos2d.h"

#include "network/HttpClient.h"

using namespace cocos2d::network;
USING_NS_CC;

void callback(cocos2d::Ref *target) {

}
void func()
{
    HttpRequest* request = new HttpRequest();
    request->setUrl("http://106.75.7.162:8999/sendreport");
    request->setRequestType(HttpRequest::Type::POST);

    // write the post data
    const char* postData = "name=file;filename=Prefix.pch";
    request->setRequestData(postData, strlen(postData));
    request->setResponseCallback(CC_CALLBACK_2(callback, this));
    request->setTag("GET test");
    request->setTag("POST test");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


int main(int argc, char *argv[])
{
    //AppDelegate app;
    //return Application::getInstance()->run();
    func();
    return 0;
}
