#include "performancewidget.h"
#include "ui_performancewidget.h"
#include "globalsetting.h"

#include <QtConcurrent/QtConcurrent>
#include <QButtonGroup>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QList>
#include <QFile>
#include <QLabel>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QLayout>
#include <QTextStream>
#include <QTableView>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>


PerformanceWidget::PerformanceWidget(QWidget *parent, bool isDebug)
    :QDialog(parent),
      m_isDebug(isDebug)
{
    srand(unsigned int(time(NULL)));

    m_send_times = m_recv_times = 0;

    QString user = QString("PerfUser") + QString::number(qrand() % 100000);
    m_userID = UTF8TOXString(user.toStdString()).c_str();
    QString robotUser = QString("robot_perf_") + QString::number(qrand() % 100000 + 500000);
    m_robotID = UTF8TOXString(robotUser.toStdString()).c_str();
    QString roomid = QString("201") + QString::number(qrand() % 10000 + 1000);
    m_roomID = UTF8TOXString(roomid.toStdString()).c_str();
    //debug
    //m_roomID = __XT("2013333");
    //m_robotID = __XT("robot");
    setMinimumSize(800, 600);
    setup();
    setupTable();
}

PerformanceWidget::~PerformanceWidget()
{
    YIMManager::CreateInstance()->SetMessageCallback(nullptr);
    YIMManager::CreateInstance()->SetLoginCallback(nullptr);
    YIMManager::CreateInstance()->SetChatRoomCallback(nullptr);

   // m_process->kill();
}

