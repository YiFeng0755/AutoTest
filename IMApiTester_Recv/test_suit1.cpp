#include "test_suit1.h"
#include <QDebug>
#include <QString>
#include <limits>

#define LOG_ERRORCODE(code) (qDebug() << QString("return code: %1").arg(code1));
void TestSuit1::test_QuerySessionHistoryMsgFromServer()
{
    YIMManager*im = YIMManager::CreateInstance();
    XUINT64 i {1};
    YIMErrorcode code1 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
                __XT(""), i, 1000, 1, 1);
    
    if (code1 != YIMErrorcode_Success) {
        qDebug() << "Error!";
    }

    YIMErrorcode code2 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
                __XT("1234"), i, 1000, 10);

    if (code2 != YIMErrorcode_Success) {
        qDebug() << "Error:" << code2;
    }

    YIMErrorcode code3 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
                __XT("aa123"), 0, 2523329926666 , 1, 10);

    YIMErrorcode code4 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
                __XT("aa123"), 0, 2523329926666 , 1, -1);
    
    LOG_ERRORCODE(code1);
}

void TestSuit1::userlogin()
{
    YIMManager* im = YIMManager::CreateInstance();
    std::unique_lock<std::mutex> lk(mutex);
    YIMErrorcode code = im->Login(__XT("1234"), __XT("12345"), __XT(""));
    qDebug() << "code is" <<  code;
    cv.wait_for(lk, std::chrono::seconds(10));
}

void TestSuit1::userlogout()
{
    YIMManager *im = YIMManager::CreateInstance();
    std::unique_lock<std::mutex> lk(mutex);
    im->Logout();
    cv.wait_for(lk, std::chrono::seconds(10));
    qDebug() << "Login successs";
}

void TestSuit1::sendPrivateMsg()
{
    YIMManager *im = YIMManager::CreateInstance();

    std::unique_lock<std::mutex> lk(mutex);

    YIMErrorcode code1 = im->GetChatRoomManager()->JoinChatRoom(__XT("2011234"));
    if (code1 == YIMErrorcode_Success) {
        cv.wait_for(lk, std::chrono::seconds(10));
    }


    int cnt = 0;
    while (true) {
        YIMErrorcode code2 = im->GetMessageManager()->SendTextMessage(__XT("1234"),
                                                                      ChatType_PrivateChat,
                                                                      __XT("test 123445234"), NULL);
//        YIMErrorcode code3 = im->GetMessageManager()->SendTextMessage(__XT("2011234"),
//                                                                      ChatType_RoomChat,
//                                                                      __XT("test 123445234"), NULL);
        if (++cnt > 10)
            break;
    }
}
