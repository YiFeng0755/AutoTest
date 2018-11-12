#ifndef TEST_SUIT1
#define TEST_SUIT1

#include "callbackwrapper.h"
#include <QDebug>

#include <condition_variable>
#include <mutex>


extern "C" {
    extern void IM_SetMode(int mode);
}

class TestSuit1: public ImWrapper
{
public:
    TestSuit1(){
        IM_SetMode(2);

        YIMManager::CreateInstance()->SetLoginCallback(this);
        YIMManager::CreateInstance()->SetMessageCallback(this);
        YIMManager::CreateInstance()->SetContactCallback(this);
        YIMManager::CreateInstance()->SetChatRoomCallback(this);
        YIMManager::CreateInstance()->SetContactCallback(this);

    }
    ~TestSuit1() {
        userlogout();
    }

    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID) {
        qDebug() << errorcode;
        if (errorcode == YIMErrorcode_Success) {
            qDebug() << XStringToLocal(userID).c_str() << "Login success!";
        }
        cv.notify_one();
    }
    void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {
        qDebug() << "Join room success..." << XStringToLocal(chatRoomID).c_str();
        cv.notify_one();
    }

    virtual void OnLogout() {
        qDebug() << "User logout" ;
        cv.notify_one();

    }

    virtual void OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode,
                                                     const XString& roomID,
                                                     int remain, std::list<std::shared_ptr<IYIMMessage> >& messageList)
    {
        qDebug() << "Room ID is : " << XStringToLocal(roomID).c_str() << " " << errorcode;
        qDebug() << remain << "... ";
        foreach (std::shared_ptr<IYIMMessage> message, messageList) {
            qDebug() << "Recv: " <<
            message->GetMessageID() << "\n";
            IYIMMessageBodyText* textbody =
                     static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            qDebug() << "Message body is: " << XStringToLocal(XString(textbody->GetMessageContent())).c_str();
            qDebug() << "======================";

        }
    }

    void OnQuerySessionHistoryMsgFromServer(YIMErrorcode errorcode,
                                            const XString& userID,
                                            std::list<std::shared_ptr<IYIMMessage> >& messageList)
    {
        YIMManager *im = YIMManager::CreateInstance();
        //qDebug() << XStringToLocal(roomID).c_str() << " " << errorcode;
        qDebug() << XStringToLocal(userID).c_str() << " " << errorcode;;
        foreach (std::shared_ptr<IYIMMessage> message, messageList) {
            qDebug() << "Recv: " <<
            message->GetMessageID() << "\n";
            IYIMMessageBodyText* textbody =
                     static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            qDebug() << XStringToLocal(XString(textbody->GetMessageContent())).c_str();
            qDebug() << "======================";

            im->GetMessageManager()->SetMessageRead(message->GetMessageID(), true);


        }
    }

    void OnGetRecentContacts(YIMErrorcode errorcode,
                             std::list<std::shared_ptr<IYIMContactsMessageInfo> >& contactList)
    {
        qDebug() << errorcode;
        foreach(std::shared_ptr<IYIMContactsMessageInfo> contact, contactList) {
            qDebug() << " =================";
            qDebug() << "contact id: " << XStringToLocal(XString(contact->GetContactID())).c_str();
            //qDebug() << "msg not read num" << contact->GetNotReadMsgNum();
            qDebug() << "contact id" << contact->GetContactID();
            qDebug() << "create time : " << contact->GetCreateTime();
            qDebug() << "msg type: " << contact->GetMessageType();
        }
    }

    void test_QuerySessionHistoryMsgFromServer();
    void test_QGetRecentContacts();

    void test_QuerySessionHistoryMsgFromServer_repeat();
    void test_QueryRoomHistoryMessageFromServer();
    void userlogin();
    void userlogout();

    void sendPrivateMsg();

private:
    std::mutex mutex;
    std::condition_variable cv;
};

#endif // TEST_SUIT1

