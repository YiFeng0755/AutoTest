#include "stabletestdialog.h"
#include "ui_stableform.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QDateTime>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QTimer>
#include <QReadWriteLock>
#include <QMessageBox>


#define LOCK_INTERFACE(strname) \
{ \
    QMutexLocker locker(&m_mutex);\
    m_stat_map[strname]++;\
}

StableTestDialog::StableTestDialog(QWidget *parent)
    :QDialog(parent), ui(new Ui::StableTestForm)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("StableDialog"));
    setup();
    createNewRoboter();
}

StableTestDialog::~StableTestDialog()
{
    if (m_process) {
        delete m_process;
        m_process = nullptr;
    }

    m_stop = true;
//    TODO
//    这里有个bug 如果线程还在跑就退出 就会crash
//    所以应该在这里让线程等一等，改点m_stop的值 join一下
}

void StableTestDialog::setup()
{
    connect(ui->btn_other, SIGNAL(clicked(bool)), this, SLOT(slot_other_interface()));
    connect(ui->btn_stop, SIGNAL(clicked(bool)), this, SLOT(slot_stop_all_threads()));
    connect(ui->btn_voice, SIGNAL(clicked(bool)), this, SLOT(slot_voice_interface()));
    connect(ui->btn_text, SIGNAL(clicked(bool)), this, SLOT(slot_text_interface()));
    connect(ui->btn_export, SIGNAL(clicked(bool)), this, SLOT(slot_exportData()));

    QString user = QString("StabletestUser") + QString::number(qrand() % 100000);
    m_userID = UTF8TOXString(user.toStdString()).c_str();
    QString robotUser = QString("robot_stestf_") + QString::number(qrand() % 100000 + 500000);
    m_robotID = UTF8TOXString(robotUser.toStdString()).c_str();
    QString roomid = QString("201") + QString::number(qrand() % 10000 + 1000);
    m_roomID = UTF8TOXString(roomid.toStdString()).c_str();

    /*debug*/
    m_roomID = __XT("2013333");
    m_process = new QProcess(this);

    YIMErrorcode code =
            YIMManager::CreateInstance()->Login(m_userID.c_str(),
                                                __XT("123456"),
                                                __XT(""));
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登录失败，错误码:");
    }

    YIMManager::CreateInstance()->SetLoginCallback(this);
    YIMManager::CreateInstance()->SetChatRoomCallback(this);
    YIMManager::CreateInstance()->SetMessageCallback(this);
    YIMManager::CreateInstance()->SetAudioPlayCallback(this);
    YIMManager::CreateInstance()->SetFriendCallback(this); //2.2.0才有
    YIMManager::CreateInstance()->SetDownloadCallback(this);
    YIMManager::CreateInstance()->SetLocationCallback(this);
    YIMManager::CreateInstance()->SetReconnectCallback(this);
    YIMManager::CreateInstance()->SetUserProfileCallback(this);

    QStandardItemModel *model = new QStandardItemModel;
    ui->tableView->setModel(model);

    createNewRoboter();
}

void StableTestDialog::createNewRoboter()
{
    QString cmd = "./roboter.exe";
    QStringList arguments;

    QString robotID = QString("robot_") + QString::number(qrand() % 1000000);
    arguments << QString("{\"Username\":\"%1\",\"RoomID\":\"%2\",\"Passwd\":\"%3\",\"Token\":\"\"}")
            .arg(robotID)
            .arg(QString::fromStdWString(m_roomID))
            .arg(QString::fromStdWString(__XT("123456"))
            .arg(QString::fromStdWString(__XT(""))));
    qDebug() << arguments;
    m_process->start(cmd, arguments);
    m_robotID = LocalToXString(robotID.toStdString());
    //m_robotID = __XT("robot");
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_updateStatData()));
    m_timer->start(10000);
}

void StableTestDialog::slot_other_interface()
{

}

