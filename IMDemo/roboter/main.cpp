
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QJsonDocument>
#include <QArgument>
#include <QJsonObject>
#include <thread>
#include "YIM.h"
#include "YIMPlatformDefine.h"
#include "mainwidget.h"

typedef struct Arguments {
    QString Username;
    QString RoomID;
    QString Passwd;
    QString Token;
    XString Appkey;
    XString Appsecret;
    bool IsDebug;
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
        if (obj.contains("Username")) {
            value = obj.take("Username");
            userinfo->Username = value.toString();
        }

        if (obj.contains("RoomID")) {
            value = obj.take("RoomID");
            userinfo->RoomID = value.toString();
        }

        if (obj.contains("Passwd")) {
            userinfo->Passwd = obj.take("Passwd").toString();
        }

        if (obj.contains("Token")) {
            userinfo->Token = obj.take("Token").toString();
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
            userinfo->IsDebug = obj.take("IsDebug").toBool();
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName(QObject::tr("Roboter"));

    g_Userinfo userinfo;

    parseArguments(&userinfo);
    if (userinfo.IsDebug)
        IM_SetMode(2);
    const XCHAR *appkey = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");
    const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");

    //const XCHAR *appkey = __XT("YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0");
    //const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");
    //const XCHAR *appkey = __XT("YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0");
    //const XCHAR *appSecret =
    //        __XT("r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=");

    if (!userinfo.Appkey.empty())
        appkey = userinfo.Appkey.c_str();
    if (!userinfo.Appsecret.empty())
        appSecret = userinfo.Appsecret.c_str();

    YIMErrorcode code1 = YIMManager::CreateInstance()->Init(appkey, appSecret, __XT(""));

    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (code1 == YIMErrorcode_Success) {
        qDebug() << QStringLiteral("调用初始化成功");
    }

    MainWidget w(userinfo.Username, userinfo.RoomID, userinfo.Passwd
                 , userinfo.Token);
    w.show();

    return app.exec();
}
