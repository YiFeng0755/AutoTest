#ifndef DLGQUECMDS
#define DLGQUECMDS

#include <QDialog>
#include <QMap>
/**
 * @brief The QueueCmdsDialog class
 * 配置全自动化的命令队列，返回一个容器对象
 */

class QListView;
class QueueCmdsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QueueCmdsDialog(QWidget *parent = nullptr);
    ~QueueCmdsDialog();

    /*这里将映射转化为字符串列表*/
    QStringList commandsList();

    void accept();
private:
    void init();

    QListView *m_listView;
    QMap<int, QString> m_commands;
};

#endif // DLGQUECMDS
