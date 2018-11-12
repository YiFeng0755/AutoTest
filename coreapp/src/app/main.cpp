#include <QApplication>
#include "posthttprequest.h"
#include "hostsmanager.h"
#include "hostmanagerwidget.h"
#include "mainwidget.h"
#include "wordsgenerator.h"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QStringList>
#include <QTextCodec>
#include <QTranslator>

//test include ; may delete later
#include "cryptwidget.h"

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
       QMetaObject::invokeMethod(MainWidget::messageHandler(), "showMessages", Qt::QueuedConnection,
                                 Q_ARG(QByteArray, localMsg));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    app.setApplicationName("Sniffer");
    qInstallMessageHandler(messageOuputHandler);
    MainWidget w ;
    w.show();

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("tr.qm"), QLatin1String("_"), QLatin1String(":/translations")))
        app.installTranslator(&translator);

    return app.exec();
}
