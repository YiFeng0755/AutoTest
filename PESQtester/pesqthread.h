#ifndef PESQTHREAD
#define PESQTHREAD

#include <QThread>
#include <QProcess>
#include "servernetworkcontroller.h"

#ifndef THREAD_STOP
#define THREAD_STOP {if (m_stopped) break;}
#endif


class PESQThread : public QThread
{
    Q_OBJECT
public:
    explicit PESQThread(QObject *parent = 0);
    ~PESQThread();

public slots:
    void stop() {
        m_stopped = true;
    }

Q_SIGNALS:
    void progress(int);
    void statusChanged(QString);
    void requestedServer();
    void setloss(int);
    void setdelay(int);
    void delloss(int);
    void deldelay(int);

    void pResultDataReady(QString, QString); //每一种网络状态下采集完后，收集的数据准备完毕
    void dResultDataReady(QString, QString); //每一种网络状态下采集完后，delay延迟数据收集
private:
    void run();

    void collectDataFromFile(QString cmdtitle, const QString& file);

    volatile bool m_stopped;
    int m_rounds; //每种网络状态下的轮次 设置默认值为20次
    int m_commands_number; // 命令行的总数
    bool m_is_delay_set_init = false;
    bool m_is_loss_set_init = false;

    QString m_PesqDir;
};


//class TestDelayThread : public QThread
//{
//    Q_OBJECT
//public:
//    TestDelayThread(QString refFileName = QString(),
//                    QString testFileName = QString(),
//                    QString recordExePath = QString(),
//                    QString waveAlignExePath = "",
//                    QObject *parent = 0);
//    ~TestDelayThread();

//public slots:
//    void stop() {
//        m_stopped = true;
//    }

//Q_SIGNALS:
//    void progress(int);
//    void resultDataReady(QString, QString);
//    void setloss(int);
//    void setdelay(int);
//    void delloss(int);
//    void deldelay(int);

//private:
//    void run();

//    QString m_reffile;
//    QString m_testfile;
//    QString m_rec_path;
//    QString m_wave_align_path;

//    QString m_exeDir;
//    volatile bool m_stopped;
//    int m_rounds;
//};


#endif // PESQTHREAD

