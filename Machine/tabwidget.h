#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include "imwrapper.h"
#include "YIMPlatformDefine.h"

namespace Ui {
   class TabWidget;
}

extern "C" {
    extern void IM_SetMode(int mode);
}


const static char *PROTOCOL_FROM_STR = "AutoTestToRobot"; //收到的消息检验这个协议头
const static char *PROTOCOL_TO_STR = "AutoTestFromRobot"; //发出的消息带上这个协议头

class TabWidget: public QTabWidget, public ImWrapper
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    ~TabWidget();

public slots:
    void updateUi();

Q_SIGNALS:
    void logined();
    void logout();

private slots:
    void sendMsg();
    void userLogin();
    void userLogout();
    void userLeaveRoom();
    void userJoinRoom();
    void setupChatRoomList(QString);

private:
    //回调重新实现
    void OnRecvMessage(std::shared_ptr<IYIMMessage> message);
    void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID);
    void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID);
    void OnLogin(YIMErrorcode errorcode, const XString& userID);
    void OnLogout(YIMErrorcode code);
    void OnRecvNotice(YIMNotice *notice);

private:
    Ui::TabWidget *ui;

    bool is_login;

};

#endif // TABWIDGET_H
