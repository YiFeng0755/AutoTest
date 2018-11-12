#include "pesqthread.h"
#include "settings.h"
#include <QDir>
#include <QRegExp>

PESQThread::PESQThread(QObject *parent)
    :QThread(parent)
{
    m_PesqDir = QString("./PESQtools/tools");
    QStringList commands = GlobalSettings::instance()->getCommandList();
    m_commands_number = commands.count();
    m_rounds = GlobalSettings::instance()->rounds();//默认是20
    m_is_delay_set_init = m_is_loss_set_init = false;
}

PESQThread::~PESQThread()
{
}

void PESQThread::run()
{
    m_stopped = false;

    int cmd_cnt = 1;
    QString dirPath = QDir(m_PesqDir).absolutePath();
    QString cmd_rec = dirPath + "/PlayAndRec2.exe";
    QString cmd_pesq = dirPath + "/PESQ.exe";
    QString tempfile_prefix = "TEMPWAVFILE";
    QString source_test_file = dirPath + "/test.wav";

    QString wavdirPath = QDir("./PESQtools/tools/DELAY").absolutePath();
    QString cmd_wav = wavdirPath + "/WaveAlign.exe";
    QString cmd_rec_delay = wavdirPath + "/PlayAndRec2.exe";
    QString refFilename = wavdirPath + "/ref_delay.wav";
    QString wavTestFilename = wavdirPath +  "/test.wav";

    QStringList commands = GlobalSettings::instance()->getCommandList();
    m_rounds = GlobalSettings::instance()->rounds();
    emit progress(0);
    foreach (QString command, commands) {
        //parse command 远程控制一下服务器之后 开始运行
        if (m_stopped) break;
        emit statusChanged(command);
        QDir dir = QDir::currentPath();
        if (QFileInfo::exists("pesq_results.txt")
                && !dir.remove("pesq_results.txt")) {
            qDebug() << "Remove file pesq_results.txt failed!";
            break;
        }

        QString cmd_digest;
        if (command.contains("loss")) {
            if (command.contains(" del ")) {
                emit delloss(0);
                continue;
            } else {
                QRegExp pattern("\\d+%");
                int pos;
                if ((pos = pattern.indexIn(command)) > -1) {
                    QStringList texts = pattern.capturedTexts();
                    if (texts.count() > 0)
                    {
                        QString numberstr = texts.at(0);
                        QString num = numberstr.remove("%");
                        int number = num.toInt();
                        cmd_digest = ("loss " + numberstr);
                        emit setloss(number);
                    }
                    else {
                        emit setloss(0);
                    }
                }
            }
        } else if (command.contains("delay")) {
            if (command.contains(" del ")) {//这里不严格 因为单词delay也有del所以前后加空格处理
                emit deldelay(0);
                continue;
            } else {
                QRegExp p("\\d+ms");
                int pos;
                if ((pos = p.indexIn(command)) > -1) {
                    QStringList texts = p.capturedTexts();
                    if (texts.count() > 0) {
                        QString str = texts.at(0);
                        int number = str.remove("ms").toInt();
                        cmd_digest = ("delay " + str);
                        emit setdelay(number);
                    }
                } else {
                    emit setdelay(0);
                }
            }
        }
        THREAD_STOP
        emit requestedServer();

        /*
         *每次执行完命令，需要收集一次结果，这个结果的目录在  ./bin/pesq_results.txt中
         * 采集完将数据存入内存，封装成数据结构穿到ui；并清理 pesq_results.txt
         */
        int normal_flag = false; //循环是否正常结束，如果被打断，则后续步骤直接退出
        for (int i = 1; i <= m_rounds; ++i) {
            if (m_stopped) break;

            emit progress(m_rounds * (cmd_cnt - 1) + i);
            //ui->progressBar->valueChanged(i * cmd_cnt);
            QString basewavefile = tempfile_prefix;
            QString wavefile = dirPath + QString("/%1%2.wav").arg(basewavefile).arg(i);

            QStringList arg1, arg2;
            arg1 << source_test_file << wavefile;
            arg2 << "+16000" << source_test_file << wavefile;
            qDebug() << cmd_pesq;
            THREAD_STOP
            int code = QProcess::execute(cmd_rec, arg1);
            THREAD_STOP

            qDebug() << cmd_rec << "Exit code = " << code;

            //执行评分命令
            THREAD_STOP
            int code2 = QProcess::execute(cmd_pesq, arg2);
            THREAD_STOP

            qDebug() << cmd_pesq << "Exit code = " << code2;
            normal_flag = true;

        }
        //执行一种网络状态收集一次
        if (!normal_flag) //非正常退出 直接退出大循环，否则会出现崩溃问题
            break;

        collectDataFromFile(cmd_digest, "pesq_results.txt");
        //计算延迟
        QString results;
        for (int i = 1; i <= m_rounds; ++i) {
            THREAD_STOP

            emit progress(m_rounds * (cmd_cnt - 1) + i + 1);
            QStringList args;
            args << refFilename << wavTestFilename;

            QProcess process;

            THREAD_STOP
            process.start(cmd_rec_delay, args);
            if (!process.waitForStarted())
                continue;

            if (!process.waitForFinished())
                continue;
            THREAD_STOP
            process.start(cmd_wav, args, QProcess::ReadWrite);
            if (!process.waitForStarted())
                continue;

            if (!process.waitForFinished())
                continue;
            THREAD_STOP

            results += process.readAll();
            process.close();
        }
        THREAD_STOP

        emit dResultDataReady(cmd_digest, results);
        ++cmd_cnt;
    }
}

void PESQThread::collectDataFromFile(QString cmdtitle, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QString all = file.readAll();
    file.close();
    emit pResultDataReady(cmdtitle, all);
}
