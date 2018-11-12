#include "roommanager.h"
#include "YIMPlatformDefine.h"
#include "globalsetting.h"
#include "ui_roommanager.h"
#include "httprequester.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QDebug>
#include <QProcess>


RoomManagerDialog::RoomManagerDialog(XString roomID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomManager)
  ,m_roomID(roomID)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("房间管理"));
    m_requester = new HttpRequester();

    ui->label_roomNum->setText(QString::fromStdWString(roomID));
    connect(ui->btn_room_cnt, SIGNAL(clicked()), this, SLOT(slot_room_cnt()));
    connect(ui->btn_add_robots, SIGNAL(clicked()), this, SLOT(slot_add_robot_in_room()));
    connect(ui->btn_del_robots,SIGNAL(clicked()), this, SLOT(slot_kickoff()));
    connect(ui->btn_all_rooms, SIGNAL(clicked()), this, SLOT(slot_room_list()));
    connect(m_requester, SIGNAL(data_recved(QByteArray)),
            this, SLOT(slot_setup_data(QByteArray)));
    YIMManager::CreateInstance()->SetChatRoomCallback(this);
}

void RoomManagerDialog::setUp()
{
    m_process = new QProcess(this);

    m_process->start("");
}

void RoomManagerDialog::slot_room_cnt()
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

void RoomManagerDialog::OnGetRoomMemberCount(YIMErrorcode errorcode,
                                             const XString &chatRoomID,
                                             unsigned int count)
{
    if (errorcode != YIMErrorcode_Success)
        qDebug() << "GetRoomMemberCount callback error , code is" << errorcode;

    qDebug() << "Room " << XStringToLocal(chatRoomID).c_str() << " number :" << count;

    ui->lineedit_room_cnt->setText(QString::number(count));
}

void RoomManagerDialog::slot_kickoff()
{
    //踢人踢出用户
}
void RoomManagerDialog::slot_add_robot_in_room()
{
    //用restapi像房间里加人
    //To Do

    QString host = GlobalSettings::getInstance()->getHost(true);
    QString strUrl = host + "v1/im/query_im_enter_channel";

    int number = ui->spinBox_add->value();
    for (int i = 1; i <= number; ++i) {
        QString userID = QString("robot_") + QString::number(10000 + i);
        QString body = QString("{\"UserID\": \"%1\", \"ChannelID\": \"%2\"}")
                .arg(userID)
                .arg(QString::fromStdWString(m_roomID));
        m_requester->youme_sendData(strUrl, body);
    }
}

void RoomManagerDialog::slot_room_list()
{
    QString host = GlobalSettings::getInstance()->getHost();
    QString strUrl = host + "v1/im/get_room_list";

    QString body = QString("{}");

    m_requester->youme_sendData(strUrl, body);
}

void RoomManagerDialog::slot_setup_data(QByteArray data)
{
    QJsonObject object = QJsonDocument::fromJson(data).object();
    int room_cnt = object["room_cnt"].toInt();
    QJsonArray room_list = object["room_list"].toArray();
    QStringList ls;
    foreach (QJsonValue member, room_list) {
        QJsonObject objectItem = member.toObject();
        ls << objectItem["room_name"].toString();

    }
    ui->textEdit_room_list->append(QString::number(room_cnt));
    ui->textEdit_room_list->append(ls.join(","));
}

RoomManagerDialog::~RoomManagerDialog()
{
    delete ui;
    delete m_requester;
    m_requester = nullptr;

    YIMManager::CreateInstance()->SetChatRoomCallback(nullptr);
}
