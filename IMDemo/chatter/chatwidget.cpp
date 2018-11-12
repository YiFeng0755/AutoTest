#include "chatwidget.h"
#include "YIM.h"
#include "../main/globalsetting.h"
#include "../main/httprequester.h"
#include "../main/outputwindow.h"
#include "ui_chatform.h"
#include <QDebug>
#include <QTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTimer>
#include <QMenu>
#include <time.h>

OutputWindow* ChatWidget::m_outputWindow = NULL;

ChatWidget::ChatWidget(const XString& roomID,
                       const XString& userID,
                       const XString& appkey,
                       const XString& appSecret,
                       int mode,
                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatForm),
      is_testMode(mode),
      m_appkey(appkey),
      m_appSecret(appSecret)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    m_roomID = roomID;
    m_userID = userID;

    /*debug*/
    if (m_roomID.empty())
        m_roomID = __XT("2014444");
    if (m_userID.empty())
        m_userID = __XT("user8123");

    qsrand((unsigned)time(nullptr));

    QString robotID = QString("robot_") + QString::number(qrand() % 1000000);
    m_robotID = XString(robotID.toStdWString());
    //m_robotID = __XT("robot");

    connect(ui->btn_send, SIGNAL(clicked(bool)), this, SLOT(slot_send_message()));

    setWindowTitle(tr("MessageForm"));

    YIMManager *im = YIMManager::CreateInstance();
    im->SetMessageCallback(this);
    im->SetContactCallback(this);
    im->SetChatRoomCallback(this);
    im->SetLoginCallback(this);
    im->SetLocationCallback(this);
    im->SetDownloadCallback(this);
    im->SetAudioPlayCallback(this);
    //im->SetFriendCallback(this);
    //im->SetUserProfileCallback(this);
    im->SetNoticeCallback(this);
    im->SetReconnectCallback(this);

    setUp();

    m_outputWindow = new OutputWindow();
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->addWidget(m_outputWindow);
    }

}

ChatWidget::~ChatWidget()
{
    YIMManager *im = YIMManager::CreateInstance();
    im->SetMessageCallback(nullptr);
    im->SetContactCallback(nullptr);
    im->SetChatRoomCallback(nullptr);
    im->SetLoginCallback(nullptr);
    im->SetLocationCallback(nullptr);
    im->SetDownloadCallback(nullptr);
    im->SetAudioPlayCallback(nullptr);
    //im->SetFriendCallback(nullptr);
    //im->SetUserProfileCallback(nullptr);
    im->SetNoticeCallback(nullptr);
    im->SetReconnectCallback(nullptr);

    foreach (QProcess *p, m_processes) {
        if (p->isOpen())
            p->kill();

        if (p) {
            delete p;
            p = nullptr;
        }
    }


}

void ChatWidget::setUp()
{
    this->setup_init();
    this->setup_roommanager();
    this->setup_message_ctrl();
    this->setup_user_ctrl();
}

void ChatWidget::setup_init()
{
    ui->lineEdit_userid->setText(QString::fromStdWString(m_userID));
    ui->lineEdit_roomid->setText(QString::fromStdWString(m_roomID));
    ui->lineEdit_roomid_2->setText(QString::fromStdWString(m_roomID));
    ui->lbl_roomid->setText(QString::fromStdWString(m_roomID));
    ui->lbl_user->setText(QString::fromStdWString(m_robotID));
    ui->lineEdit_robotname->setReadOnly(true);

    connect(ui->btn_create_robot, SIGNAL(clicked(bool)), this, SLOT(slot_create_new_robot()));
    connect(ui->btn_login, SIGNAL(clicked(bool)), this, SLOT(slot_login()));
    connect(ui->btn_joinroom, SIGNAL(clicked(bool)), this, SLOT(slot_join_room()));
    connect(ui->btn_leave, SIGNAL(clicked(bool)), this, SLOT(slot_leave_room()));
    connect(ui->btn_leaveall, SIGNAL(clicked(bool)), this, SLOT(slot_leave_all_room()));
    connect(ui->btn_logout, SIGNAL(clicked(bool)), this, SLOT(slot_logout()));
}

void ChatWidget::setup_roommanager()
{
    QStringListModel *model = new QStringListModel;
    ui->listView_rooms->setModel(model);
    m_requester = new HttpRequester();

    connect(ui->btn_roomcnt, SIGNAL(clicked(bool)), this, SLOT(slot_room_count()));
    connect(ui->btn_roomlist, SIGNAL(clicked(bool)), this, SLOT(slot_room_list()));
    connect(ui->btn_addroboter, SIGNAL(clicked(bool)), this, SLOT(slot_add_robot_in_room()));
    connect(ui->btn_deleteroboter, SIGNAL(clicked(bool)), this, SLOT(slot_delete_robot()));
    connect(m_requester, SIGNAL(data_recved(QByteArray)), this, SLOT(slot_setup_data(QByteArray)));
}

