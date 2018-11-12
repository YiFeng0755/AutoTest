#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
#include <QList>
#include <QProcess>
#include "../main/callbackwrapper.h"
#include "../main/outputwindow.h"

namespace Ui {
class ChatForm;
}

class HttpRequester;
//class OutputWindow;
class ChatWidget
        : public QWidget
        , public ImWrapper
{
    Q_OBJECT
public:
    typedef struct
    {
        QString room_id;
        QString user_id;
    } UserRoomIDPair;

    explicit ChatWidget(const XString& roomID,
                                   const XString& userID,
                        const XString& appkey,
                        const XString& appsecret,
                        int serverMode=1,//1 测试环境
                                   QWidget *parent = 0);
    ~ChatWidget();

    static OutputWindow * messageHandler() {
        return m_outputWindow;
    }

    void OnRecvMessage(std::shared_ptr<IYIMMessage> message);
    void OnStopAudioSpeechStatus(YIMErrorcode errorcode,
                                 std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo);
    void OnStartSendAudioMessage(XUINT64 requestID,
                                 YIMErrorcode errorcode,
                                 const XString &text, const XString &audioPath,
                                 unsigned int audioTime);
    void OnSendMessageStatus(XUINT64 requestID,
                             YIMErrorcode errorcode,
                             unsigned int sendTime,
                             bool isForbidRoom,
                             int reasonType, XUINT64 forbidEndTime);

    void OnLogin(YIMErrorcode errorcode, const XString &userID);
    void OnJoinChatRoom(YIMErrorcode errorcode, const XString &chatRoomID);
    void OnLeaveChatRoom(YIMErrorcode errorcode, const XString &chatRoomID);
    //void OnLeaveAllChatRooms(YIMErrorcode errorcode);
    void OnUserJoinChatRoom(const XString& chatRoomID, const XString& userID);
    void OnUserLeaveChatRoom(const XString& chatRoomID, const XString& userID);
    void OnLogout(YIMErrorcode errorcode);

    void OnGetRoomMemberCount(YIMErrorcode errorcode,
                              const XString &chatRoomID, unsigned int count);
    void OnReceiveMessageNotify(YIMChatType chatType,  const XString& targetID);

    void OnQueryRoomHistoryMessageFromServer(YIMErrorcode errorcode,
                                             const XString &roomID,
                                             int remain,
                                             std::list<std::shared_ptr<IYIMMessage> > &messageList);
    void OnQuerySessionHistoryMsgFromServer(YIMErrorcode errorcode,
                                            const XString &userID,
                                            std::list<std::shared_ptr<IYIMMessage> > &messageList);
    void OnQueryHistoryMessage(YIMErrorcode errorcode,
                               const XString &targetID,
                               int remain, std::list<std::shared_ptr<IYIMMessage> > messageList);
    void OnQueryUserStatus(YIMErrorcode errorcode,
                           const XString &userID,
                           YIMUserStatus status);

    void OnGetNearbyObjects(YIMErrorcode errorcode,
                            std::list<std::shared_ptr<RelativeLocation> > neighbourList,
                            unsigned int startDistance, unsigned int endDistance);
    void OnGetRecentContacts(YIMErrorcode errorcode,
                             std::list<std::shared_ptr<IYIMContactsMessageInfo> >& contactList);
    void OnGetBlockUsers(YIMErrorcode errorcode, std::list<XString> userList);
    void OnGetDistance(YIMErrorcode errorcode, const XString &userID, unsigned int distance);
    void OnGetMicrophoneStatus(AudioDeviceStatus status);
    void OnGetRecognizeSpeechText(XUINT64 requestID, YIMErrorcode errorcode,
                                  const XString &text);
    void OnGetUserInfo(YIMErrorcode errorcode, const XString &userID,
                       const XString &userInfo);
private slots:
    void slot_send_message();
    void slot_start_record();
    void slot_stop_and_send_voice();
    void slot_create_new_robot();
    void slot_login();
    void slot_join_room();
    void slot_logout();
    void slot_leave_all_room();
    void slot_leave_room();

    void slot_room_count();
    void slot_room_list();
    void slot_add_robot_in_room();
    void slot_delete_robot();
    void slot_setup_data(QByteArray data);

    void slot_change_state(int state);
    void slot_get_message();

    void slot_room_history_messages();
    void slot_private_history_messages();
    void slot_get_historyr_message();
    void slot_get_historyp_message();
    void slot_recent_contacts();
    void slot_block_user();
    void slot_unblock_user();
    void slot_get_block_users();
    void slot_get_user_info();

private:
    void setUp();
    void setup_init();
    void setup_roommanager();
    void setup_message_ctrl();
    void setup_user_ctrl();

    Ui::ChatForm *ui;

    XString m_roomID;
    XString m_userID;
    XString m_robotID;
    QList<QProcess*> m_processes;
    HttpRequester *m_requester;
    static OutputWindow *m_outputWindow;
    QList<UserRoomIDPair> m_user_rooms;

    bool is_testMode = true;
    XString m_appkey;
    XString m_appSecret;
};


#endif // CHATDIALOG_H
