#ifndef LOGINDIALOG
#define LOGINDIALOG
#include <QDialog>
#include "../main/callbackwrapper.h"

class QPushButton;
class QLineEdit;
class LoginDialog :public QDialog, public ImWrapper
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID);
    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID);

    QString userID() const;
    QString roomID() const;
private slots:
    void slot_login();

private:
    QString m_userID;
    QString m_roomID;

    QPushButton *m_btn_login;
    QLineEdit *m_edit_roomid;
    QLineEdit *m_edit_userid;
};

#endif // LOGINDIALOG