void ChatWidget::setup_message_ctrl()
{
    ui->checkBox_autogetmsg->setChecked(true);
    ui->btn_getmessages->setEnabled(false);

    QStandardItemModel *model = new QStandardItemModel;

    ui->tableView_room_history->setModel(model);
    //slot_room_history_messages();

    QStandardItemModel *privateModel = new QStandardItemModel;
    ui->tableView_private_history->setModel(privateModel);

    QStandardItemModel *history_model = new QStandardItemModel;
    ui->tableView_msgsrecord->setModel(history_model);

    connect(ui->checkBox_autogetmsg, SIGNAL(stateChanged(int))
            , this, SLOT(slot_change_state(int)));
    connect(ui->tbn_record, SIGNAL(clicked(bool)), this, SLOT(slot_start_record()));
    connect(ui->tbn_stop_and_send, SIGNAL(clicked(bool)), this, SLOT(slot_stop_and_send_voice()));
    connect(ui->btn_getmessages, SIGNAL(clicked(bool)), this, SLOT(slot_get_message()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(slot_room_history_messages()));
    connect(ui->btn_get_private_messages, SIGNAL(clicked(bool)),
            this, SLOT(slot_private_history_messages()));
    connect(ui->btn_room, SIGNAL(clicked(bool)), this, SLOT(slot_get_historyr_message()));
    connect(ui->btn_private, SIGNAL(clicked(bool)), this, SLOT(slot_get_historyp_message()));
}

void ChatWidget::setup_user_ctrl()
{
    QTimer::singleShot(60000, this, SLOT(slot_recent_contacts()));

    QStandardItemModel *contacts_model = new QStandardItemModel;
    ui->tableView_contacts->setModel(contacts_model);

    QMenu *btn_menu = new QMenu;
    btn_menu->addAction(QIcon(), QStringLiteral("屏蔽用户"), this, SLOT(slot_block_user()));
    btn_menu->addAction(QIcon(), QStringLiteral("解除屏蔽用户"), this,
                        SLOT(slot_unblock_user()));
    ui->btn_block->setMenu(btn_menu);
    connect(ui->btn_userinfo, SIGNAL(clicked(bool)), this, SLOT(slot_get_user_info()));
    connect(ui->btn_recent_contacts, SIGNAL(clicked(bool)), this,
            SLOT(slot_recent_contacts()));
}

void ChatWidget::slot_create_new_robot()
{    
    QString cmd = "./roboter.exe";
    QStringList arguments;

    QString robotID = QString("robot_") + QString::number(qrand() % 1000000);
    arguments << QString("{\"Username\":\"%1\",\"RoomID\":\"%2\",\"Passwd\":\"%3\","
                         " \"Token\":\"\",\"Appkey\":\"%4\",\"Appsecret\":\"%5\",\"IsDebug\":\"%6\"}")
            .arg(robotID)
            .arg(QString::fromStdWString(m_roomID))
            .arg(QString::fromStdWString(__XT("123456")))
            .arg(QString::fromStdWString(m_appkey))
            .arg(QString::fromStdWString(m_appSecret))
            .arg(is_testMode);
    qDebug() << arguments;

    QProcess *process = new QProcess;
    m_processes.append(process);

    connect(process, SIGNAL(finished(int)), process, SLOT(kill()));

    process->start(cmd, arguments);

//    QProcess::execute(cmd, arguments);
    m_robotID = LocalToXString(robotID.toStdString());
    //m_robotID = __XT("robot");
    ui->lineEdit_robotname->setText(QString::fromStdWString(m_robotID));
}

void ChatWidget::slot_change_state(int state)
{
    std::vector<XString> room_list;
    room_list.push_back(m_roomID);

    if (state == Qt::Checked) {
        YIMErrorcode code = YIMManager::CreateInstance()->GetMessageManager()->SetReceiveMessageSwitch(
                    room_list, true);
        if (code != YIMErrorcode_Success) {
            qDebug() << QStringLiteral("设置手动接收消息错误");
        } else {
            ui->btn_getmessages->setEnabled(false);
        }
    } else if (state == Qt::Unchecked) {
        YIMErrorcode code = YIMManager::CreateInstance()->GetMessageManager()->SetReceiveMessageSwitch(
                    room_list, false);
        if (code != YIMErrorcode_Success) {
            qDebug() << QStringLiteral("设置手动接收消息错误");
        } else {
            ui->btn_getmessages->setEnabled(true);
        }
    }
}

void ChatWidget::slot_get_message()
{
    std::vector<XString> roomids;
    roomids.push_back(m_roomID);
    YIMErrorcode code =
        YIMManager::CreateInstance()->GetMessageManager()->GetNewMessage(roomids);
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("获取消息成功...");
    }
}

