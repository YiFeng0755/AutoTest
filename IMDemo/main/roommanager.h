#ifndef ROOMMANAGERDIALOG_H
#define ROOMMANAGERDIALOG_H

#include <QDialog>
#include "YIM.h"
#include "callbackwrapper.h"
#include <QProcess>

namespace Ui {
class RoomManager;
}

class HttpRequester;
class RoomManagerDialog : public QDialog, public ImWrapper
{
    Q_OBJECT

public:
    explicit RoomManagerDialog(XString roomID, QWidget *parent = 0);
    ~RoomManagerDialog();

    void OnGetRoomMemberCount(YIMErrorcode errorcode,
                              const XString &chatRoomID, unsigned int count);

private slots:
    void slot_room_cnt();
    void slot_kickoff();
    void slot_add_robot_in_room();
    void slot_room_list();
    void slot_setup_data(QByteArray);

private:
    void setUp();

    Ui::RoomManager *ui;
    XString m_roomID;
    HttpRequester *m_requester;
    QProcess *m_process;
};

#endif // ROOMMANAGERDIALOG_H
