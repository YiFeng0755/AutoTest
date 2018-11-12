#ifndef PERFORMANCEWIDGET_H
#define PERFORMANCEWIDGET_H

#include <QDialog>
#include "../main/callbackwrapper.h"
#include <QProcess>
#include <thread>
#include <atomic>


namespace Ui {
class PerformanceWidget;
}


class QLabel;
class QPushButton;
class QTableView;
class QStandardItemModel;
class QComboBox;
class QSpinBox;
class QGroupBox;
class QRadioButton;
class QProgressBar;
class PerformanceWidget : public QDialog,
        public ImWrapper
{
    Q_OBJECT
public:
    typedef struct
    {
        qint64 msgid;
        qint64 send_time;
        qint64 robot_recv_time;
        qint64 recv_time;
        qint64 create_time;
        int size;
    } TestResult;

    explicit PerformanceWidget(QWidget *parent = 0, bool isDebug=false);
    ~PerformanceWidget();

    void OnRecvMessage(std::shared_ptr<IYIMMessage> message);
    void OnSendMessageStatus(XUINT64 requestID,
                             YIMErrorcode errorcode,
                             unsigned int sendTime, bool isForbidRoom,
                             int reasonType, XUINT64 forbidEndTime);
    virtual void OnLogin(YIMErrorcode errorcode, const XString& userID);
    virtual void OnLogout(YIMErrorcode errorcode);
    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID);
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID);
Q_SIGNALS:
    void progress(int);
    void timesChanged();//统计次数发生变化的发出这个信号

private slots:
    void slot_exe_perftest(); //calculate the mean delay by millisecs
    void slot_login();
    void slot_leave();
    void slot_writeToDisk();
    void slot_updateProgress(int);
    void slot_export2csv();
    void slot_updateLabelString();

private:
    void setup();
    void setupTable();

    QString getRandomStr(int n);

    QProcess *m_process;
    XString m_roomID;
    XString m_robotID;
    XString m_userID;

    std::atomic_int32_t m_send_times; //消息发出次数统计变量
    std::atomic_int32_t m_recv_times; //消息接收次数统计变量

    QStandardItemModel *m_model;
    QTableView *m_table_view;
    QLabel *m_status;
    QLabel *m_stat_send;
    QLabel *m_stat_recv;
    QLabel *m_percent_rate;//消息抵达率

    QPushButton *m_delay_test;
    QPushButton *m_voice_delay_test;
    QPushButton *m_btn_export;

    QComboBox *m_text_bytesize;
    QComboBox *m_voice_elapse;
    QSpinBox *m_rounds;
    QGroupBox *m_chattypeBox;
    QRadioButton *m_btn_private;
    QRadioButton *m_btn_room;
    QRadioButton *m_btn_multi;
    QProgressBar *m_progressbar;
    QList<TestResult> m_results;

    std::thread m_exe_thread;

    bool m_isDebug;
    Ui::PerformanceWidget *ui;
};

#endif // PERFORMANCEWIDGET_H