void StableTestDialog::slot_text_interface()
{
    m_stop = false;
    std::thread t1 = std::thread([this](){
       while (true) {
           if (m_stop) {
               break;
           }

           XUINT64 reqNo;

           YIMErrorcode code1 = YIMManager::CreateInstance()->GetMessageManager()
                   ->SendTextMessage(0, ChatType_RoomChat, __XT(""), &reqNo);
           LOCK_INTERFACE("SendTextMessage")
           qDebug() << QStringLiteral("SendTextMessage ") << code1;
           YIMErrorcode code2 = YIMManager::CreateInstance()->GetMessageManager()
                   ->SendTextMessage(m_roomID.c_str(), ChatType_RoomChat,
                                     __XT("+=\sdfbfg/.2432kksedf"), nullptr);
           LOCK_INTERFACE("SendTextMessage")
           qDebug() << QStringLiteral("SendTextMessage ") << code2;
           YIMErrorcode code3 = YIMManager::CreateInstance()->GetMessageManager()
                   ->SendTextMessage(m_roomID.c_str(),
                                     ChatType_RoomChat,
                                     __XT(""), &reqNo);
           LOCK_INTERFACE("SendTextMessage")
           qDebug() << QStringLiteral("SendTextMessage ") << code3;
           YIMErrorcode code4 = YIMManager::CreateInstance()->GetMessageManager()
                   ->SendTextMessage(nullptr, ChatType_PrivateChat, __XT("c%sdf.."),
                                     &reqNo);
           LOCK_INTERFACE("SendTextMessage")
           qDebug() << QStringLiteral("SendTextMessage ") << code4;
           YIMErrorcode code5 = YIMManager::CreateInstance()->GetMessageManager()
                   ->SendTextMessage(m_roomID.c_str(), ChatType_PrivateChat,
                                     __XT(""), &reqNo);
           LOCK_INTERFACE("SendTextMessage")
           qDebug() << QStringLiteral("SendTextMessage ") << code5;
           YIMErrorcode code6 = YIMManager::CreateInstance()->GetMessageManager()
                   ->SendTextMessage(m_roomID.c_str(), ChatType_RoomChat, __XT(""), &reqNo);

           LOCK_INTERFACE("SendTextMessage")

           qDebug() << QStringLiteral("=================") << code6;

           std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 500));
       }
    });
    t1.detach();

    std::thread t2 = std::thread([this](){
        while (true) {
            if (m_stop)
                break;
            YIMErrorcode code1 = YIMManager::CreateInstance()->GetMessageManager()->Accusation(m_userID.c_str()
                                                                                               , ChatType_RoomChat
                                                                                                       , 1
                                                                                               , __XT("..."), __XT("abc"));
            LOCK_INTERFACE("Accusation")
            qDebug() << QStringLiteral("Accusation : ") << code1;
            YIMErrorcode code2 =
                    YIMManager::CreateInstance()->GetMessageManager()->BlockUser(m_robotID.c_str(), true);

            qDebug() << QStringLiteral("BlockUser : ") << code2;

            LOCK_INTERFACE("BlockUser")
            YIMErrorcode code3 =
                YIMManager::CreateInstance()->GetMessageManager()->BlockUser(m_robotID.c_str(), false);
            LOCK_INTERFACE("BlockUser")
            qDebug() << QStringLiteral("BlockUser: ") << code3;

            YIMErrorcode code4 =
                YIMManager::CreateInstance()->GetMessageManager()->UnBlockAllUser();

            LOCK_INTERFACE("UnBlockAllUser")
            qDebug() << QStringLiteral("UnBlockAllUser: ") << code4;
            std::this_thread::sleep_for(std::chrono::seconds(qrand() % 10));
        }
    });
    t2.detach();

    std::thread t3 = std::thread([this](){
        while (true) {
            if (m_stop)
                break;

            char bytes[1024];
            memset(bytes, '0', 1024);
            XUINT64 reqNo;
            YIMManager::CreateInstance()->GetMessageManager()->SendCustomMessage(m_roomID.c_str()
                                                                                 , ChatType_RoomChat
                                                                                 , bytes, 100
                                                                                 , &reqNo);
            LOCK_INTERFACE("SendCustomMessage")
            YIMManager::CreateInstance()->GetMessageManager()->SendCustomMessage(nullptr,
                                                                                 ChatType_RoomChat,
                                                                                 bytes, 10,
                                                                                 &reqNo);
            LOCK_INTERFACE("SendCustomMessage")
            YIMManager::CreateInstance()->GetMessageManager()->SendCustomMessage(m_roomID.c_str(), ChatType_RoomChat
                                                                                 ,bytes, 100,
                                                                                 &reqNo);
            LOCK_INTERFACE("SendCustomMessage")
            std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 1000));
        }
    });

    t3.detach();

    std::thread t4 = std::thread([this](){
        //开关接口的控制
       while (true) {
           if (m_stop)
               break;
           YIMManager::CreateInstance()->GetMessageManager()->PauseReceiveMessage(qrand() % 2);
           LOCK_INTERFACE("PauseReceiveMessage")
           std::vector<XString> rooms;
           rooms.push_back(m_roomID);
           YIMManager::CreateInstance()->GetMessageManager()->SetReceiveMessageSwitch(rooms, qrand() % 2);

           LOCK_INTERFACE("SetReceiveMessageSwitch")
           YIMManager::CreateInstance()->GetMessageManager()->SetDownloadAudioMessageSwitch(qrand() % 2);

           //YIMManager::CreateInstance()->GetMessageManager()->SetVoiceMsgPlayed();
           LOCK_INTERFACE("SetDownloadAudioMessageSwitch")
           std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 5000));
       }
    });
    t4.detach();

    std::thread t5 = std::thread([this](){
        while (true) {
            if (m_stop) {
                break;
            }

            XUINT64 reqNo;
            YIMManager::CreateInstance()->GetMessageManager()->SendGift(nullptr,
                                                                        m_roomID.c_str(), 0, 10, "aaa", &reqNo);
            LOCK_INTERFACE("SendGift")
            YIMManager::CreateInstance()->GetMessageManager()->SendGift(m_robotID.c_str(), m_roomID.c_str(),
                                                                        10001,
                                                                        10, "1234", &reqNo);
            LOCK_INTERFACE("SendGift")
            YIMManager::CreateInstance()->GetMessageManager()->SendGift(m_robotID.c_str(), m_roomID.c_str(),
                                                                        100, 1, "{\"key\":\"test\"}"
                                                                        , &reqNo);
            LOCK_INTERFACE("SendGift")
            YIMManager::CreateInstance()->GetMessageManager()->SendGift(m_robotID.c_str(),
                                                                        m_roomID.c_str(),
                                                                        10001, 10, nullptr, &reqNo);
            LOCK_INTERFACE("SendGift")
            YIMManager::CreateInstance()->GetMessageManager()->SendGift(m_robotID.c_str(), nullptr,
                                                                        1000, 1, "", &reqNo);
            LOCK_INTERFACE("SendGift")

            std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 1000));
        }
    });
    t5.detach();

    std::thread t6 = std::thread([this](){

        while (true) {
            if (m_stop) {
                break;
            }
            std::vector<XString> users;
            YIMManager::CreateInstance()->GetMessageManager()->MultiSendTextMessage(users, __XT("test string"));
            LOCK_INTERFACE("MultiSendTextMessage")
            users.push_back(m_robotID);
            YIMManager::CreateInstance()->GetMessageManager()->MultiSendTextMessage(users, __XT("test string"));
            users.push_back(__XT("2013456"));
            LOCK_INTERFACE("MultiSendTextMessage")
            YIMManager::CreateInstance()->GetMessageManager()->MultiSendTextMessage(users, __XT("test string s"));
            LOCK_INTERFACE("MultiSendTextMessage")
            users.push_back(__XT("abc1234"));
            YIMManager::CreateInstance()->GetMessageManager()->MultiSendTextMessage(users, __XT("test string t"));
            LOCK_INTERFACE("MultiSendTextMessage")
            YIMManager::CreateInstance()->GetMessageManager()->MultiSendTextMessage(users, nullptr);
            LOCK_INTERFACE("MultiSendTextMessage")
            std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 1000));
        }
    });
    t6.detach();

    std::thread t7 = std::thread([this]() {
       while (true) {
           if (m_stop)
               break;
           YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage(m_robotID.c_str(),
                                                                                  ChatType_RoomChat);
           LOCK_INTERFACE("QueryHistoryMessage")
           YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage(nullptr, ChatType_PrivateChat);
           LOCK_INTERFACE("QueryHistoryMessage")
           YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage(m_robotID.c_str(),
                                                                                  ChatType_PrivateChat, 0, 100, 1);
           LOCK_INTERFACE("QueryHistoryMessage")
           YIMManager::CreateInstance()->GetMessageManager()->QueryHistoryMessage(m_roomID.c_str(),
                                                                                  ChatType_RoomChat, 0, 50, 0);
           LOCK_INTERFACE("QueryHistoryMessage")

           std::this_thread::sleep_for(std::chrono::seconds(qrand() % 5));
       }
    });
    t7.detach();

    std::thread t8 = std::thread([this](){
        while (true) {
            if (m_stop)
                break;
            YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(m_roomID.c_str(),
                                                                                                 1, 1);
            LOCK_INTERFACE("QueryRoomHistoryMessageFromServer")

            YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(m_roomID.c_str(),
                                                                                                 100, 1);
            LOCK_INTERFACE("QueryRoomHistoryMessageFromServer")
            YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(m_roomID.c_str(),
                                                                                   50, 1);
            LOCK_INTERFACE("QueryRoomHistoryMessageFromServer")
            YIMManager::CreateInstance()->GetMessageManager()->QueryRoomHistoryMessageFromServer(nullptr,
                                                                                                 30, 0);
            LOCK_INTERFACE("QueryRoomHistoryMessageFromServer")

            std::this_thread::sleep_for(std::chrono::seconds(qrand() % 5));
        }
    });
    t8.detach();

    std::thread t9 = std::thread([this](){

       //YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom(m_roomID.c_str());
        while (true) {
            if (m_stop)
                break;
            YIMManager::CreateInstance()->GetMessageManager()->DeleteHistoryMessage(m_robotID.c_str(),
                                                                                    ChatType_RoomChat,
                                                                                    0, 100);
            LOCK_INTERFACE("DeleteHistoryMessage")
            YIMManager::CreateInstance()->GetMessageManager()->DeleteHistoryMessage(m_roomID.c_str(),
                                                                                    ChatType_PrivateChat,
                                                                                    0, 1000);
            LOCK_INTERFACE("DeleteHistoryMessage")
            std::vector<XUINT64> ids;
            ids.push_back(0L);
            ids.push_back(1L);
            ids.push_back(2L);
            YIMManager::CreateInstance()->GetMessageManager()->DeleteSpecifiedHistoryMessage(m_robotID.c_str(),
                                                                                             ChatType_RoomChat,
                                                                                             ids);
            LOCK_INTERFACE("DeleteSpecifiedHistoryMessage")
            ids.clear();
            //YIMManager::CreateInstance()->GetMessageManager()->DeleteSpecifiedHistoryMessage(nullptr,
            //                                                                                 ChatType_PrivateChat, ids);
            LOCK_INTERFACE("DeleteSpecifiedHistoryMessage")
            YIMManager::CreateInstance()->GetMessageManager()->DeleteSpecifiedHistoryMessage(m_robotID.c_str(),
                                                                                             ChatType_RoomChat, ids);
            LOCK_INTERFACE("DeleteSpecifiedHistoryMessage")

            XUINT64 id = 10000000000000000L + qrand() % 1000000000000000L;
            YIMManager::CreateInstance()->GetMessageManager()->DeleteHistoryMessageByID(id);
            LOCK_INTERFACE("DeleteHistoryMessageByID")

            std::this_thread::sleep_for(std::chrono::seconds(qrand() % 10));
        }
    });
    t9.detach();

    std::thread t10 = std::thread([this](){
        while (true) {
            if (m_stop)
                break;
            YIMManager::CreateInstance()->GetMessageManager()->SetAllMessageRead(m_robotID.c_str(), true);
            LOCK_INTERFACE("SetAllMessageRead")

            YIMManager::CreateInstance()->GetMessageManager()->SetAllMessageRead(m_robotID.c_str(), false);
            LOCK_INTERFACE("SetAllMessageRead")

            YIMManager::CreateInstance()->GetMessageManager()->SetDownloadDir(__XT(""));
            LOCK_INTERFACE("SetDownloadDir")

            YIMManager::CreateInstance()->GetMessageManager()->SetDownloadDir(__XT("test"));
            LOCK_INTERFACE("SetDownloadDir")
            std::this_thread::sleep_for(std::chrono::seconds(qrand() % 10));
        }
    });
    t10.detach();

    //detach的线程全部加到容器里 退出的时候统一处置

}