void PerformanceWidget::setup()
{
    m_process = new QProcess(this);
    QString cmd = "./roboter.exe";
    QStringList arguments;
    QString appkey = GlobalSettings::getInstance()->getIMAppkey();
    QString appsecret = GlobalSettings::getInstance()->getIMAppSecret();
    int isDebug = (int)m_isDebug;
    arguments << QString("{\"Username\":\"%1\",\"RoomID\":\"%2\",\"Passwd\":\"%3\","
                         " \"Token\":\"\",\"Appkey\":\"%4\",\"Appsecret\":\"%5\",\"IsDebug\":%6}")
            .arg(QString::fromStdWString(m_robotID))
            .arg(QString::fromStdWString(m_roomID))
            .arg(QString::fromStdWString(__XT("123456")))
            .arg(appkey)
            .arg(appsecret)
            .arg(isDebug);
    qDebug() << arguments;
    m_process->start(cmd, arguments);

    //register callback functions
    YIMManager::CreateInstance()->SetMessageCallback(this);
    YIMManager::CreateInstance()->SetLoginCallback(this);
    YIMManager::CreateInstance()->SetChatRoomCallback(this);

    m_table_view = new QTableView(this);
    m_model = new QStandardItemModel;
    m_table_view->setModel(m_model);

    m_status = new QLabel;
    m_status->setText(QString("RoomID: %1\nRobotID: %2\n").arg(QString::fromStdWString(m_roomID))
                                                        .arg(QString::fromStdWString(m_robotID)));

    m_stat_send = new QLabel(QStringLiteral("已发送: 0"));
    m_stat_recv = new QLabel(QStringLiteral("已接收: 0"));
    m_percent_rate = new QLabel(QStringLiteral("消息抵达率: %0.00"));

    QVBoxLayout *statusLayout = new QVBoxLayout;
    statusLayout->addWidget(m_stat_send);
    statusLayout->addWidget(m_stat_recv);
    statusLayout->addWidget(m_percent_rate);

    m_text_bytesize = new QComboBox;
    m_voice_elapse = new QComboBox;
    m_text_bytesize->addItems(QStringList() << "10" << "50" << "200" << "300");
    m_voice_elapse->addItems(QStringList() << "5" << "10" << "25" << "50");
    m_text_bytesize->setEditable(true);
    m_voice_elapse->setEditable(true);

    QLabel *lbl_bytesize = new QLabel(QStringLiteral("字节大小"));
    QLabel *lbl_voicesize = new QLabel(QStringLiteral("录音时长"));
    QLabel *lbl_send_times = new QLabel(QStringLiteral("发送次数"));

    m_rounds = new QSpinBox;
    m_rounds->setValue(20);
    m_rounds->setRange(1, 50000);

    lbl_bytesize->setBuddy(m_text_bytesize);
    lbl_voicesize->setBuddy(m_voice_elapse);
    lbl_send_times->setBuddy(m_rounds);

    QHBoxLayout *comboLayout1 = new QHBoxLayout;
    comboLayout1->addWidget(lbl_bytesize);
    comboLayout1->addWidget(m_text_bytesize);
    //comboLayout1->addStretch();
    QHBoxLayout *comboLayout2 = new QHBoxLayout;
    comboLayout2->addWidget(lbl_voicesize);
    comboLayout2->addWidget(m_voice_elapse);
    //comboLayout2->addStretch();
    QHBoxLayout *spinboxLayout = new QHBoxLayout;
    spinboxLayout->addWidget(lbl_send_times);
    spinboxLayout->addWidget(m_rounds);
    //spinboxLayout->addStretch();

    m_chattypeBox = new QGroupBox;
    m_btn_multi = new QRadioButton(QStringLiteral("多房间聊天"));
    m_btn_private = new QRadioButton(QStringLiteral("私聊"));
    m_btn_room = new QRadioButton(QStringLiteral("房间"));
    m_btn_room->setChecked(true);

    QVBoxLayout *gLayout = new QVBoxLayout;
    QButtonGroup *group = new QButtonGroup;
    group->setExclusive(true);
    group->addButton(m_btn_multi);
    group->addButton(m_btn_private);
    group->addButton(m_btn_room);
    gLayout->addWidget(m_btn_multi);
    gLayout->addWidget(m_btn_private);
    gLayout->addWidget(m_btn_room);

    m_chattypeBox->setLayout(gLayout);

    QVBoxLayout *vcomboLayout = new QVBoxLayout;
    vcomboLayout->addLayout(comboLayout1);
    vcomboLayout->addLayout(comboLayout2);
    vcomboLayout->addLayout(spinboxLayout);

    QHBoxLayout *paneLayout = new QHBoxLayout;
    paneLayout->addLayout(vcomboLayout);
    paneLayout->addWidget(m_chattypeBox);
    paneLayout->addLayout(statusLayout);
    //paneLayout

    m_delay_test = new QPushButton(QStringLiteral("测量延迟"));
    m_voice_delay_test = new QPushButton(QStringLiteral("测量短语音延迟"));
    m_btn_export = new QPushButton(QStringLiteral("导出结果"));
    QPushButton *btn_login = new QPushButton(QStringLiteral("登录"));
    QPushButton *btn_save = new QPushButton(QStringLiteral("保存数据"));

    connect(btn_login, SIGNAL(clicked()), this, SLOT(slot_login()));
    connect(btn_save, SIGNAL(clicked()), this, SLOT(slot_writeToDisk()));
    connect(m_delay_test, SIGNAL(clicked(bool)), this, SLOT(slot_exe_perftest()));
    connect(m_btn_export, SIGNAL(clicked(bool)), this, SLOT(slot_export2csv()));

    m_progressbar = new QProgressBar(this);
    m_progressbar->hide();
    connect(this, SIGNAL(progress(int)), this, SLOT(slot_updateProgress(int)));
    connect(this, SIGNAL(timesChanged()), this, SLOT(slot_updateLabelString()));

    QVBoxLayout *vlayout = new QVBoxLayout;

    QHBoxLayout *btnLayout = new QHBoxLayout;
    QHBoxLayout *btnLayout2 = new QHBoxLayout;

    btnLayout->addStretch();
    btnLayout->addWidget(m_delay_test);
    btnLayout->addWidget(m_voice_delay_test);
    btnLayout->addWidget(m_btn_export);
    btnLayout2->addStretch();
    //btnLayout2->addWidget(btn_login);
    //btnLayout2->addWidget(btn_save);

    vlayout->addWidget(m_status);
    vlayout->addLayout(paneLayout);
    vlayout->addWidget(m_table_view);
    vlayout->addWidget(m_progressbar);
    vlayout->addLayout(btnLayout);
    vlayout->addLayout(btnLayout2);

    setLayout(vlayout);

    slot_login(); //自动登录
}

void PerformanceWidget::setupTable()
{
    QStringList title;
    title << "msgid" << "len" << "send_time" << "recv_time" << "robot_send_time" << "robot_recv_time"
          << "server_create_time" << "delay" << "back_delay" << "to_delay";

    m_model->setHorizontalHeaderLabels(title);
    //m_table_view->verticalHeader()->hide();
    m_table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_table_view->setVerticalScrollMode(QTableView::ScrollPerItem);
    m_table_view->setAutoScroll(true);
}