void ChatWidget::slot_send_message()
{
    YIMManager *im = YIMManager::CreateInstance();

    YIMChatType chattype;
    if (ui->radioButton_private->isChecked())
        chattype = ChatType_PrivateChat;
    else if (ui->radioButton_chatroom->isChecked())
        chattype = ChatType_RoomChat;
    else if (ui->radioButton_unknown->isChecked())
        chattype = ChatType_Unknow;

    XUINT64 reqNo;

    QString message = ui->textEdit->document()->toPlainText();
    XString xmessage = UTF8TOXString(message.toStdString());

    if (chattype == ChatType_PrivateChat) {
        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(
                    m_robotID.c_str(),
                    chattype,
                    xmessage.c_str(),
                    &reqNo);
        if (code != YIMErrorcode_Success) {
            qDebug() << "Send Message failed. Error code is " << code;
        }
    } else if (chattype == ChatType_RoomChat) {
        YIMErrorcode code = im->GetMessageManager()->SendTextMessage(
                    m_roomID.c_str(),
                    chattype,
                    xmessage.c_str(),
                    &reqNo);
        if (code != YIMErrorcode_Success) {
            qDebug() << "Send Message failed. Error code is " << code;
        }
    } else if (chattype == ChatType_Multi) {

    }

    ui->textEdit->clear();

    QTextBrowser *browser;
    if (chattype == ChatType_RoomChat) {
        browser = ui->textBrowser;
    } else if (chattype == ChatType_PrivateChat) {
        browser = ui->textBrowser_2;
    }

    QFont font1, font2 = ui->textBrowser->font();
    browser->setTextColor(QColor(Qt::magenta));
    browser->setAlignment(Qt::AlignRight);
    font1.setItalic(true);
    font1.setPointSize(10);
    browser->setFont(font1);
    QString currentTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ");
    browser->append(currentTime + QString::fromStdWString(m_userID + __XT(" :")));
    browser->setFont(font2);
    browser->append(message);
}

void ChatWidget::slot_login()
{
    XString userID = LocalToXString(ui->lineEdit_userid->text().toStdString());
    if (userID.empty()) {
        userID = m_userID;
    }

    YIMErrorcode code =
            YIMManager::CreateInstance()->Login(userID.c_str(), __XT("123456"), __XT(""));
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登录失败, 错误码 ") << code ;
    }
    m_userID = userID;
}

void ChatWidget::slot_join_room()
{
    XString roomID = LocalToXString(ui->lineEdit_roomid->text().toStdString());
    if (roomID.empty()) {
        roomID = m_roomID;
    }

    YIMErrorcode code =
            YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom(roomID.c_str());
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("加入房间成功 错误码") << code;
    }
    m_roomID = roomID;
}

void ChatWidget::slot_leave_room()
{
    QString roomID = ui->lineEdit_roomid_2->text();
    XString leave_room_id = roomID.isEmpty() ? m_roomID:
                                               LocalToXString(roomID.toStdString());
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetChatRoomManager()->LeaveChatRoom(leave_room_id.c_str());
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("退出房间失败 错误码") << code;
    }
}

void ChatWidget::slot_logout()
{
    YIMErrorcode code =
            YIMManager::CreateInstance()->Logout();
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登出失败 错误码") << code;
    }
}

void ChatWidget::slot_leave_all_room()
{
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetChatRoomManager()->LeaveAllChatRooms();
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("离开所有房间失败 错误码 ") << code;
    }
}

void ChatWidget::slot_room_count()
{
    YIMManager* im = YIMManager::CreateInstance();
    YIMErrorcode code =
        im->GetChatRoomManager()->GetRoomMemberCount(m_roomID.c_str());
    if (code != YIMErrorcode_Success) {
        qDebug() << "Call GetRoomMemberCount Failed " << code;
    }else {
        qDebug() << "Call GetRoomMemberCount success " << code;
    }
}

void ChatWidget::slot_room_list()
{
    QString host = GlobalSettings::getInstance()->getHost(true);
    QString strUrl = host + "v1/im/get_room_list";

    QString body = QString("{}");

    m_requester->youme_sendData(strUrl, body);
}

