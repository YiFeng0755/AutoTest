#include "mainwidget.h"
#include <QDebug>
#include <limits>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QLayout>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutexLocker>
#include <thread>
#include "qglobal.h"

#define LOG_ERRORCODE(code) (qDebug() << QString("return code: %1").arg(code1));

MainWidget::MainWidget(QString username,
                     QString roomid,
                     QString passwd,
                     QString token,
                     QWidget *parent)
    :QWidget(parent)
{
    setMinimumSize(600, 800);
#ifdef WIN32
    m_username = username.toStdWString();
    m_roomID = roomid.toStdWString();
    m_passwd = passwd.toStdWString();
    if (m_passwd.empty()) m_passwd = __XT("123456");
    m_token = token.toStdWString();
#else
    m_username = username.toStdString();
    m_roomID = roomid.toStdString();
    m_passwd = passwd.toStdString();
    m_token = token.toStdString();
#endif

    init();
}

MainWidget::~MainWidget()
{
    //quitRobot();
}

void MainWidget::init()
{
    YIMManager* im = YIMManager::CreateInstance();
    im->SetLoginCallback(this);
    im->SetChatRoomCallback(this);
    im->SetMessageCallback(this);

    //YIMErrorcode code1 = im->GetMessageManager()->SetDownloadDir(__XT("./audio/"));
//    if (code1 != YIMErrorcode_Success)
//        qDebug() << QStringLiteral("设置下载目录错误") << code1;

    //im->GetMessageManager()->SetDownloadAudioMessageSwitch(true); //默认自动下载语音

    m_status = new QLabel(tr("status:"));
    m_editor = new QTextBrowser(this);

    YIMErrorcode code2 = im->Login(m_username.c_str(),
                                  m_passwd.c_str(),
                                  m_token.c_str());
    if (code2 != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("登录失败，错误码 : ") << code2;
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_status);
    mainLayout->addWidget(m_editor);

    QPushButton *close = new QPushButton(QStringLiteral("杀死机器人"));
    connect(close, SIGNAL(clicked()), this, SLOT(quitRobot()));
    //QPushButton *login = new QPushButton(QStringLiteral("登录"));
    //connect(login, SIGNAL(clicked()), this, SLOT());

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(close);
    buttonLayout->addStretch(0);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void MainWidget::OnRecvMessage(std::shared_ptr<IYIMMessage> message)
{
    YIMManager *im = YIMManager::CreateInstance();
    XUINT64 id = message->GetMessageID();
    qDebug() << QStringLiteral("收到消息id") << id;
    YIMMessageBodyType messageType = message->GetMessageBody()->GetMessageType();
    XString sendID = message->GetSenderID();
    XString receivID = message->GetReceiveID();
    YIMChatType type = message->GetChatType();
    unsigned int create_time = message->GetCreateTime();

    QJsonObject jobj;
    jobj["msg_id"] = static_cast<qint64>(id);
    jobj["send_id"] = QString::fromStdWString(sendID);
    jobj["recv_id"] = QString::fromStdWString(receivID);
    jobj["create_time"] = static_cast<qint64>(create_time);

    qint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    jobj["robot_recv_time"] = current_time;
    switch (messageType)
    {
    case MessageBodyType_TXT:
        {
            IYIMMessageBodyText* text =
                static_cast<IYIMMessageBodyText*>(message->GetMessageBody());
            //qDebug() << XStringToLocal(XString(text->GetMessageContent())).c_str();
            jobj["msg_content"] = QString::fromStdWString(XString(text->GetMessageContent()));

            QString showtext = QString::number(current_time) + " " +
                    QString::fromStdWString(XString(text->GetMessageContent()));

           //锁住
            {
                QMutexLocker locker(&textlock);
                //m_editor->append(showtext);
            }

        }
        break;
    case MessageBodyType_CustomMesssage:
        {
            IYIMMessageBodyCustom* custom =
                static_cast<IYIMMessageBodyCustom*>(message->GetMessageBody());
            std::string custom_str = custom->GetCustomMessage();
            QByteArray text = QByteArray::fromStdString(custom_str).toBase64();
            jobj["msg_content"] = QString(text);

            QString showText = QString::number(current_time)
                    + QString(" ") +
                    QString(text);

            //m_editor->append(showText.left(20));
            {
                QMutexLocker locker(&textlock);
                //m_editor->append(QString::fromStdString("bindata: " + custom_str));
            }
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
            jobj["audiotime"] = static_cast<qint64>(audio->GetAudioTime());
            jobj["extraparam"] = QString::fromStdWString(XString(audio->GetExtraParam()));
            jobj["localpath"] = QString::fromStdWString(XString(audio->GetLocalPath()));
            jobj["filesize"] = static_cast<qint64>(audio->GetFileSize());
            jobj["text"] = QString::fromStdWString(XString(audio->GetText()));

            QString voicestr = QString::number(current_time)
                    + QString(" [voice] ") + QString::number(audio->GetAudioTime()) + QString("s");
            {
                QMutexLocker locker(&textlock);
                m_editor->append(voicestr);
            }
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

    QString qsendID = QString::fromStdWString(sendID);

    if (messageType == MessageBodyType_TXT ||
            messageType == MessageBodyType_Gift||
            messageType == MessageBodyType_CustomMesssage
            && (!qsendID.contains("robot"))) { //如果是机器人发的消息 就不回
        QJsonDocument doc;
        qint64 ctime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        jobj["robot_send_time"] = ctime;
        doc.setObject(jobj);

        if (type == ChatType_RoomChat) {
            XUINT64 reqNo;
            YIMErrorcode code =
                im->GetMessageManager()->SendTextMessage(receivID.c_str(),
                                                         type,
                                                         LocalToXString(doc.toJson().toStdString()).c_str(),
                                                         &reqNo);
            if (code != YIMErrorcode_Success) {
                qDebug() << QStringLiteral("消息回复不成功");
            }
        }

        else if (type == ChatType_PrivateChat ||
             type == ChatType_Multi) {
            XUINT64 reqNo;
            YIMErrorcode code = im->GetMessageManager()->SendTextMessage(sendID.c_str(), type,
                                                                     LocalToXString(doc.toJson().toStdString()).c_str(),
                                                                     &reqNo);
            if (code != YIMErrorcode_Success) {
                qDebug() << QStringLiteral("消息回复不成功");
            }
        }
        else {
            qDebug() << QStringLiteral("收到一条来源不明的消息, 机器人不回应");
        }
    }
    //机器人是低智商的，每次收到消息就回复一个回音
    /**
    if (messageType == MessageBodyType_Voice) {
        if (type == ChatType_RoomChat) {
            XUINT64 reqNo;
            YIMErrorcode code =
                im->GetMessageManager()->SendAudioMessage(receivID.c_str(),
                                                      ChatType_PrivateChat,
                                                      &reqNo);
            if (code != YIMErrorcode_Success) {
                qDebug() << QStringLiteral("消息回复不成功");
            }

            std::this_thread::sleep_for(std::chrono::seconds(3));

            YIMErrorcode code1 =
                    im->GetMessageManager()->StopAudioMessage(__XT("robot answer"));
            if (code1 != YIMErrorcode_Success) {
                qDebug() << QStringLiteral("回复录音失败 ") << code1;
            }

        }

        else if (type == ChatType_PrivateChat ||
             type == ChatType_Multi) {
            XUINT64 reqNo;
            YIMErrorcode code = im->GetMessageManager()->SendAudioMessage(sendID.c_str(),
                                                                          ChatType_PrivateChat,
                                                                          &reqNo);
            if (code != YIMErrorcode_Success) {
                qDebug() << QStringLiteral("消息回复不成功");
            }

            std::this_thread::sleep_for(std::chrono::seconds(3));

            YIMErrorcode code1 =
                    im->GetMessageManager()->StopAudioMessage(__XT("robot answer"));
            if (code1 != YIMErrorcode_Success) {
                qDebug() << QStringLiteral("回复录音失败 ") << code1;
            }
        }
        else {
            qDebug() << QStringLiteral("收到一条来源不明的消息, 机器人不回应");
        }
    }
    **/
}

void MainWidget::OnDownload(YIMErrorcode errorcode,
                            std::shared_ptr<IYIMMessage> msg,
                            const XString &savePath)
{
    Q_UNUSED(msg)
    if (errorcode != YIMErrorcode_Success)
        qDebug() << QStringLiteral("下载回调失败, 错误码:") << errorcode;
    qDebug() << "Save path is " << XStringToLocal(savePath).c_str();
}

void MainWidget::quitRobot()
{
    YIMManager *im = YIMManager::CreateInstance();
    std::unique_lock<std::mutex> lk(mutex);
    im->Logout();
    cv.wait_for(lk, std::chrono::seconds(10));
    qDebug() << "Quit successs";
}
