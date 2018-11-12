#ifndef STABLETESTDIALOG
#define STABLETESTDIALOG

#include "YIM.h"
#include "callbackwrapper.h"
#include <QDialog>
#include <thread>
#include <atomic>
#include <QMutex>
#include <QMap>
#include <QProcess>

namespace Ui {
class StableTestForm;
}

class StableTestDialog : public QDialog
        , public ImWrapper
{
    Q_OBJECT
public:

    explicit StableTestDialog(QWidget *parent = 0);
    ~StableTestDialog();

    void OnLogin(YIMErrorcode errorcode, const XString& userID);
    //登出回调
    void OnLogout(YIMErrorcode errorcode);
    void OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID);
    void OnSendAudioMessageStatus(XUINT64 requestID,
                                  YIMErrorcode errorcode,
                                  const XString &text,
                                  const XString &audioPath,
                                  unsigned int audioTime,
                                  unsigned int sendTime,
                                  bool isForbidRoom,
                                  int reasonType, XUINT64 forbidEndTime);
    void OnStartSendAudioMessage(XUINT64 requestID,
                                 YIMErrorcode errorcode,
                                 const XString &text,
                                 const XString &audioPath,
                                 unsigned int audioTime);
    void OnSendMessageStatus(XUINT64 requestID,
                             YIMErrorcode errorcode,
                             unsigned int sendTime,
                             bool isForbidRoom,
                             int reasonType, XUINT64 forbidEndTime);
    void OnStopAudioSpeechStatus(YIMErrorcode errorcode,
                                 std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo);


    void OnStartReconnect();

    void OnRecvMessage(std::shared_ptr<IYIMMessage> message);
private slots:
    void slot_voice_interface();
    void slot_text_interface();
    void slot_other_interface();
    void slot_stop_all_threads();
    void slot_reconnect_interface();
    void slot_updateStatData();

    void slot_exportData();
private:
    void createNewRoboter();
    void setup();
    QString getRandomStr(int n);

    XString m_userID;
    XString m_roomID;
    XString m_robotID;

    volatile bool m_stop;

    Ui::StableTestForm *ui;
    //std::atomic_int32_t i;
    QMap<QString, int> m_stat_map; //统计接口的调用次数
    QProcess *m_process;
    QMutex m_mutex;
    QTimer *m_timer;

};

#endif // STABLETESTDIALOG

