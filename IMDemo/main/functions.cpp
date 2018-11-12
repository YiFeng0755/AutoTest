#include "functions.h"
#include "roommanager.h"
#include <QDebug>
#include <QInputDialog>

#include "ui_functions.h"

LoginDialog::LoginDialog(QWidget *parent)
    :QDialog(parent)
{
    QPushButton *ok = new QPushButton(QStringLiteral("确定"));
    QPushButton *cancel = new QPushButton(QStringLiteral("取消"));

    line_edit_passwd = new QLineEdit();
    line_edit_passwd->setPlaceholderText(QStringLiteral("密码,不填默认为123456"));
    line_edit_username = new QLineEdit();
    line_edit_username->setPlaceholderText(QStringLiteral("用户名"));
    line_edit_token = new QLineEdit();
    line_edit_token->setPlaceholderText("token");

    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(ok);
    btnLayout->addWidget(cancel);
    btnLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(line_edit_username);
    layout->addWidget(line_edit_passwd);
    layout->addWidget(line_edit_token);
    layout->addLayout(btnLayout);

    setLayout(layout);
}

QString LoginDialog::userName() const
{
    return line_edit_username->text();
}

QString LoginDialog::loginPasswd() const
{
    return line_edit_passwd->text();
}

QString LoginDialog::token() const
{
    return line_edit_token->text();
}

FunctionsWidget::FunctionsWidget(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::FunctionTestDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("功能测试"));
    setup();
}

FunctionsWidget::~FunctionsWidget()
{
    std::unique_lock<std::mutex> lk(m_mutex);
    leaveAllRoom();
    logout();
    m_cv.wait_for(lk, std::chrono::seconds(10));
}

void FunctionsWidget::setup()
{
    YIMManager *im = YIMManager::CreateInstance();
    im->SetLoginCallback(this);
    im->SetChatRoomCallback(this);

    connect(ui->btn_login, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->btn_logout, SIGNAL(clicked()), this, SLOT(logout()));
    connect(ui->btn_joinroom, SIGNAL(clicked()), this, SLOT(joinRoom()));
    connect(ui->btn_leave_room, SIGNAL(clicked()), this, SLOT(leaveRoom()));
    connect(ui->btn_leave_all_room, SIGNAL(clicked()), this, SLOT(leaveAllRoom()));
    connect(ui->btn_msg_manager, SIGNAL(clicked()), this, SLOT(message_widget_show()));
    connect(ui->btn_voice_msg, SIGNAL(clicked()), this, SLOT(voice_widget_show()));
    connect(ui->btn_room_manager, SIGNAL(clicked()), this, SLOT(room_manager_show()));
}

void FunctionsWidget::login()
{
    YIMManager *im = YIMManager::CreateInstance();

    LoginDialog loginDlg(this);
    if (loginDlg.exec() != QDialog::Accepted) {
        return;
    }

    QString userID = loginDlg.userName();
    QString passwd = loginDlg.loginPasswd();
    if (passwd.isEmpty())
        passwd = "123456";
    QString token = loginDlg.token();
    YIMErrorcode code =
    im->Login(LocalToXString(userID.toStdString()).c_str(),
              passwd.toStdWString().c_str(),
              token.toStdWString().c_str());
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登录失败 code is") << code;
    }
}

void FunctionsWidget::joinRoom()
{
    YIMManager *im = YIMManager::CreateInstance();
    QString roomID =
            QInputDialog::getText(this, tr("Input room id"), tr("User Join room"));
    YIMErrorcode code =
    im->GetChatRoomManager()->JoinChatRoom(LocalToXString(roomID.toStdString()).c_str());
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("加入房间失败") << code;
    }
}

void FunctionsWidget::room_manager_show()
{
    RoomManagerDialog manager(m_current_room, 0);
    manager.exec();
}

void FunctionsWidget::voice_widget_show()
{

}

void FunctionsWidget::message_widget_show()
{
    //MessageControllDialog messagedialog(m_current_room, m_current_user);
    //messagedialog.exec();
}

void FunctionsWidget::leaveRoom()
{
    YIMManager *im = YIMManager::CreateInstance();
    if (m_joinRoomIDs.empty()) {
        YIMErrorcode code1 = im->GetChatRoomManager()->LeaveChatRoom(__XT(""));
        if (code1 != YIMErrorcode_Success)
            qDebug() << "Cannot Leave room : " << code1;

        return;
    }
    XString roomid = m_joinRoomIDs.at(0);
    qDebug() << "Now leaving chatroom " << XStringToLocal(m_joinRoomIDs.at(0)).c_str();
    YIMErrorcode code2 = im->GetChatRoomManager()->LeaveChatRoom(roomid.c_str());
    if (code2 != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("离开房间错误") << code2;
    }
}

void FunctionsWidget::leaveAllRoom()
{
    YIMManager *im = YIMManager::CreateInstance();

    YIMErrorcode code2 = im->GetChatRoomManager()->LeaveAllChatRooms();
    if (code2 != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("离开所有房间错误") << code2;
    }
}

void FunctionsWidget::OnLogin(YIMErrorcode errorcode, const XString &userID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << XStringToLocal(userID).c_str() << "Login success!";
    }else {
        qDebug() << XStringToLocal(userID).c_str() <<  QStringLiteral("用户登录失败") << errorcode;
    }
    m_current_user = userID;
}

void FunctionsWidget::logout()
{
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->Logout();
    if (code == YIMErrorcode_Success) {
        qDebug() << "Logout success!";
    }else {
        qDebug() << QStringLiteral("用户登出失败");
    }
}

void FunctionsWidget::OnLogout(YIMErrorcode errorcode)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << "User logout";
    } else {
        qDebug() << QStringLiteral("用户登出失败");
    }
    m_cv.notify_all();
}

void FunctionsWidget::OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << "join room success" << XStringToLocal(chatRoomID).c_str();
    } else {
        qDebug() << QStringLiteral("加入房间失败");
    }
    m_joinRoomIDs.append(chatRoomID);
    m_current_room = chatRoomID;
}

void FunctionsWidget::OnLeaveChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << "Leave room success " << XStringToLocal(chatRoomID).c_str();
    } else {
        qDebug() << QStringLiteral("离开房间失败");
    }
}

void FunctionsWidget::OnLeaveAllChatRooms(YIMErrorcode errorcode)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << "Leave all room success " ;
    }else {
        qDebug() << QStringLiteral("离开全部房间失败");
    }
}
