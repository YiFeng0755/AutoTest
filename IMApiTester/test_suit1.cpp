#include "test_suit1.h"
#include <QDebug>
#include <QString>
#include <limits>

#define LOG_ERRORCODE(code) (qDebug() << QString("return code: %1").arg(code1));
void TestSuit1::test_QuerySessionHistoryMsgFromServer()
{
//    YIMManager*im = YIMManager::CreateInstance();
//    XUINT64 i {1};
//    YIMErrorcode code1 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
//                __XT(""), i, 1000, 1, 1);
    
//    if (code1 != YIMErrorcode_Success) {
//        qDebug() << "Error!";
//    }

//    YIMErrorcode code2 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
//                __XT("aa123"), i, 1000, 10);

//    if (code2 != YIMErrorcode_Success) {
//        qDebug() << "Error:" << code2;
//    }

//    YIMErrorcode code3 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
//                __XT("aa123"), 202270306809950891, 202270310660321983 , 1, 10);

//    //YIMErrorcode code4 = im->GetMessageManager()->QuerySessionHistoryMsgFromServer(
//   //             __XT("aa123"), 0, 302270310660321983 , 1, -1);
    
//    LOG_ERRORCODE(code1);
//    LOG_ERRORCODE(code3);
}

void TestSuit1::userlogin()
{
    YIMManager* im = YIMManager::CreateInstance();
    std::unique_lock<std::mutex> lk(mutex);
    YIMErrorcode code = im->Login(__XT("AutoRoboter"), __XT("12345"), __XT(""));
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

void TestSuit1::test_QuerySessionHistoryMsgFromServer_repeat()
{
//    int cnt = 0;
//    int fail = 0, success = 0;
//    while (true) {
//        if (++cnt > 10000)
//            break;

//        YIMErrorcode code1 = YIMManager::CreateInstance()->GetMessageManager()->QuerySessionHistoryMsgFromServer(
//                    __XT("1234"), 202270306809950891, 202270310660321983, 1);
//        if (code1 == YIMErrorcode_Success) {
//            ++fail;
//        } else {
//            success++;
//        }
//    }
//    qDebug() << "Fail : " << fail << "Success: " << success;
}

void TestSuit1::test_QueryRoomHistoryMessageFromServer()
{
    YIMErrorcode code = YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(
                __XT("2019889"), 200, 0);
    if (code != YIMErrorcode_Success) {
        qDebug() << "Query error !!!";
    }

//    YIMErrorcode code1 = YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(
//                __XT("2011234"), 100, 0);
//    if (code1 != YIMErrorcode_Success) {
//        qDebug() << "Error is " << code1;
//    }

//    for (int i = 0; i < 1; ++i) {
//        YIMErrorcode code1 = YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(
//                    __XT("2011234"), 100, 0);
//        qDebug() << code1;
//    }
}

void TestSuit1::test_QGetRecentContacts()
{
    YIMManager *im = YIMManager::CreateInstance();
    std::unique_lock<std::mutex> lk(mutex);
    YIMErrorcode code = im->GetRecentContacts();
    if (code != YIMErrorcode_Success) {
        qDebug() << "Error";
    }
    cv.wait_for(lk, std::chrono::seconds(10));
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
        YIMErrorcode code2 = im->GetMessageManager()->SendTextMessage(__XT("aa123"),
                                                                      ChatType_PrivateChat,
                                                                      __XT("test 123445234"), NULL);
//        YIMErrorcode code3 = im->GetMessageManager()->SendTextMessage(__XT("2011234"),
//                                                                      ChatType_RoomChat,
//                                                                      __XT("test 123445234"), NULL);
        if (++cnt > 10)
            break;
    }
}