void ChatWidget::slot_delete_robot()
{
    QString host = GlobalSettings::getInstance()->getHost(true);
    QString strUrl = host + "v1/im/query_im_leave_channel";

    foreach (UserRoomIDPair pair, m_user_rooms) {
        QString userID = pair.user_id;
        QString roomID = pair.room_id;
        QString body = QString("{\"UserID\": \"%1\", \"ChannelID\": \"%2\"}")
                .arg(userID)
                .arg(roomID);
        m_requester->youme_sendData(strUrl, body);
    }
    m_user_rooms.clear();
}


void ChatWidget::slot_add_robot_in_room()
{
    //用restapi像房间里加人
    //To Do
    QString host = GlobalSettings::getInstance()->getHost(true);
    QString strUrl = host + "v1/im/query_im_enter_channel";
    int number = ui->spin_robots->value();
    for (int i = 1; i <= number; ++i) {
        QString userID = QString("robot_") + QString::number(10000 + i);
        QString body = QString("{\"UserID\": \"%1\", \"ChannelID\": \"%2\"}")
                .arg(userID)
                .arg(QString::fromStdWString(m_roomID));
        m_requester->youme_sendData(strUrl, body);
        UserRoomIDPair pitem;
        pitem.room_id = QString::fromStdWString(m_roomID);
        pitem.user_id = userID;
        m_user_rooms.append(pitem);
    }
}


void ChatWidget::slot_setup_data(QByteArray data)
{
    QJsonObject object = QJsonDocument::fromJson(data).object();
   // int room_cnt = object["room_cnt"].toInt();
    QJsonArray room_list = object["room_list"].toArray();
    QStringList ls;
    int n = 1;
    foreach (QJsonValue member, room_list) {
        QJsonObject objectItem = member.toObject();
        ls << QString::number(n++) + " " + objectItem["room_name"].toString();

    }

    //ui->listView_rooms->append(ls.join(","));
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->listView_rooms->model());

    if (!model)
        return;
    model->setStringList(ls);

}

void ChatWidget::slot_room_history_messages()
{
    YIMErrorcode code =
        YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(
                m_roomID.c_str(), 200, 0);
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("查询历史消息记录失败, 错误码") << code;
    }
}

void ChatWidget::slot_private_history_messages()
{
    XUINT64 imin = 0;
    XUINT64 imax = 0x7fffffffffffffff;
//    YIMErrorcode code =
//            YIMManager::CreateInstance()->GetMessageManager()->QuerySessionHistoryMsgFromServer(
//                m_userID.c_str(), imin, imax, 0, 20);
//    if (code != YIMErrorcode_Success) {
//        qDebug() << QStringLiteral("查询私聊历史消息记录失败, 错误码") << code;
//    }
}

void ChatWidget::slot_get_historyp_message()
{
    YIMErrorcode code1 =
            YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage(
                m_roomID.c_str(), ChatType_RoomChat);
    if (code1 != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("查询历史房间消息记录失败, 错误码") << code1;
    }
}

void ChatWidget::slot_get_historyr_message()
{
    YIMErrorcode code2 =
            YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage(
                m_userID.c_str(), ChatType_RoomChat);
    if (code2 != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("查询历史私聊消息记录失败, 错误码") << code2;
    }
}

