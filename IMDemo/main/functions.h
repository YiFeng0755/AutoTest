#ifndef FUNCTIONS
#define FUNCTIONS

#include <QDialog>
#include "callbackwrapper.h"
#include <mutex>
#include <condition_variable>

class QLineEdit;
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = 0);
    QString token() const;
    QString userName() const;
    QString loginPasswd() const;
private:

    QLineEdit *line_edit_passwd;
    QLineEdit *line_edit_username;
    QLineEdit *line_edit_token;
};

namespace Ui {
class FunctionTestDialog;
}

class FunctionsWidget : public QDialog,
        public ImWrapper
{
    Q_OBJECT
public:
    explicit FunctionsWidget(QWidget *parent = 0);
    ~FunctionsWidget();

private slots:
    void login();
    void logout();
    void joinRoom();
    void leaveRoom();
    void leaveAllRoom();
    void message_widget_show();
    void room_manager_show();
    void voice_widget_show();

protected:
    void OnLogin(YIMErrorcode errorcode, const XString &userID);
    void OnLogout(YIMErrorcode errorcode);
    void OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID);
    void OnLeaveChatRoom(YIMErrorcode errorcode, const XString &chatRoomID);
    void OnLeaveAllChatRooms(YIMErrorcode errorcode);

private:
    void setup();

    Ui::FunctionTestDialog *ui;

    QList<XString> m_joinRoomIDs;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    XString m_current_user;
    XString m_current_room;
};

#endif // FUNCTIONS

