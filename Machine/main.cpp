#include "mainwidget.h"
#include "YIM.h"

#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QTextCodec>
#include <QString>

void messageOuputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QByteArray localMsg = msg.toLocal8Bit();

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

//       QFile file("mylog.log");
//       if (!file.open(QIODevice::Append)) {
//           printf("Error ,cannot open file");
//           return ;
//       }

//       QTextStream s(&file);
//       s << localMsg << "\n";

//       file.close();
       QMetaObject::invokeMethod(MainWidget::messageHandler(),
                                 "showMessages", Qt::QueuedConnection,
                                 Q_ARG(QByteArray, localMsg));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    app.setApplicationName(QObject::tr("RobotChater"));
    MainWidget w;
    w.show();
    qInstallMessageHandler(messageOuputHandler);
    const XCHAR *appkey = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");
    const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");

    const XCHAR *appkey_online = __XT("YOUME670584CA1F7BEF370EC7780417B89BFCC4ECBF78");
    const XCHAR *app_secret_online =
            __XT("yYG7XY8BOVzPQed9T1/jlnWMhxKFmKZvWSFLxhBNe0nR4lbm5OUk3pTAevmxcBn1mXV9Z+gZ3B0Mv/MxZ4QIeDS4sDRRPzC+5OyjuUcSZdP8dLlnRV7bUUm29E2CrOUaALm9xQgK54biquqPuA0ZTszxHuEKI4nkyMtV9sNCNDMBAAE=");
    YIMErrorcode code = YIMManager::CreateInstance()->Init(appkey, appSecret, __XT(""));
    if (code != YIMErrorcode_Success) {
        qDebug() << "Attend to init youme sdk error!";
        qDebug() << "The return code is " << code;
    }


    qsrand(QDateTime::currentDateTime().toTime_t());
    return app.exec();
}
