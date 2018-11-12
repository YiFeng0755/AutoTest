//
//  MainScene.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/4.
//
//

#include "MainScene.hpp"
#include "cocostudio/CocoStudio.h" 
#include <vector>
//#include "AutoRandTest.hpp"

USING_NS_CC;
using namespace ui;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    layer->setColor(Color3B::WHITE);
    //layer->setPosition(400, 400);
    scene->addChild(layer, 0);
    return scene;
}

bool MainScene::init()
{
    if (!LayerColor::init()) {
        return false;
    }

   // this->addChild(nullptr);
    this->setColor(Color3B(255, 0, 128));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();


    //setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    //setPosition(origin.x, origin.y);

    auto btnTabIm = Button::create();
    btnTabIm->setTitleText("IM Test");
    btnTabIm->setTitleAlignment(TextHAlignment::CENTER);
    btnTabIm->setTitleFontSize(20.0f);
    //btnTabIm->setAnchorPoint(Vec2(0.5, 0.1));
    btnTabIm->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height / 2));
    btnTabIm->addClickEventListener(CC_CALLBACK_1(MainScene::switchToImTestCallback, this));
    btnTabIm->setColor(Color3B(250, 220, 106));
    btnTabIm->setPosition(Vec2(origin.x + visibleSize.width/2 - 80, origin.y + visibleSize.height - 10));

    auto btnTabTalk = Button::create();
    btnTabTalk->setTitleText("Talk Test");
    btnTabTalk->setTitleAlignment(TextHAlignment::RIGHT);
    btnTabTalk->setTitleFontSize(20.0f);
    //btnTabTalk->setAnchorPoint(Vec2(0.5, 0.5));
    btnTabTalk->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 10));
    btnTabTalk->setColor(Color3B(238, 139, 58));
    btnTabTalk->addClickEventListener(CC_CALLBACK_1(MainScene::switchToTalkTestCallback, this));
    
    auto btnPerf = Button::create();
	btnPerf->setTitleText("Perf Test");
	btnPerf->setTitleAlignment(TextHAlignment::RIGHT);
	btnPerf->setTitleFontSize(20.0f);
    //btnPerf->setAnchorPoint(Vec2(0.5, 0.5));
	btnPerf->setPosition(Vec2(origin.x + visibleSize.width / 2 - 120, origin.y + visibleSize.height - 100));
	btnPerf->addClickEventListener(CC_CALLBACK_1(MainScene::switchToPerfCallback, this));

    addChild(btnTabIm);
    addChild(btnTabTalk);
	addChild(btnPerf);

    return true;
}

void MainScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
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
    auto statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    //_labelStatusCode->setString(statusString);
    log("response code: %ld", statusCode);
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        return;
    }
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
    }
    printf("\n");
}

void MainScene::sendLogCallback(cocos2d::Ref *pSender)
{
    HttpRequest* request = new HttpRequest();
    request->setUrl("http://106.75.7.162:8999/sendreport");
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(MainScene::onHttpRequestCompleted, this));

    // write the post data
    const char *postData = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"file\"; filename=\"/Users/micheal/zangci.jmx\"\r\nContent-Type: false\r\n\r\n\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--";
    request->setRequestData(postData, strlen(postData));
    request->setTag("POST test");
    std::vector<std::string> headers;
    headers.push_back("Content-Type: multipart/form-data; boundary=------WebKitFormBoundary7MA4YWxkTrZu0gW");
    //headers.push_back("");
    request->setHeaders(headers);
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void MainScene::switchToImTestCallback(cocos2d::Ref *pSender)
{ 
    //Scene* scene = IMTestScene::createScene();
    //Director::getInstance()->replaceScene(scene);
}

void MainScene::switchToTalkTestCallback(cocos2d::Ref *pSender)
{
    //log("call talk test here!");
    //Scene* scene = TalkTestScene::createScene();
    //log("call talk test here!");
    //Director::getInstance()->replaceScene(scene);
}

void MainScene::switchToPerfCallback(cocos2d::Ref *pSender)
{
	//Scene *perfscene = PerfScene::createScene();
	//Director::getInstance()->replaceScene(perfscene);
}

void MainScene::startIMAutoTestCallback( cocos2d::Ref * pSender ){
    //imTest.registAllTest();
    
    //imTest.StartTest( 3 );
}
void MainScene::startTalkAutoTestCallback( cocos2d::Ref * pSender ){
    //talkTest.registAllTest();
    
    //talkTest.StartTest(3);
}