void StableTestDialog::slot_updateStatData()
{
    QStandardItemModel *model =
            qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (!model)
        return;
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "interface_name" << "times");

    QList<QString> interfaces = m_stat_map.keys();
    foreach (QString interface, interfaces) {
        QStandardItem *item0 = new QStandardItem(interface);
        QStandardItem *item1 = new QStandardItem(QString::number(m_stat_map[interface]));
        model->appendRow(QList<QStandardItem*>() << item0 << item1);
    }
}

void StableTestDialog::slot_voice_interface()
{
    //qDebug() << "=============================";
    m_stop = false;

    std::thread t1 = std::thread([this](){
        for (;;) {
            if (m_stop)
                break;
            XUINT64 reqNo;
            YIMManager *im = YIMManager::CreateInstance();
            YIMErrorcode code1 =
                im->GetMessageManager()->StartAudioSpeech(&reqNo, qrand()%2);

            m_mutex.lock();
            m_stat_map["StartAudioSpeech"]++;
            m_mutex.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 3000));
            YIMErrorcode code2 =
                im->GetMessageManager()->StopAudioMessage(__XT("{\"errno\": \"ok\"}"));
            qDebug() << "[StartAudioSpeech]Errorcode : " << code1 <<
                        "," << "[StopAudioMessage]" << code2;

            {
                QMutexLocker lock(&m_mutex);
                m_stat_map["StopAudioMessage"]++;
            }

        }
    });

    std::thread t2 = std::thread([this](){
        for (;;) {
            if (m_stop)
               break;
            YIMManager *im = YIMManager::CreateInstance();
            //带语音识别
            XUINT64 reqNo;
            if (qrand() % 2) { //房间
                YIMErrorcode code1 =
                        im->GetMessageManager()->SendAudioMessage(m_roomID.c_str()
                                       , ChatType_RoomChat
                                       , nullptr);
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["SendAudioMessage"]++;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 10000));

                YIMErrorcode code2 =
                        im->GetMessageManager()->StopAudioMessage(__XT(""));
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["StopAudioMessage"]++;
                }

                qDebug() << "[SendAudioMessage RoomType ]Errorcode : " << code1 <<
                            "," << "[StopAudioMessage]" << code2;
            } else { // private
                YIMErrorcode code1 =
                        im->GetMessageManager()->SendAudioMessage(m_robotID.c_str()
                                       , ChatType_PrivateChat
                                       , &reqNo);
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["SendAudioMessage"]++;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 10000));

                YIMErrorcode code2 =
                    im->GetMessageManager()->StopAudioMessage(__XT(""));
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["StopAudioMessage"]++;
                }
                qDebug() << "[SendAudioMessage PrivateType ]Errorcode : " << code1 <<
                            "," << "[StopAudioMessage]" << code2;
            }
        }
    });

    std::thread t3 = std::thread([this](){
        while (true) {
            if (m_stop)
                break;
            YIMManager *im = YIMManager::CreateInstance();
            //带语音识别
            XUINT64 reqNo;
            if (qrand() % 2) { //房间
                YIMErrorcode code1 =
                        im->GetMessageManager()->SendAudioMessage(m_roomID.c_str()
                                       , ChatType_RoomChat
                                       , &reqNo);
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["SendAudioMessage"]++;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 + qrand() % 15000));

                YIMErrorcode code2 =
                        im->GetMessageManager()->StopAudioMessage(__XT(""));

                LOCK_INTERFACE("StopAudioMessage")
                qDebug() << "[SendOnlyAudioMessage RoomType] Errorcode: "
                         << code1 << ", " << "[StopAudioMessage]" << code2;
            } else { // private
                YIMErrorcode code1 = im->GetMessageManager()->SendAudioMessage(m_robotID.c_str()
                                       , ChatType_PrivateChat
                                       , &reqNo);

                LOCK_INTERFACE("SendAudioMessage")
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 + qrand() % 15000));

                YIMErrorcode code2 = im->GetMessageManager()->StopAudioMessage(__XT(""));

                LOCK_INTERFACE("StopAudioMessage")
                qDebug() << "[SendAudioMessage PrivateType] Errorcode: "
                         << code1 << ", " << "[StopAudioMessage]" << code2;
            }
        }
    });

    std::thread t4 = std::thread([this]() {
        while (true) {
            if (m_stop)
                break;
            YIMManager *im = YIMManager::CreateInstance();
            //带语音识别
            XUINT64 reqNo;
            if (qrand() % 2) { //房间
                YIMErrorcode code1 =
                        im->GetMessageManager()->SendOnlyAudioMessage(m_roomID.c_str()
                                       , ChatType_RoomChat
                                       , &reqNo);

                LOCK_INTERFACE("SendOnlyAudioMessage")
                std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 3000));

                YIMErrorcode code2 =
                        im->GetMessageManager()->CancleAudioMessage();

                LOCK_INTERFACE("CancleAudioMessage")
                qDebug() << "[SendOnlyAudioMessage RoomType] Errorcode: "
                         << code1 << ", " << "[CancleAudioMessage]" << code2;
            } else { // private
                YIMErrorcode code1 = im->GetMessageManager()->SendOnlyAudioMessage(m_robotID.c_str()
                                       , ChatType_PrivateChat
                                       , &reqNo);

                LOCK_INTERFACE("SendOnlyAudioMessage")
                std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 10000));

                YIMErrorcode code2 = im->GetMessageManager()->CancleAudioMessage();

                LOCK_INTERFACE("CancleAudioMessage")
                qDebug() << "[SendOnlyAudioMessage PrivateType] Errorcode: "
                         << code1 << ", " << "[CancleAudioMessage]" << code2;
            }
        }
    });

    std::thread t5 = std::thread([this]() {
        while (true) {
            if (m_stop)
                break;
            YIMManager *im = YIMManager::CreateInstance();
            //带语音识别
            XUINT64 reqNo;
            if (qrand() % 2) { //房间
                YIMErrorcode code1 =
                    im->GetMessageManager()->SendAudioMessage(m_roomID.c_str()
                                       , ChatType_RoomChat
                                       , &reqNo);
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["SendAudioMessage"]++;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 10000));

                YIMErrorcode code2 =
                        im->GetMessageManager()->CancleAudioMessage();
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["CancleAudioMessage"]++;
                }
                qDebug() << "[SendAudioMessage RoomType] Errorcode: "
                         << code1 << ", " << "[CancleAudioMessage]" << code2;
            } else { // private
                YIMErrorcode code1 = im->GetMessageManager()->SendAudioMessage(m_robotID.c_str()
                                       , ChatType_PrivateChat
                                       , &reqNo);
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["SendAudioMessage"]++;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(qrand() % 10000));

                YIMErrorcode code2 = im->GetMessageManager()->CancleAudioMessage();
                {
                    QMutexLocker lock(&m_mutex);
                    m_stat_map["CancleAudioMessage"]++;
                }
                qDebug() << "[SendAudioMessage PrivateType] Errorcode: "
                         << code1 << ", " << "[CancleAudioMessage]" << code2;
            }
        }
    });

    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    t5.detach();
}

