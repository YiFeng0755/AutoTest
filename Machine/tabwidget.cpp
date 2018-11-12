#include "tabwidget.h"
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QDebug>

#include <thread>

#include "ui_tabwidget.h"
#include "YIM.h"

TabWidget::TabWidget(QWidget *parent)
    :QTabWidget(parent),
      ui(new Ui::TabWidget)
{
    ui->setupUi(this);
    IM_SetMode(2);
    is_login = false;

    YIMManager *handler = YIMManager::CreateInstance();
    handler->SetLoginCallback(this);
    handler->SetMessageCallback(this);
    handler->SetChatRoomCallback(this);
    handler->SetContactCallback(this);
    handler->SetDownloadCallback(this);
    //handler->SetMode(2);

    ui->label->setText(tr("IM version: %1").arg(QString::number(handler->GetSDKVersion())));

    ui->lineEdit_roomid->setText("2019899");
    ui->lineEdit_robotId->setText("2019899");
    ui->lineEdit_userid->setText("AutoRobot");
    ui->lineEdit_robotId->setText("AutoRobot");
    ui->lineEdit_roomid_robot->setText("2019899");
    ui->lineEdit_roomid_robot->setReadOnly(false);

    ui->radioButton_chatroom->setChecked(true);
    ui->radioButton_text->setChecked(true);

    connect(ui->btn_login, SIGNAL(clicked(bool)), this, SLOT(userLogin()));
    connect(ui->btn_logout, SIGNAL(clicked(bool)), this, SLOT(userLogout()));
    connect(ui->btn_joinroom, SIGNAL(clicked(bool)), this, SLOT(userJoinRoom()));
    connect(ui->btn_leaveroom, SIGNAL(clicked(bool)), this, SLOT(userLeaveRoom()));

    connect(this, SIGNAL(logined()), this, SLOT(updateUi()));
    connect(this, SIGNAL(logout()), this, SLOT(updateUi()));
    connect(ui->btn_sendMsg, SIGNAL(clicked(bool)), this, SLOT(sendMsg()));

    ui->textEdit_robotChatRoom->setText(QString(""));
}

TabWidget::~TabWidget()
{
   // YIMManager::CreateInstance()->DestroyInstance();
}


void TabWidget::OnLogin(YIMErrorcode errorcode, const XString& userID)
{
    if (errorcode == YIMErrorcode_Success) {
        is_login = true;
        qDebug() << tr("User: %1 Login success!").arg(QString::fromStdString(XStringToLocal(userID)));
    }
    else {
        qDebug() << tr("User: %1 Login Fail! Reson: %2").
                    arg(QString::fromStdString(XStringToLocal(userID))).
                    arg(errorcode);
    }

    emit logined();
}

void TabWidget::OnLogout(YIMErrorcode errorcode)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << tr("Logout success");
        is_login = false;
    }
    emit logout();
}

void TabWidget::userJoinRoom()
{
    if (ui->lineEdit_roomid->text().isEmpty()) {
        qDebug() << tr("RoomId is empty");
        return;
    }
    XString xroomid = LocalToXString(ui->lineEdit_roomid->text().toStdString());
    const XCHAR *roomid = xroomid.c_str();

    YIMManager* handler = YIMManager::CreateInstance();
    Q_ASSERT(handler != nullptr);
    YIMErrorcode code = handler->GetChatRoomManager()->JoinChatRoom(roomid);
    if (code != YIMErrorcode_Success) {
        qDebug() << tr("Join Room Failed! code:%1").arg(code);
    }
    ui->lineEdit_roomid->setReadOnly(true);
}

void TabWidget::userLeaveRoom()
{
    if (ui->lineEdit_roomid->text().isEmpty()) {
        qDebug() << tr("RoomId is empty");
        return;
    }
    const XCHAR* roomid = LocalToXString(ui->lineEdit_roomid->text().toStdString()).c_str();
    YIMManager* handler = YIMManager::CreateInstance();
    Q_ASSERT(handler != nullptr);
    YIMErrorcode code = handler->GetChatRoomManager()->LeaveChatRoom(roomid);
    if (code != YIMErrorcode_Success) {
        qDebug() << tr("Leave Room Failed. The return code is %1").arg(code);
    }
    ui->lineEdit_roomid->setReadOnly(false);
}


