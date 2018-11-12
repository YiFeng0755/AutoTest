#ifndef DIGYOUMEREQUESTCONFIG
#define DIGYOUMEREQUESTCONFIG

#include <QDialog>
#include <QModelIndex>

namespace Ui {
   class Dialog;
}

class QStandardItemModel;
class QSqlTableModel;
class ConfigDialog: public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

    void accept();
private slots:
    void loadConfigs();
    void updateAppkeys(QModelIndex index);
    void addNewRow();
    void addLineToDb();

    void apply();
    void removeSelectedRow();
    //新接口
    void saveConfigs();

private:
    void setup();

private:
    Ui::Dialog *ui;
    QSqlTableModel *m_model;
};

#endif // DIGYOUMEREQUESTCONFIG