void ChatWidget::OnRecvMessage(std::shared_ptr<IYIMMessage> message)
{
    XUINT64 id = message->GetMessageID();
    qDebug() << QStringLiteral("收到消息id") << id;
    YIMMessageBodyType messageType = message->GetMessageBody()->GetMessageType();
    XString sendID = message->GetSenderID();
    XString receivID = message->GetReceiveID();
    YIMChatType type = message->GetChatType();
    unsigned int msg_time = message->GetCreateTime();
    QString currentTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    int delay = QTime::currentTime().elapsed() - msg_time;

    QTextBrowser *browser = ui->textBrowser;
    if (type == ChatType_PrivateChat)
        browser = ui->textBrowser_2;
    else if (type == ChatType_RoomChat)
        browser = ui->textBrowser;

    browser->setAlignment(Qt::AlignLeft);
    browser->append(currentTime);
    browser->setTextColor(QColor(Qt::blue));
    browser->append(QString::fromStdWString(sendID + __XT(" ") + receivID + __XT(":")));
    switch (messageType)
    {
    case MessageBodyType_TXT:
        {
            IYIMMessageBodyText* text =
                static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            QString msgcontent = QString::fromStdWString(XString(text->GetMessageContent()));
            browser->setTextColor(QColor(Qt::black));
            browser->append(msgcontent);
        }
        break;
    case MessageBodyType_CustomMesssage:
        {
            qDebug() << QStringLiteral("收到二进制消息");
            IYIMMessageBodyCustom *customtext =
                    static_cast<IYIMMessageBodyCustom*>(message->GetMessageBody());
            if (!customtext)
                break;
            std::string custom_str = customtext->GetCustomMessage();
            QByteArray text = QByteArray::fromStdString(custom_str).toBase64();
            browser->setTextColor(QColor(Qt::black));
            browser->append(QString(text.toBase64()));
            if (type == ChatType_PrivateChat) {
                browser->setTextColor(QColor(Qt::red));
                browser->append("type: " + QString::number(delay) + " delay: " + delay);
            }
        }
        break;
    case MessageBodyType_Gift:
        {
            IYIMMessageGift *gift =
                    static_cast<IYIMMessageGift*>(message->GetMessageBody());
            if (!gift)
                break;
            const XCHAR *anchor = gift->GetAnchor();
            int cnt = gift->GetGiftCount();
            const XCHAR *param = gift->GetExtraParam();
            int id = gift->GetGiftID();
            QJsonObject jsonObject;
            jsonObject["anchor"] = QString::fromStdWString(XString(anchor));
            jsonObject["giftcnt"] = cnt;
            jsonObject["extparam"] = QString::fromStdWString(XString(param));
            jsonObject["id"] = id;
            QJsonDocument doc;
            doc.setObject(jsonObject);
            QByteArray json = doc.toJson();
            browser->setTextColor(QColor(Qt::green));
            browser->append(QString(json));
        }
        break;
    case MessageBodyType_Voice:
        {
            IYIMMessageBodyAudio *audio =
                    static_cast<IYIMMessageBodyAudio*>(message->GetMessageBody());
            if (!audio)
                break;
            unsigned int interval = audio->GetAudioTime();
            unsigned int filesize = audio->GetFileSize();
            const XCHAR *path = audio->GetLocalPath();
            const XCHAR *text = audio->GetText();
            QVariantMap voice_map;
            voice_map["audiotime"] = interval;
            voice_map["filesize"] = filesize;
            voice_map["text"] = QString::fromStdWString(XString(text));
            voice_map["path"] = QString::fromStdWString(XString(path));
            voice_map["extraParam"] = QString::fromStdWString(XString(audio->GetExtraParam()));

            QJsonDocument doc;
            doc.setObject(QJsonObject::fromVariantMap(voice_map));
            QByteArray json = doc.toJson();
            browser->setTextColor(QColor(Qt::darkYellow));
            browser->append(QString(json));
        }
        break;
    case MessageBodyType_Image:
    case MessageBodyType_Emoji:
    case MessageBodyType_File:
    case MessageBodyType_Video:
    case MessageBodyType_Unknow:
        break;
    default:
        break;
    }
}

void ChatWidget::OnStopAudioSpeechStatus(YIMErrorcode errorcode,
                                                    std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("回调失败，错误码: ") << errorcode;
    }
    qDebug() << "Audio time: " << audioSpeechInfo->GetAudioTime();
    qDebug() << "Download URL: " << QString::fromStdWString(audioSpeechInfo->GetDownloadURL());
    qDebug() << "Local Path: " << QString::fromStdWString(XString(audioSpeechInfo->GetLocalPath()));
    qDebug() << "File size: " << audioSpeechInfo->GetFileSize();
    qDebug() << "RequestID: " << audioSpeechInfo->GetRequestID();
    qDebug() << "Text: " << QString::fromStdWString(audioSpeechInfo->GetText());
}

void ChatWidget::OnStartSendAudioMessage(XUINT64 requestID,
                                                    YIMErrorcode errorcode,
                                                    const XString &text,
                                                    const XString &audioPath,
                                                    unsigned int audioTime)
{

}

void ChatWidget::OnSendMessageStatus(XUINT64 requestID,
                                                YIMErrorcode errorcode,
                                                unsigned int sendTime,
                                                bool isForbidRoom,
                                                int reasonType, XUINT64 forbidEndTime)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("回调失败, 错误码: ") << errorcode;
    }
    qDebug() << QStringLiteral("消息发送成功");
    QVariantMap messageMap;
    messageMap["requestID"] = requestID;
    messageMap["sendTime"] = sendTime;
    messageMap["isForbid"] = isForbidRoom;
    messageMap["forbidEndTime"] = forbidEndTime;
    messageMap["resonType"] = reasonType;

    QJsonObject::fromVariantMap(messageMap);
}

void ChatWidget::OnLogin(YIMErrorcode errorcode, const XString &userID)
{
    if (errorcode == YIMErrorcode_Success)
    qDebug() << "Login success!";
}

void ChatWidget::OnLogout(YIMErrorcode errorcode)
{
    qDebug() << "Logout " << errorcode;
}

//void ChatWidget::OnLeaveAllChatRooms(YIMErrorcode errorcode)
//{

//}

void ChatWidget::OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << QStringLiteral("加入房间成功 ") << XStringToLocal(chatRoomID).c_str();
    }
}