void TabWidget::OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << tr("Join room failed. code: %1").arg(errorcode);
        return;
    }
    QString roomId = QString::fromStdString(XStringToLocal(chatRoomID));
    qDebug() << tr("Join room %1 success.").arg(roomId);
    setupChatRoomList(roomId);
}

void TabWidget::OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << tr("Leave room failed. code: %1").arg(errorcode);
        return;
    }
    QString roomId = QString::fromStdString(XStringToLocal(chatRoomID));
    qDebug() << tr("leave room %1 success.").arg(roomId);
}

void TabWidget::userLogin()
{
    XString Uuserid= UTF8TOXString(ui->lineEdit_userid->text().toStdString());
    const XCHAR* userId = Uuserid.c_str();

    YIMManager* handler = YIMManager::CreateInstance();
    Q_ASSERT(handler != nullptr);
    YIMErrorcode code = handler->Login(userId, __XT("12345"), __XT(""));
    if (code != YIMErrorcode_Success) {
        qDebug() << tr("%1 Login Failed! The return code is %2")
                    .arg(ui->lineEdit_userid->text()).arg(code);
    }
    ui->lineEdit_userid->setReadOnly(true);
}

void TabWidget::userLogout()
{
    YIMManager* handler = YIMManager::CreateInstance();
    Q_ASSERT(handler != nullptr);
    YIMErrorcode code = handler->Logout();
    if (code != YIMErrorcode_Success) {
        qDebug() << tr("Logout failed; The return code is ") << code;
    }
}

void TabWidget::sendMsg()
{

    //int type, QString targetId
    XUINT64 reqNo;

    YIMChatType type = ui->radioButton_private->isChecked()? ChatType_PrivateChat : ChatType_RoomChat;
    QString sendMsgType = "text";
    if (ui->radioButton_giif->isChecked()) sendMsgType = "gift";
    if (ui->radioButton_custom->isChecked()) sendMsgType = "custom";

    const int NUM = 5;
    static const XCHAR * texts[NUM] = {
        __XT("工欲善其事必先利其器"),
        __XT("Stay hungry, stay foolish."),
        __XT("知行合一,至于至善"),
        __XT("Youme最棒;欢迎来到这里"),
        __XT("见贤思齐，见不贤而内自省也！")
    };

    const XCHAR * text = texts[qrand() % NUM];


    YIMManager *handler = YIMManager::CreateInstance();
    if (type == ChatType_PrivateChat) {
       // QString targetId = ui->lineEdit_robotId->text();
        QString targetId = QString("222");

        const int number = 100;
        YIMErrorcode code;
        if (sendMsgType == "text") {
            for (int i = 0; i < number; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                code =
                handler->GetMessageManager()->SendTextMessage(LocalToXString(targetId.toStdString()).c_str(), ChatType_PrivateChat,
                                                          text, &reqNo);
            }
            qDebug() << tr("Send %1 message done!").arg(number);

        } else if (sendMsgType == "gift") {
            //
        } else if (sendMsgType == "custom") {
            //
        }

        if (code != YIMErrorcode_Success) {
            qDebug() << tr("Send Message Failed; Reason is %1").arg(code);
        }
    }
    if (type == ChatType_RoomChat) {
        QString targetId = ui->lineEdit_roomid->text();
        YIMErrorcode code;
        if (sendMsgType == "text") {
            code = handler->GetMessageManager()->SendTextMessage(LocalToXString(targetId.toStdString()).c_str(), ChatType_RoomChat,
                                                              text, &reqNo);
        } else if (sendMsgType == "gift") {
            //
        } else if (sendMsgType == "custom") {
            //
        }

        if (code != YIMErrorcode_Success) {
            qDebug() << tr("Send Message Failed; Reason is %1").arg(code);

        }
    }

    qDebug() << tr("Type is not valid!");
}

void TabWidget::updateUi()
{
    ui->btn_login->setEnabled(!is_login);
    ui->btn_logout->setEnabled(is_login);
}

