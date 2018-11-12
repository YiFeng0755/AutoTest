#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTranslator>
#include <QDir>
#include <QDateTime>
#include "YIM.h"
#include "YIMPlatformDefine.h"
#include "mainwidget.h"

#ifdef WIN32
    #include "DbgHelp.h"
    #include <windows.h>
#endif

#include <QDateTime>


void messageOuputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

       QMetaObject::invokeMethod(MainWidget::messageHandler(), "showMessages",
                                 Qt::QueuedConnection,
                                 Q_ARG(QByteArray, localMsg));
}

#ifdef WIN32
long ApplicationCrashHandler(EXCEPTION_POINTERS *pException) {

    QDateTime current_time = QDateTime::currentDateTime();
    QString current_date = current_time.toString("yyyy_MM_dd_hh_mm_ss");
    QString filename = "youme_" + current_date + ".dmp";
    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
    QString errFlag(QString::number(record->ExceptionFlags, 16));
    QString errPara(QString::number(record->NumberParameters, 16));

    qDebug() << "errCode: " << errCode;
    qDebug() << "errAddr: " << errAddr;
    qDebug() << "errFlag: " << errFlag;
    qDebug() << "errPara: " << errPara;

    LPCWSTR filename_pstr = (LPCWSTR)QString(filename).utf16();

    HANDLE hDumpFile = CreateFile(filename_pstr,
                       GENERIC_WRITE,
                                  0,
                                  NULL,
                                  CREATE_NEW,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
    if(hDumpFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
        CloseHandle(hDumpFile);

    } else {
        qDebug()<<"hDumpFile == null";
    }

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

#ifdef WIN32
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif
    app.setApplicationName("IMDemo");
    qInstallMessageHandler(messageOuputHandler);

    MainWidget w ;
    w.show();
    IM_SetMode(2);
    const XCHAR *appkey = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");
    const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");

    //const XCHAR *appkey = __XT("YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0");
    //const XCHAR *appSecret =
    //        __XT("r1+ih9rvMEDD3jUoU+nj8C7VljQr7Tuk4TtcByIdyAqjdl5lhlESU0D+SoRZ30sopoaOBg9EsiIMdc8R16WpJPNwLYx2WDT5hI/HsLl1NJjQfa9ZPuz7c/xVb8GHJlMf/wtmuog3bHCpuninqsm3DRWiZZugBTEj2ryrhK7oZncBAAE=");

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("tr.qm"), QLatin1String("_"), QLatin1String(":/translations")))
        app.installTranslator(&translator);

    YIMErrorcode code = YIMManager::CreateInstance()->Init(appkey, appSecret, __XT(""));
    if (code == YIMErrorcode_Success) {
        qDebug() << QStringLiteral("调用初始化成功");
        //qDebug() << "调用初始化成功";
    }
    return app.exec();
}
