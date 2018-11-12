#include "chatwidget.h"
#include "YIM.h"
#include <QDebug>
#include <QApplication>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QTextCodec>
#include "logindialog.h"


void messageOuputHandler(QtMsgType type, const QMessageLogContext &context,
                         const QString &msg)
{
    Q_UNUSED(context);
    QByteArray localMsg = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss ").toLocal8Bit()
            + msg.toLocal8Bit();

       switch (type) {
       case QtDebugMsg:
           localMsg = "[DEBUG] " + localMsg;
           break;
       case QtInfoMsg:
           localMsg = "[INFO]" + localMsg;
           break;
       case QtWarningMsg:
           localMsg = "[WARN]" + localMsg;
           break;
       case QtCriticalMsg:
           localMsg = "[CRITCAL]" + localMsg;
           break;
       case QtFatalMsg:
           localMsg = "[FETAL]" + localMsg;
           abort();
       }

       QMetaObject::invokeMethod(ChatWidget::messageHandler(), "showMessages",
                                 Qt::QueuedConnection,
                                 Q_ARG(QByteArray, localMsg));


}

typedef struct Arguments {
    QString UserID;
    QString RoomID;
    XString Appkey;
    XString Appsecret;
    bool Isdebug;
} g_Userinfo;

void parseArguments(Arguments *userinfo)
{
    if (!userinfo)
        return;

    QStringList arguments = QCoreApplication::arguments();
    qDebug() << "Arguments" << arguments;

    if (arguments.count() < 2)
        return;

    QString strJson = arguments.at(1);
    qDebug() << "Json: " << strJson;

    QJsonParseError jsonError;
    QJsonDocument document
            = QJsonDocument::fromJson(strJson.toLocal8Bit(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
        return;

    if (document.isObject()) {
        QJsonObject obj = document.object();
        QJsonValue value;
        if (obj.contains("UserID")) {
            value = obj.take("UserID");
            userinfo->UserID = value.toString();
        }

        if (obj.contains("RoomID")) {
            value = obj.take("RoomID");
            userinfo->RoomID = value.toString();
        }

        if (obj.contains("Appkey")) {
            QString qstr_appkey = obj.take("Appkey").toString();
            userinfo->Appkey = LocalToXString(qstr_appkey.toStdString());
        }

        if (obj.contains("Appsecret")) {
            QString qstr_appsecret = obj.take("Appsecret").toString();
            userinfo->Appsecret = LocalToXString(qstr_appsecret.toStdString());
        }

        if (obj.contains("IsDebug")) {
            value = obj.take("IsDebug");
            userinfo->Isdebug = value.toInt();
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    qInstallMessageHandler(messageOuputHandler);
    QStringList arguments = QCoreApplication::arguments();
    qDebug() << "Arguments" << arguments;

    g_Userinfo userinfo;

    parseArguments(&userinfo);

    XString roomID = LocalToXString(userinfo.RoomID.toStdString());
    XString userID = LocalToXString(userinfo.UserID.toStdString());

    const XCHAR *appkey = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");
    const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");

    if (userinfo.Isdebug)
        IM_SetMode(2);

//    if (roomID.empty() || userID.empty()) {
//        LoginDialog dlg;
//        dlg.exec();

//        YIMErrorcode code = YIMManager::CreateInstance()->Init(appkey, appSecret, __XT(""));

//        if (code == YIMErrorcode_Success) {
//            qDebug() << QStringLiteral("调用初始化成功");
//        }

//        roomID = LocalToXString(dlg.roomID().toStdString());
//        userID = LocalToXString(dlg.userID().toStdString());
//    }



    if (!userinfo.Appkey.empty())
        appkey = userinfo.Appkey.c_str();
    if (!userinfo.Appsecret.empty())
        appSecret = userinfo.Appsecret.c_str();
    ChatWidget w(roomID, userID, appkey, appSecret, userinfo.Isdebug);
    w.show();

    YIMErrorcode code = YIMManager::CreateInstance()->Init(appkey, appSecret, __XT(""));

    if (code == YIMErrorcode_Success) {
        qDebug() << QStringLiteral("调用初始化成功");
    }
    return a.exec();
}