void StableTestDialog::slot_reconnect_interface()
{
   YIMManager *im = YIMManager::CreateInstance();
   //im->GetUserProfileManager();
   im->GetSDKVersion(); //重连怎么触发 有两个回调
}

void StableTestDialog::slot_stop_all_threads()
{
    m_stop = true;
    qDebug() << QStringLiteral("停止所有线程");
}

void StableTestDialog::OnLogin(YIMErrorcode errorcode, const XString &userID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << XStringToLocal(userID).c_str() << QStringLiteral("登录成功");
    }
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom(m_roomID.c_str());
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("加入房间失败");
    }
}

void StableTestDialog::OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("加入房间失败：错误码") << errorcode;
    }
    qDebug() << QStringLiteral("加入房间成功, 房间号: ") << XStringToLocal(chatRoomID).c_str();
}

void StableTestDialog::OnStartReconnect()
{

}

void StableTestDialog::OnSendMessageStatus(XUINT64 requestID,
                                           YIMErrorcode errorcode,
                                           unsigned int sendTime,
                                           bool isForbidRoom,
                                           int reasonType, XUINT64 forbidEndTime)
{
    qDebug() << "OnSendMessageStatus";
}

void StableTestDialog::OnStartSendAudioMessage(XUINT64 requestID,
                                               YIMErrorcode errorcode,
                                               const XString &text,
                                               const XString &audioPath,
                                               unsigned int audioTime)
{
    qDebug() << "OnStartSendAudioMessage";
}