void PerformanceWidget::OnRecvMessage(std::shared_ptr<IYIMMessage> message)
{
    XUINT64 id = message->GetMessageID();
    YIMMessageBodyType messageType = message->GetMessageBody()->GetMessageType();

    quint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch(); //收到消息时的时间戳

    switch (messageType)
    {
    case MessageBodyType_TXT:
        {
            ++m_recv_times;
            emit timesChanged();
            IYIMMessageBodyText* text =
                static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            //QString msgcontent = QString::fromStdWString();
            QByteArray jstext =
            QByteArray::fromStdString(XStringToLocal(XString(text->GetMessageContent())));

            QJsonObject json_object = QJsonDocument::fromJson(jstext).object();
            qint64 create_time = json_object["create_time"].toInt(); //机器人发送的消息创建时间

            std::string msg_content = json_object["msg_content"].toString().toStdString();

            QJsonObject content_obj = QJsonDocument::fromJson(QByteArray::fromStdString(msg_content)).object();

            qint64 send_time = static_cast<qint64>(content_obj["sendtime"].toDouble());
            QString content = content_obj["content"].toString();
            qint64 robot_recv_time = static_cast<qint64>(json_object["robot_recv_time"].toDouble());
            //QString sendID = json_object["sendID"].toString();
            //QString recvID = json_object["reveivID"].toString();
            qint64 robot_send_time = static_cast<qint64>(json_object["robot_send_time"].toDouble());
            QStandardItem *item0 = new QStandardItem;
            item0->setText(QString::number(id));

            QStandardItem *item1 = new QStandardItem;
            item1->setText(QString::number(content.size()));

            QStandardItem *item2 = new QStandardItem;
            item2->setText(QString::number(send_time));

            QStandardItem *item3 = new QStandardItem;
            item3->setText(QString::number(current_time));

            QStandardItem *item4 = new QStandardItem;
            item4->setText(QString::number(robot_send_time));

            QStandardItem *item5 = new QStandardItem;
            item5->setText(QString::number(robot_recv_time));

            QStandardItem *item6 = new QStandardItem;
            item6->setText(QString::number(create_time));

            QStandardItem *item7 = new QStandardItem;
            item7->setText(QString::number(current_time - send_time));

            QStandardItem *item8 = new QStandardItem;
            item8->setText(QString::number(current_time - robot_send_time));

            QStandardItem *item9 = new QStandardItem;
            item9->setText(QString::number(robot_recv_time - send_time));
            QList<QStandardItem*> items;
            items << item0 << item1 << item2 << item3 << item4 << item5 << item6
                  << item7 << item8 << item9;
            m_model->appendRow(items);

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

void PerformanceWidget::OnSendMessageStatus(XUINT64 requestID,
                                            YIMErrorcode errorcode,
                                            unsigned int sendTime,
                                            bool isForbidRoom,
                                            int reasonType,
                                            XUINT64 forbidEndTime)
{
    if (errorcode == YIMErrorcode_Success) {
        m_send_times++; //++操作不是原子的，注意 变量m_send_times要加锁
       // qDebug() << tr("Message send success! have sended %1 number").arg(m_send_times);
        emit progress(m_send_times);
        emit timesChanged();
    } else {
        qDebug() << QStringLiteral("消息发送失败 : ") << errorcode;
    }
}

void PerformanceWidget::slot_exe_perftest()
{
    m_send_times = 0;
    m_recv_times = 0; //init
    m_progressbar->show();

    //int cases_voice_time[] = {5, 10, 25, 50};
    int cnt = m_rounds->value();
    int bytes = m_text_bytesize->currentText().toInt();
    m_progressbar->setRange(0, cnt);
    m_progressbar->setValue(0);
    m_exe_thread = std::thread([&, cnt, bytes](){
        for (int i = 0; i < cnt; ++i) {
            QString text = this->getRandomStr(bytes);

            QJsonDocument doc;
            QJsonObject obj;
            obj["sendtime"] = static_cast<qint64>(QDateTime::currentDateTime().toMSecsSinceEpoch());
            obj["content"] = text;
            doc.setObject(obj);
            doc.toJson();
            XString xtext = LocalToXString(doc.toJson().toStdString());

            YIMErrorcode code;

            if (m_btn_private->isChecked()) {
                code = YIMManager::CreateInstance()->GetMessageManager()->SendTextMessage(m_robotID.c_str(),
                                                                                  ChatType_PrivateChat,
                                                                                  xtext.c_str(), nullptr);
            } else if (m_btn_room->isChecked()) {
                code = YIMManager::CreateInstance()->GetMessageManager()->SendTextMessage(m_roomID.c_str(),
                                                                                   ChatType_RoomChat,
                                                                                   xtext.c_str(), nullptr);
            } else if (m_btn_multi->isChecked()) {
                code = YIMManager::CreateInstance()->GetMessageManager()->SendTextMessage(m_roomID.c_str(),
                                                                                   ChatType_Multi,
                                                                                   xtext.c_str(), nullptr);
            }

            if (code != YIMErrorcode_Success)
                qDebug() << QStringLiteral("消息发送失败，错误码 ") << code;

            std::this_thread::sleep_for(std::chrono::milliseconds(200)); //0.1s发一次
        }
    });

    m_exe_thread.detach();
}

void PerformanceWidget::slot_login()
{
    YIMErrorcode code =
            YIMManager::CreateInstance()->Login(m_userID.c_str(),
                                                __XT("123456"),
                                                __XT(""));
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登录失败, 错误码") << code;
    }
}

void PerformanceWidget::slot_leave()
{
    YIMErrorcode code =
            YIMManager::CreateInstance()->GetChatRoomManager()->LeaveChatRoom(m_roomID.c_str());
    if (code != YIMErrorcode_Success)
        qDebug() << "leave room success";
}

void PerformanceWidget::slot_updateProgress(int v)
{
    m_progressbar->setValue(v);
    if (v == m_rounds->value())
        m_progressbar->hide();
}

QString PerformanceWidget::getRandomStr(int n)
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

void PerformanceWidget::OnLogin(YIMErrorcode errorcode, const XString &userID)
{
    Q_UNUSED(userID)
    if (errorcode == YIMErrorcode_Success)
    {
        qDebug() <<  XStringToLocal(userID).c_str() << QStringLiteral("登录成功");
        YIMManager::CreateInstance()->GetChatRoomManager()->JoinChatRoom(m_roomID.c_str());
    }
}

void PerformanceWidget::OnLogout(YIMErrorcode errorcode)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << "Logout success";
    }
}

