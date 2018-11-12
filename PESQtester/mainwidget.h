#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <condition_variable>
#include <mutex>

namespace Ui {
    class MainWidget;
}

class PESQThread;
class TestDelayThread;
class QProcess;
class OutputWindow;
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    static OutputWindow* messageHandler() {
        return m_outputWindow;
    }

    void setPesqDir(const QString & newDir);

private:
    void setupComboBox();
    void setupSliders();

    /*重置网络等操作*/

private slots:
    void syncSliders(); //同步滑块的值
    void syncCombos();
    void runPESQ();

    void detectStatus();
    void changePlayAndRecDir();
    void updateRecvTextEditor(QByteArray data);
    void updateStatus(QByteArray data);
    void updateStatusString(QString text); //更新进度文字状态
    void slot_setting();
    void slot_thread_finished();
    void setupScoreTable(QString, QString);
    void setupDelayTable(QString, QString);

    void setDelay(int value = 0);
    void delDelay(int value = 0);
    void setLoss(int value = 0);
    void delLoss(int value = 0);
    void setJitterDelay();
    void setCustomCmd();

    void exportToCsv();
    void clearTask();
private:
    Ui::MainWidget *ui;
    static OutputWindow *m_outputWindow; //        Console ouptut window

    std::mutex m_mutex;
    std::condition_variable m_cv;

    PESQThread *m_thread;
    QTimer *m_timer;

    bool m_init;
};

#endif // MAINWIDGET_H