void StableTestDialog::OnStopAudioSpeechStatus(YIMErrorcode errorcode,
                                               std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo)
{
    qDebug() << "OnStopAudioSpeechStatus";
}

void StableTestDialog::OnSendAudioMessageStatus(XUINT64 requestID,
                                                YIMErrorcode errorcode,
                                                const XString &text,
                                                const XString &audioPath,
                                                unsigned int audioTime,
                                                unsigned int sendTime,
                                                bool isForbidRoom,
                                                int reasonType,
                                                XUINT64 forbidEndTime)
{
    qDebug() << "OnSendAudioMessageStatus";
}

void StableTestDialog::OnLogout(YIMErrorcode errorcode)
{
    if (errorcode != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登出失败");
    }
}

void StableTestDialog::OnRecvMessage(std::shared_ptr<IYIMMessage> message)
{
    XUINT64 id = message->GetMessageID();
    YIMMessageBodyType messageType = message->GetMessageBody()->GetMessageType();

    quint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch(); //收到消息时的时间戳

    switch (messageType)
    {
    case MessageBodyType_TXT:
        {
        }
        break;
    case MessageBodyType_CustomMesssage:
        {
            IYIMMessageBodyCustom *customtext =
                    static_cast<IYIMMessageBodyCustom*>(message->GetMessageBody());
            if (!customtext)
                break;
            std::string custom_str = customtext->GetCustomMessage();
            QByteArray text = QByteArray::fromStdString(custom_str).toBase64();
            Q_UNUSED(text)
        }
        break;
    case MessageBodyType_Gift:
        {
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
            qDebug() << current_time << ":" << json;
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

QString StableTestDialog::getRandomStr(int n)
{
    const char* source_character = "`~!@#$%^&*((_))+=}{[]|\?><,./\":;'\1234567890"
                                   "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = strlen(source_character);
    QString res(n, '0');
    for (int i = 0; i < n; ++i) {
        res[i] = source_character[qrand() % len];
    }
    return res;
}

void StableTestDialog::slot_exportData()
{
    QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm");
    QFile file(QString("statble_stat_%1.txt").arg(time));

    if (!file.open(QIODevice::Append)) {
        qDebug() << "Open file failed " << file.errorString();
        return;
    }
    QList<QString> keys = m_stat_map.keys();
    QTextStream out(&file);
    foreach (QString key, keys) {
         out << key << "\t" <<  m_stat_map[key] << "\n";
    }

    file.close();
    QMessageBox::information(this, QString("info"), QStringLiteral("导出成功"),
                             QMessageBox::Ok);
}
