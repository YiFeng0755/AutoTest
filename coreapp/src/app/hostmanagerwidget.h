#ifndef HOSTMANAGERWIDGET_H
#define HOSTMANAGERWIDGET_H

#include <QDialog>

class HostManager ;
class QTableView ;
class QToolBar ;
class QTextEdit ;
class QStandardItemModel ;
class HostManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HostManagerDialog(QWidget * parent = 0);
    ~HostManagerDialog();

private slots:
    void loadHostFile();
    void addItem();
    void removeItem();
    void accept();
    void reload();
    //void accept();
private:
    void setup();
    void initializeTableView();
    void createToolButtons();
    void loadDataFromFile();

private:
    QTextEdit * m_fileView ;
    HostManager * m_manager ;
    QStandardItemModel  * m_model ;
    QTableView * m_tableview ;
    QToolBar * m_toolBar ;
};

#endif // HOSTMANAGERWIDGET_H
