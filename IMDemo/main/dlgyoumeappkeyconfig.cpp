
#include "dlgyoumeappkeyconfig.h"
#include "ui_dialogforconfig.h"

#include <qpushbutton.h>
#include "globalsetting.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QtSql>
#include <QSqlTableModel>
#include <YIM.h>
#include "database.h"

ConfigDialog::ConfigDialog(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    m_model = nullptr;

    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pushButton_accept, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(saveConfigs()));
    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply()));
    connect(ui->toolButton, SIGNAL(clicked(bool)), this, SLOT(addNewRow()));
    connect(ui->toolButton_2, SIGNAL(clicked(bool)), this, SLOT(removeSelectedRow()));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addLineToDb()));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateAppkeys(QModelIndex)));
    this->loadConfigs();
}

void ConfigDialog::updateAppkeys(QModelIndex index)
{
    if (!index.isValid()) {
        return;
    }
    int row = index.row();
    //int appid = m_model->data(QModelIndex(row, 1)).toInt();
    QString appkey = m_model->data(m_model->index(row, 2)).toString();
    QString appsecret = m_model->data(m_model->index(row, 3)).toString();
    ui->edit_im_appkey->setText(appkey);
    ui->edit_im_app_secret->setText(appsecret);
}

void ConfigDialog::loadConfigs()
{
   // bool test = ui->radioButton_test->isChecked();
    //从文件中读
    ui->edit_im_appkey->setText(GlobalSettings::getInstance()->getIMAppkey(true));
    ui->edit_im_app_secret->setText(GlobalSettings::getInstance()->getIMAppSecret(true));

    //从数据库读
    setup();
}

void ConfigDialog::saveConfigs()
{
    if (ui->edit_im_appkey->text().isEmpty()) {
        return;
    }
    GlobalSettings::getInstance()->setIMAppkey(ui->edit_im_appkey->text().toLatin1()
                                               , true);
    GlobalSettings::getInstance()->setIMAppSecret(ui->edit_im_app_secret->text().toLatin1()
                                                     , true);

}

void ConfigDialog::accept()
{
    saveConfigs();
    YIMManager *im = YIMManager::CreateInstance();
    QByteArray bappkey = GlobalSettings::getInstance()->getIMAppkey(true);
    XString xappkey = LocalToXString(bappkey.toStdString());

    const XCHAR *appkey = xappkey.c_str();

    QByteArray bappsecret = GlobalSettings::getInstance()->getIMAppSecret(true);
    XString xappsecret = LocalToXString(bappsecret.toStdString());

    const XCHAR *appsecret = xappsecret.c_str();

    YIMErrorcode code = im->Init(appkey, appsecret, __XT(""));
    if (code != YIMErrorcode_Success) {
        qDebug() << QStringLiteral("重置appkey 初始化失败！错误码: ") << code;
    } else {
        qDebug() << QStringLiteral("更换appkey, 初始化成功！");
    }
    return QDialog::accept();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::setup()
{
    if (m_model == nullptr) {
        m_model = new QSqlTableModel(this);
    }
    SqlManager::createInstance()->getSb();

    m_model->setTable("apps");
    m_model->select();
    int c = m_model->columnCount();
    //m_model->removeColumn(0);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
//    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
//            this, SLOT(removeSelectedRow(QModelIndex)));
    ui->tableView->hideColumn(0);
}

void ConfigDialog::addNewRow()
{
    m_model->insertRow(m_model->rowCount());
}

void ConfigDialog::apply()
{
    int row = m_model->rowCount();
    int appid = m_model->data(m_model->index(row, 1)).toInt(); //这里隐藏一个自增id哦
    QString appkey = m_model->data(m_model->index(row, 2)).toString();
    QString appsecret = m_model->data(m_model->index(row, 3)).toString();

    QSqlRecord record;

    record.setValue("appid", appid);
    record.setValue("appkey", appkey);
    record.setValue("appsecret", appsecret);

    m_model->insertRecord(row, record);
    m_model->submitAll();
}

void ConfigDialog::addLineToDb()
{
    qDebug() << "Try to add new row to db";
    QString appkey = ui->edit_im_appkey->text();
    QString appSec = ui->edit_im_app_secret->text();

    if (appkey.isEmpty() || appSec.isEmpty()) {
        return;
    }

    int row = m_model->rowCount();
    for (int i = 0; i < row; ++i) {
        QString text = m_model->data(m_model->index(i, 2)).toString();
        if (appkey == text) {
            qDebug() << tr("There is already an key named %1").arg(appkey);
            return;
        }
    }

    int appid = 0;
    QSqlRecord record;

    record.setValue("appid", appid);
    record.setValue("appkey", appkey);
    record.setValue("appsecret", appSec);
    m_model->insertRecord(row, record);
    m_model->insertRow(row);
    m_model->setData(m_model->index(row, 1), appid);
    m_model->setData(m_model->index(row, 2), appkey);
    m_model->setData(m_model->index(row, 3), appSec);
}

void ConfigDialog::removeSelectedRow()
{
    if (QMessageBox::information(this, tr("info"), tr("Are you sure to delete this row"), QMessageBox::Ok |
                                              QMessageBox::Cancel)
                        != QMessageBox::Ok)
        return;
    //注意，这里的行数是不断变化的，所以下标的递增有讲究
    QModelIndex index = ui->tableView-> currentIndex();
    if (!index.isValid())
        return;
    m_model->removeRow(index.row());
}