void ChatWidget::OnUserJoinChatRoom(const XString &chatRoomID, const XString &userID)
{
    qDebug() << QStringLiteral("加入房间: ") << XStringToLocal(chatRoomID).c_str()
             << QStringLiteral("用户ID: ") << XStringToLocal(userID).c_str();
}

void ChatWidget::OnUserLeaveChatRoom(const XString &chatRoomID, const XString &userID)
{
    qDebug() << QStringLiteral("用户: ") << XStringToLocal(userID).c_str()
             << QStringLiteral("房间ID: ") << XStringToLocal(chatRoomID).c_str();
}

void ChatWidget::OnLeaveChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << QStringLiteral("离开房间成功: ") << XStringToLocal(chatRoomID).c_str();
    }
    else {
        qDebug() << QStringLiteral("离开房间失败: ") << errorcode;

    }
}

void ChatWidget::OnGetRoomMemberCount(YIMErrorcode errorcode,
                                             const XString &chatRoomID,
                                             unsigned int count)
{
    if (errorcode != YIMErrorcode_Success)
        qDebug() << "GetRoomMemberCount callback error , code is" << errorcode;

    qDebug() << "Room " << XStringToLocal(chatRoomID).c_str() << " number :" << count;

    ui->lineEdit_room_cnt->setText(QString::number(count));
}

void ChatWidget::OnReceiveMessageNotify(YIMChatType chatType, const XString &targetID)
{
    qDebug() << XStringToLocal(targetID).c_str();
    if (chatType == ChatType_RoomChat) {
        qDebug() << QString("Recv 1 message");
    } else if (chatType == ChatType_PrivateChat) {
        qDebug() << QString("Recv 1 private message");
    }
}

void ChatWidget::OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode,
                                                     const XString &roomID,
                                                     int remain,
                                                     std::list<std::shared_ptr<IYIMMessage> > &messageList)
{
    Q_UNUSED(roomID)
    Q_UNUSED(remain)
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("查询消息失败");
    }
    QStandardItemModel * model = qobject_cast<QStandardItemModel*>(
                ui->tableView_room_history->model());
    if (!model)
        return;
    //model->clear();
    QStringList title;
    title << "id" << "create_time" << "text" << "recvid" << "sendid" << "chattype";
    model->setHorizontalHeaderLabels(title);
    for (auto it = messageList.begin(); it != messageList.end(); ++it) {
        std::shared_ptr<IYIMMessage> message = *(it);
        unsigned int ctime = message->GetCreateTime();
        IYIMMessageBodyBase *body = message->GetMessageBody();
        IYIMMessageBodyText *text =  static_cast<IYIMMessageBodyText*>(body);
        if (!text)
            continue;

        QString recvid = QString::fromStdWString(message->GetReceiveID());
        QString sendid = QString::fromStdWString(message->GetSenderID());
        YIMChatType chattype = message->GetChatType();

        QStandardItem *item0 = new QStandardItem(QString::number(message->GetMessageID()));
        QStandardItem *item1 = new QStandardItem(QString::number(ctime));
        QStandardItem *item2 = new QStandardItem(QString::fromStdWString(XString(text->GetMessageContent())));
        QStandardItem *item3 = new QStandardItem(recvid);
        QStandardItem *item4 = new QStandardItem(sendid);
        QStandardItem *item5 = new QStandardItem(QString::number(chattype));

        QList<QStandardItem*>items;
        items << item0 << item1 << item2 << item3 << item4 << item5;
        model->appendRow(items);
    }

}

void ChatWidget::OnQuerySessionHistoryMsgFromServer(YIMErrorcode errorcode,
                                                    const XString &userID,
                                                    std::list<std::shared_ptr<IYIMMessage> > &messageList)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("查询消息失败");
    }
    Q_UNUSED(userID)

    QStandardItemModel *model
            = qobject_cast<QStandardItemModel*>(ui->tableView_private_history->model());
    if (!model)
        return;
    model->setHorizontalHeaderLabels(QStringList() <<
                                     "msg_id" <<
                                     "send_id" <<
                                     "recv_id" <<
                                     "content" <<
                                     "is_read" <<
                                     "date");
    for (auto it = messageList.begin(); it != messageList.end();
         ++it)
    {
        std::shared_ptr<IYIMMessage> message = *(it);
        message->GetMessageID();
        QString sendid = QString::fromStdWString(message->GetSenderID());
        QString recvid = QString::fromStdWString(message->GetReceiveID());
        bool isread = message->IsRead();
        QString d =
                QDateTime::fromTime_t(message->GetCreateTime()).toString("yyyy-MM-dd hh:mm:ss");
        QStandardItem *item0 = new QStandardItem(QString::number(message->GetMessageID()));
        QStandardItem *item1 = new QStandardItem(sendid);
        QStandardItem *item2 = new QStandardItem(recvid);

        IYIMMessageBodyText *text =
                static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
        QStandardItem *item3 = new QStandardItem();
        if (text) {
            item3->setText(QString::fromStdWString(XString(text->GetMessageContent())));
        } else {
            item3->setText(" ");
        }
        QStandardItem *item4 = new QStandardItem();
        if (isread) {
            item4->setText("yes");
        } else {
            item4->setText("no");
        }
        QStandardItem *item5 = new QStandardItem(d);
        QList<QStandardItem*> items;
        items << item0 << item1 << item2 << item3 << item4 << item5;
        model->appendRow(items);
    }
}