void TabWidget::OnRecvMessage(std::shared_ptr<IYIMMessage> pMessage)
{
    uint createTime = pMessage->GetCreateTime();
    XString senderId = pMessage->GetSenderID();
    XString recvId = pMessage->GetReceiveID();

    YIMMessageBodyType msgType = pMessage->GetMessageBody()->GetMessageType();
    if (msgType == YIMMessageBodyType::MessageBodyType_TXT) {
        //文本消息
        //static int index = 0;
        IYIMMessageBodyText* pMsg = static_cast<IYIMMessageBodyText*>(pMessage->GetMessageBody());
        Q_ASSERT(pMsg != nullptr);

        std::string text = XStringToLocal(XString(pMsg->GetMessageContent()));
        qDebug() << tr("Recv message: %1").arg(QString::fromStdString(text));

        QString messageText = QString::fromStdString(text);
        qDebug() << messageText;
        if (messageText.startsWith(PROTOCOL_FROM_STR)) {
            YIMManager*handler = YIMManager::CreateInstance();

            XUINT64 reqNo;
            std::string stdStr = std::string(PROTOCOL_TO_STR) + ("I Have revc your message");
            XString sendMessageText = LocalToXString(stdStr);
            if (XStringToLocal(recvId) != ui->lineEdit_roomid_robot->text().toStdString())
            {//如果接受者的ID不是房间ID，那就当成是私聊处理
                YIMErrorcode code =
                handler->GetMessageManager()->SendTextMessage(senderId.c_str(), ChatType_PrivateChat,
                                                          sendMessageText.c_str(), &reqNo);
                if (code != YIMErrorcode_Success) {
                    qDebug() << tr("Send Message Failed; Reason is %1").arg(code);
                }else{
                    qDebug() << tr("Send message to %1 success...").arg(QString::fromStdString(XStringToLocal(senderId)));
                }
            } else {
                YIMErrorcode code =
                handler->GetMessageManager()->SendTextMessage(recvId.c_str(), ChatType_RoomChat,
                                                          sendMessageText.c_str(), &reqNo);
                if (code != YIMErrorcode_Success) {
                    qDebug() << tr("Send Message Failed; Reason is %1").arg(code);
                }else {
                    qDebug() << tr("Send message to %1 success...").arg(QString::fromStdString(XStringToLocal(recvId)));
                }
            }
        }

        std::string _showText = XStringToLocal(XString(pMsg->GetMessageContent()));
        QString showText = QString::number(createTime) + QString::fromStdString(_showText);
        ui->textEdit_robotChatRoom->append(showText);
    }
}

void TabWidget::OnRecvNotice(YIMNotice *notice)
{
    if (!notice)
        return;

    std::string text = XStringToLocal(XString(notice->GetContent()));
    qDebug() << tr("Recv notice: %1").arg(QString::fromStdString(text));
    qDebug() << tr("NoticeID: ") << notice->GetNoticeID();
    qDebug() << tr("Link addr: %1").arg(QString::fromStdString(XStringToLocal(XString(notice->GetLinkAddr()))));
    qDebug() << tr("Link text: %1").arg(QString::fromStdString(XStringToLocal(XString(notice->GetLinkText()))));
    qDebug() << tr("BeginTime: ") << notice->GetEndTime();
    qDebug() << tr("EndTime") << notice->GetBeginTime();
    qDebug() << tr("Notice type: ") << notice->GetNoticeType();

    QString messageText = QString::fromStdString(text);
    if (messageText.startsWith(PROTOCOL_FROM_STR)) {
        YIMManager*handler = YIMManager::CreateInstance();

        XUINT64 reqNo;
        std::string stdStr = std::string(PROTOCOL_TO_STR) + ("I Have revc your message");
        XString sendMessageText = LocalToXString(stdStr);

        YIMErrorcode code =
        handler->GetMessageManager()->SendTextMessage(notice->GetChannelID(), ChatType_RoomChat,
                                                  sendMessageText.c_str(), &reqNo);
        if (code != YIMErrorcode_Success) {
            qDebug() << tr("Send Message Failed; Reason is %1").arg(code);
        }

    }
}

void TabWidget::setupChatRoomList(QString roomId)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    if (model == nullptr) {
        model = new QStandardItemModel();
    }

    QStandardItem *item = new QStandardItem(roomId);
    item->setIcon(QIcon(":/images/aa.png"));

    QList<QStandardItem*> items;
    items << item;

    model->appendRow(items);

    ui->listView->setModel(model);
}
