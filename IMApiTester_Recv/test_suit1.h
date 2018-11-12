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
        //IM_SetMode(2);

        YIMManager::CreateInstance()->SetLoginCallback(this);
        YIMManager::CreateInstance()->SetMessageCallback(this);
        YIMManager::CreateInstance()->SetContactCallback(this);
        YIMManager::CreateInstance()->SetChatRoomCallback(this);

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
        qDebug() << XStringToLocal(roomID).c_str() << " " << errorcode;
        qDebug() << remain << "... ";
        foreach (std::shared_ptr<IYIMMessage> message, messageList) {
            qDebug() << "Recv: " <<
            message->GetMessageID() << "\n";
            IYIMMessageBodyText* textbody =
                     static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            qDebug() << XStringToLocal(XString(textbody->GetMessageContent())).c_str();
            qDebug() << "======================";

        }

    }

    void OnQuerySessionHistoryMsgFromServer(YIMErrorcode errorcode,
                                            const XString& userID,
                                            std::list<std::shared_ptr<IYIMMessage> >& messageList)
    {
        //qDebug() << XStringToLocal(roomID).c_str() << " " << errorcode;
        qDebug() << XStringToLocal(userID).c_str() << " " << errorcode;;
        foreach (std::shared_ptr<IYIMMessage> message, messageList) {
            qDebug() << "Recv: " <<
            message->GetMessageID() << "\n";
            IYIMMessageBodyText* textbody =
                     static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            qDebug() << XStringToLocal(XString(textbody->GetMessageContent())).c_str();
            qDebug() << "======================";

        }
    }

    void OnRecvMessage( std::shared_ptr<IYIMMessage> message)
    {
        XUINT64 id = message->GetMessageID();
        IYIMMessageBodyText* text = static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
        qDebug() << XStringToLocal(XString(text->GetMessageContent())).c_str();
    }


    void test_QuerySessionHistoryMsgFromServer();
    void userlogin();
    void userlogout();

    void sendPrivateMsg();

private:
    std::mutex mutex;
    std::condition_variable cv;
};

#endif // TEST_SUIT1