void ChatWidget::OnGetRecentContacts(YIMErrorcode errorcode,
                                     std::list<std::shared_ptr<IYIMContactsMessageInfo> > &contactList)
{
    if (errorcode != 0) {
        qDebug() << QStringLiteral("获取最近联系人失败") << errorcode;
    }
    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->tableView_contacts->model());

    if (!model)
        return;
    model->clear();

    model->setHorizontalHeaderLabels(QStringList()
                                     << "contact_id"
                                     << "date"
                                     << "noread_num"
                                     << "content");

    for (auto it = contactList.begin(); it != contactList.end(); ++it) {
        std::shared_ptr<IYIMContactsMessageInfo> info = *it;
        QStandardItem *item0 = new QStandardItem(QString::fromStdWString(XString(info->GetContactID())));

        QString date = QDateTime::fromTime_t(info->GetCreateTime()).toString("yy/MM/dd hh:mm:ss");
        QStandardItem *item1 = new QStandardItem(date);

        QStandardItem *item2 = new QStandardItem(info->GetNotReadMsgNum());

        QStandardItem *item3 = new QStandardItem(QString::fromStdWString(XString(info->GetMessageContent())));
        model->appendRow(QList<QStandardItem*>()
                         << item0
                         << item1
                         << item2
                         << item3);

    }
}

void ChatWidget::OnQueryUserStatus(YIMErrorcode errorcode, const XString &userID, YIMUserStatus status)
{
    ui->textEdit_userinfo->clear();
    if (status == STATUS_ONLINE) {
        ui->textEdit_userinfo->append(QString::fromStdWString(userID) + " online");
    } else if (status == STATUS_OFFLINE) {
        ui->textEdit_userinfo->append(QString::fromStdWString(userID) + " offline");
    }
}

void ChatWidget::OnGetBlockUsers(YIMErrorcode errorcode, std::list<XString> userList)
{
    if (errorcode != 0) {
        return;
    }
    QStringList users;
    for (auto it = userList.begin(); it != userList.end(); ++it) {
        users << QString::fromStdWString(*it);
    }
    ui->textEdit_userinfo->clear();
    ui->textEdit_userinfo->append(users.join(","));
}

void ChatWidget::OnGetDistance(YIMErrorcode errorcode, const XString &userID, unsigned int distance)
{

}

void ChatWidget::OnGetUserInfo(YIMErrorcode errorcode,
                               const XString &userID, const XString &userInfo)
{
    if (errorcode != 0) {
        qDebug() << QStringLiteral("获取用户信息失败, 错误码") << errorcode
                 << QStringLiteral("用户id : ") << XStringToLocal(userID).c_str();
    }
    ui->textEdit_userinfo->append(QString::fromStdWString(userInfo));
}

void ChatWidget::OnQueryHistoryMessage(YIMErrorcode errorcode,
                                       const XString &targetID, int remain,
                                       std::list<std::shared_ptr<IYIMMessage> > messageList)
{
    Q_UNUSED(targetID)
    Q_UNUSED(remain)
    if (errorcode != 0) {
        qDebug() << QStringLiteral("查询消息失败, 错误码") << errorcode;
    }
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->tableView_msgsrecord->model());
    if (!model)
        return;
    model->setHorizontalHeaderLabels(QStringList() << "msgid"
                                     << "sendid");
    for (auto it = messageList.begin(); it != messageList.end(); ++it) {
        std::shared_ptr<IYIMMessage> message = (*it);
        QStandardItem *item0 = new QStandardItem(QString::number(message->GetMessageID()));

        QStandardItem *item1 = new QStandardItem(QString::fromStdWString(XString(message->GetSenderID())));
        QStandardItem *item2 = new QStandardItem(QString::fromStdWString(XString(message->GetReceiveID())));
        QString date = QDateTime::fromTime_t(message->GetCreateTime()).toString("yy/MM/dd hh:mm:ss");
        QStandardItem *item3 = new QStandardItem(date);
        IYIMMessageBodyText *text = static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
        if (!text)
            continue;

        QStandardItem *item4 = new QStandardItem(QString::number(message->IsRead()));

        QList<QStandardItem*> items;
        items << item0 << item1 << item2 << item3 << item4;
        model->appendRow(items);
    }
}

