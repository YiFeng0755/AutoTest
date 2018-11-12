#include "mainwidget.h"
#include "outputwindow.h"
#include <QApplication>
#include <QTextCodec>

void messageOuputHandler(QtMsgType type, const QMessageLogContext &context,
                         const QString &msg)
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

    QMetaObject::invokeMethod(MainWidget::messageHandler(),
                              "showMessages", Qt::QueuedConnection,
                              Q_ARG(QByteArray, localMsg));
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    app.setApplicationName("PESQTester");
    app.setApplicationVersion("0.9.0alpha");

    qInstallMessageHandler(messageOuputHandler);

    MainWidget w;
    w.show();

    return app.exec();
}
