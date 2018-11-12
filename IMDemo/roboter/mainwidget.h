#ifndef TEST_SUIT1
#define TEST_SUIT1

#include "../main/callbackwrapper.h"
#include <QDebug>
#include <QWidget>
#include <condition_variable>
#include <mutex>
#include <QLabel>
#include <QMutex>

extern "C" {
    extern void IM_SetMode(int mode);
}


class QTextBrowser;
class QLabel;
class MainWidget: public QWidget,
        public ImWrapper
{
    Q_OBJECT
public:
    explicit MainWidget(QString username,
                       QString roomid,
                       QString passwd=QString(),
                       QString token=QString(),
                       QWidget *parent = 0);
    ~MainWidget();

    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID) {
        qDebug() << errorcode;
        if (errorcode == YIMErrorcode_Success) {
            qDebug() << XStringToLocal(userID).c_str() << "Login success!";
        }
        YIMErrorcode code = YIMManager::CreateInstance()->GetChatRoomManager()
                ->JoinChatRoom(m_roomID.c_str());
        if (code != YIMErrorcode_Success) {
            qDebug() << QStringLiteral("");
        }
        cv.notify_one();
    }
    void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {
        qDebug() << "Join room success..." << XStringToLocal(chatRoomID).c_str();

        YIMManager* im = YIMManager::CreateInstance();
        if (errorcode != YIMErrorcode_Success) {
            qDebug() << tr("Join room failed...");
        }

        #ifdef WIN32
            m_status->setText(QString::number(im->GetSDKVersion())
                              + "\n"
                              + QString::fromStdWString(m_username)
                              + "\n"
                              + QString::fromStdWString(m_roomID));

        #else
            m_status->setText(im->GetSDKVersion() + "\n" + QString::fromStdString(m_username)
                              + "\n" + QString::fromStdString(m_roomID));
        #endif
        cv.notify_one();
    }

    virtual void OnLogout() {
        qDebug() << "User logout" ;
        cv.notify_one();
    }

    void OnRecvMessage( std::shared_ptr<IYIMMessage> message);

    void OnDownload(YIMErrorcode errorcode, std::shared_ptr<IYIMMessage> msg, const XString &savePath);

private slots:
    void quitRobot();
private:
    void init();

private:
    std::mutex mutex;
    std::condition_variable cv;

    XString m_username;
    XString m_roomID;
    XString m_passwd;
    XString m_token;
    QLabel * m_status;
    QTextBrowser *m_editor;

    QMutex textlock;
};

#endif // TEST_SUIT1