void ChatWidget::OnGetNearbyObjects(YIMErrorcode errorcode,
                                    std::list<std::shared_ptr<RelativeLocation> > neighbourList,
                                    unsigned int startDistance, unsigned int endDistance)
{

}

void ChatWidget::OnGetRecognizeSpeechText(XUINT64 requestID, YIMErrorcode errorcode, const XString &text)
{

}

void ChatWidget::OnGetMicrophoneStatus(AudioDeviceStatus status)
{

}

//slots functions
void ChatWidget::slot_start_record()
{
    YIMManager *im = YIMManager::CreateInstance();

    YIMChatType chattype;

    if (ui->radioButton_chatroom->isChecked())
        chattype = ChatType_RoomChat;
    else if (ui->radioButton_private->isChecked())
        chattype = ChatType_PrivateChat;
    else
        chattype = ChatType_Unknow;

    if (ui->checkBox_withrec->isChecked()) {
        YIMErrorcode code;
        if (chattype == ChatType_RoomChat) {
            code = im->GetMessageManager()->SendAudioMessage(m_roomID.c_str(),
                                                                          chattype,
                                                                          nullptr);
        }

        if (chattype == ChatType_PrivateChat) {
            code = im->GetMessageManager()->SendAudioMessage(m_robotID.c_str(),
                                                             chattype,
                                                             nullptr);
        }

        if (code != YIMErrorcode_Success) {
            qDebug() << QStringLiteral("启动录音失败");
        } else {
            ui->tbn_record->setChecked(true);
        }
    } else {
        YIMErrorcode code;

        if (chattype == ChatType_RoomChat) {
            code = im->GetMessageManager()->SendOnlyAudioMessage(m_roomID.c_str(),
                                                                 chattype,
                                                                 nullptr);
        }
        if (chattype == ChatType_PrivateChat) {
            code = im->GetMessageManager()->SendOnlyAudioMessage(m_robotID.c_str(),
                                                                 chattype,
                                                                 nullptr);
        }

        if (code != YIMErrorcode_Success) {
            qDebug() << QStringLiteral("启动录音失败");
        } else {
            ui->tbn_record->setChecked(true);
        }
    }

}

void ChatWidget::slot_stop_and_send_voice()
{
    YIMManager *im = YIMManager::CreateInstance();
    YIMErrorcode code = im->GetMessageManager()->StopAudioMessage(__XT("youmetest"));
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("录音发送失败, 错误码 ") << code;
    }
}

void ChatWidget::slot_recent_contacts()
{
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetRecentContacts();
    if (code != 0) {
        qDebug() << QStringLiteral("获取联系人失败 ") << code;
    }
}

void ChatWidget::slot_get_user_info()
{
    QString userid = ui->lineEdit_userid_2->text();

    YIMErrorcode code;
    QString userinfo = QString(
    "{\"nickname\":\"%1\",\"server_area_id\":\"\",\"server_area\":\"\",\"location_id\":\"\","
    "\"location\":\"\",\"level\":"",\"vip_level\":\"\",\"platform_id\":\"\",\"platform\":\"\"}").arg(userid);

    YIMManager::CreateInstance()->SetUserInfo(LocalToXString(userinfo.toStdString()).c_str());

    if (userid.isEmpty()) {
        code = YIMManager::CreateInstance()->GetUserInfo(m_userID.c_str());
    } else {
        code = YIMManager::CreateInstance()->GetUserInfo(LocalToXString(userid.toStdString()).c_str());
    }
}

void ChatWidget::slot_block_user()
{
    QString userName = ui->lineEdit_username->text();
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetMessageManager()->BlockUser(
                LocalToXString(userName.toStdString()).c_str(), true);
    if (code != 0) {
        qDebug() << QStringLiteral("屏蔽用户失败, 错误码 ") << code;
    }
}

void ChatWidget::slot_unblock_user()
{
    QString userName = ui->lineEdit_username->text();
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetMessageManager()->BlockUser(
                LocalToXString(userName.toStdString()).c_str(), false);
    if (code != 0) {
        qDebug() << QStringLiteral("屏蔽用户失败, 错误码 ") << code;
    }
}

void ChatWidget::slot_get_block_users()
{
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetMessageManager()->GetBlockUsers();
    if (code != 0) {
        qDebug() << QStringLiteral("获取屏蔽用户失败, 错误码 ") << code;
    }
}
