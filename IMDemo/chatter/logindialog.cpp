#include "logindialog.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include "YIM.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    :QDialog(parent)
{
    YIMManager::CreateInstance()->SetLoginCallback(this);
    YIMManager::CreateInstance()->SetChatRoomCallback(this);

    m_btn_login = new QPushButton(QStringLiteral("登录"));

    connect(m_btn_login, SIGNAL(clicked(bool)), this, SLOT(slot_login()));
    m_edit_roomid = new QLineEdit;
    m_edit_userid = new QLineEdit;

    QLabel *lbl_roomid = new QLabel("RoomID: ");
    QLabel *lbl_userid = new QLabel("UserID: ");

    QHBoxLayout *layout1 = new QHBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;

    layout1->addWidget(lbl_userid);
    layout1->addWidget(m_edit_userid);

    layout2->addWidget(lbl_roomid);
    layout2->addWidget(m_edit_userid);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addWidget(m_btn_login);

    setLayout(mainLayout);
}

LoginDialog::~LoginDialog()
{
    YIMManager::CreateInstance()->SetLoginCallback(nullptr);
    YIMManager::CreateInstance()->SetChatRoomCallback(nullptr);
}

void LoginDialog::OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode != YIMErrorcode_Success) {
        QMessageBox::information(this, QString("info"),
                                 QString("Join room %1 failed").
                                 arg(QString::fromStdWString(chatRoomID)),
                                 QMessageBox::Ok);
        return;
    }
    qDebug() << "Join room: " << QString::fromStdWString(chatRoomID) << " success!";
}

void LoginDialog::OnLogin(YIMErrorcode errorcode, const XString &userID)
{
    if (errorcode == YIMErrorcode_Success) {
        QString roomid = m_edit_roomid->text();
        if (roomid.isEmpty()){
            QMessageBox::information(this, QString("info"),
                                     QString("RoomID cannot be empty!"),
                                     QMessageBox::Ok);
            return;
        }
        YIMErrorcode code =
            YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom(LocalToXString(roomid.toStdString()).c_str());
        if (code != YIMErrorcode_Success) {
            QMessageBox::warning(this, QString("Warning"),
                                 QString("Join room id %1").arg(roomid));
        }
    }
    qDebug() << "Login success " << QString::fromStdWString(userID);
}

QString LoginDialog::userID() const
{
    return m_userID;
}

QString LoginDialog::roomID() const
{
    return m_roomID;
}

void LoginDialog::slot_login()
{
    QString userid = m_edit_userid->text();
    if (userid.isEmpty()) {
        QMessageBox::warning(this, QString("Warning")
                             , QStringLiteral("用户名ID不能为空")
                             , QMessageBox::Ok);
        return;
    }
    YIMErrorcode code = YIMManager::CreateInstance()->Login(
                LocalToXString(userid.toStdString()).c_str(),
                __XT("123456"),
                __XT(""));

    if (code != YIMErrorcode_Success)
        qDebug() << QStringLiteral("登录不成功, 错误码: ") << code;
}