void PerformanceWidget::OnLeaveChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << QString("Leave room %1 success").arg(XStringToLocal(chatRoomID).c_str());
        YIMManager::CreateInstance()->Logout();
    }
}

void PerformanceWidget::OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID)
{
    if (errorcode == YIMErrorcode_Success) {
        qDebug() << QString("Join room :%1 success").arg(XStringToLocal(chatRoomID).c_str());
    }
}

void PerformanceWidget::slot_writeToDisk()
{
    QString filename = QString("perf_%1.txt").arg(QDateTime::currentDateTime().toString(
                                                      "yyyy_MM_dd_hh"));
    QFile file(filename);
    if (!file.open(QIODevice::Append)) {
        qDebug() << "Open file failed !" << file.errorString();
        return;
    }
    //sendtime 代表本地发出消息的时间
    //robot_send_time 机器人收到消息的时间 这个时间和机器人回应消息的时间相差很小，合二为一
    //recv_time 收到机器人发出的消息的时间戳
    QTextStream out(&file);
    out << "MsgID" << "\t" << "MsgLen" << "\t" << "recv_time" << "\t"
        << "send_time" << "\t" << "robot_send_time" << "\t"
        << "server_create_time" << "\t" << "cycle_delay" << "\t"
        << "delay" << "\n";

    foreach (TestResult res, m_results) {
         int cycle_delay = res.recv_time - res.send_time;
         int delay = res.robot_recv_time - res.send_time;
         out << res.msgid << "\t" << res.size
             << "\t" << res.recv_time << "\t"
             << res.send_time << "\t"
             << res.robot_recv_time << "\t"
             << res.create_time << "\t"
             << cycle_delay << "\t"
             << delay << "\n";
    }

    file.close();
}

void PerformanceWidget::slot_export2csv()
{
    QStandardItemModel *result_model = qobject_cast<QStandardItemModel*>(m_table_view->model());

    if (!result_model
            || result_model->rowCount() == 0) {//有一个出错我们就停止导出，发出错误报告
        QMessageBox::warning(this, QStringLiteral("数据错误"), QStringLiteral("导出错误，确认是否有生成的数据，格式等问题!停止导出"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    //dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    QDir dir = dialog.directory();

    QString baseName = QString("IM_delay_report_%1.csv")
            .arg(QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss"));
    QString filename = dir.absoluteFilePath(baseName);

    //注释掉，是为了保持历史报告
    //获取数据
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }
    file.write("delay test report\r\n\r\n");

    for (int i = 0; i < result_model->columnCount(); ++i) {
        file.write(result_model->horizontalHeaderItem(i)->text().toLatin1());
        if (i != result_model->columnCount() - 1)
            file.write(",");
    }
    file.write("\r\n");

    for (int i = 0; i < result_model->rowCount(); ++i) {
        for (int j = 0; j < result_model->columnCount(); ++j) {
            file.write(result_model->item(i, j)->text().toLatin1());
            if (j != result_model->columnCount() - 1)
                file.write(",");
        }
        file.write("\r\n");
    }

    file.close();
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导出文件 %1 成功").arg(filename),
                             QMessageBox::Ok);
}

void PerformanceWidget::slot_updateLabelString()
{
    m_stat_send->setText(QStringLiteral("已发送: ") + QString::number(m_send_times));
    m_stat_recv->setText(QStringLiteral("已接收: ") + QString::number(m_recv_times));
    if (m_rounds->value() != 0)
        m_percent_rate->setText(QStringLiteral("抵达率: ")
                                + QString::number(double(m_recv_times) / m_rounds->value() * 100) + "%");
}
