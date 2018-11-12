
#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>

#include "YIM.h"
#include "YIMPlatformDefine.h"

#include "test_suit1.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    //QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);



    app.setApplicationName(QObject::tr("IMApiTester"));

    //IMApiWidget w;
    //w.show();
    IM_SetMode(2);
    //const XCHAR *appkey = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");
    //const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");


    const XCHAR *appkey = __XT("YOUMEBC2B3171A7A165DC10918A7B50A4B939F2A187D0");
    const XCHAR *appSecret = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");

    YIMErrorcode code = YIMManager::CreateInstance()->Init(appkey, appSecret, __XT(""));
    if (code == YIMErrorcode_Success) {
        qDebug() << QStringLiteral("调用初始化成功");
        //qDebug() << "调用初始化成功";
    }

    TestSuit1 *tester = new TestSuit1();
    tester->userlogin();

   // tester->sendPrivateMsg();
   // tester->test_QuerySessionHistoryMsgFromServer();

    return app.exec();
}
