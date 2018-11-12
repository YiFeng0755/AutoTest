#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "outputwindow.h"
#include "dlgyoumeappkeyconfig.h"
#include "performancewidget.h"
#include "globalsetting.h"
#include "database.h"
#include "dlgperftest.h"
#include "stabletestdialog.h"
#include <QStringListModel>
#include <QProcess>
#include <QLayout>
#include <QDebug>

OutputWindow* MainWidget::m_outputWindow = NULL;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    chatter_process = new QProcess(this);
    setup();
}

void MainWidget::slot_function_widget()
{
    QString cmd = "./chatter.exe";
    QStringList arguments;

    QString userID = QString("TestUser") + QString::number(qrand() % 100000);

    //QString robotUser = QString("robot_perf_") + QString::number(qrand() % 100000 + 500000);

    QString roomID = QString("201") + QString::number(qrand() % 10000 + 1000);

    QByteArray appkey = GlobalSettings::getInstance()->getIMAppkey();
    QByteArray appsecret = GlobalSettings::getInstance()->getIMAppSecret();

    bool isDebug = ui->mode_comboBox->currentIndex() == 1;
    arguments << QString("{\"UserID\":\"%1\",\"RoomID\":\"%2\",\"Passwd\":\"%3\","
                         " \"Token\":\"\",\"Appkey\":\"%4\",\"Appsecret\":\"%5\",\"IsDebug\":\"%6\"}")
            .arg(userID)
            .arg(roomID)
            .arg("123456")
            .arg(QString(appkey))
            .arg(QString(appsecret))
            .arg(QString::number(isDebug));


    qDebug() << arguments;
    //QProcess::execute(cmd, arguments);
    chatter_process->start(cmd, arguments);
}

void MainWidget::slot_config_widget()
{
    ConfigDialog dlg(this);
    dlg.exec();
    update_label_status();
}

void MainWidget::slot_change_server(int index)
{

    QStringListModel *model = qobject_cast<QStringListModel*>(ui->zone_comboBox->model());
    if (!model)
        return;
    QModelIndex i = model->index(index);
    ServerZone zone = (ServerZone)model->data(i, Qt::UserRole + 1).toInt();

    YIMManager *im = YIMManager::CreateInstance();
    im->SetServerZone(zone);
    qDebug() << "Server zone change to " << model->data(i, Qt::DisplayRole).toString();
    update_label_status();
}

void MainWidget::slot_switch_mode()
{
   // ui->mode_comboBox->
    int idx = ui->mode_comboBox->currentIndex();
    QByteArray array_appkey;
    QByteArray array_secret;
    if (idx == 0) {// online
        IM_SetMode(2);


        array_appkey = GlobalSettings::getInstance()->getIMAppkey(true);
        array_secret = GlobalSettings::getInstance()->getIMAppSecret(true);

        qDebug() << "appkey: " << array_appkey << ", " << "appsecret " << array_secret;
        qDebug() << QStringLiteral("切换到测试环境");

    }
    else if (idx == 1 || idx == 2) {
        IM_SetMode(0);
        array_appkey = GlobalSettings::getInstance()->getIMAppkey(true);
        array_secret = GlobalSettings::getInstance()->getIMAppSecret(true);
        qDebug() << "appkey: " << array_appkey << ", " << "appsecret " << array_secret;
        qDebug() << QStringLiteral("切换到正式环境");
    }

    std::string stdAppkey = array_appkey.toStdString();
    std::string stdAppsecret = array_secret.toStdString();
    XString xappkey = LocalToXString(stdAppkey);
    const XCHAR *appkey = xappkey.c_str();
    XString xappsecret = LocalToXString(stdAppsecret);
    const XCHAR *appsecret = xappsecret.c_str();
    YIMManager::CreateInstance()->Init(appkey,
                                       appsecret
                                       , __XT(""));
}

void MainWidget::update_label_status()
{
    ui->label_appkey->setText(GlobalSettings::getInstance()->getIMAppkey());
    ui->label_version->setText(QString::number(YIMManager::CreateInstance()->GetSDKVersion()));
    // ...
}

void MainWidget::slot_perftest_widget()
{
    // 杀死 启动进程
    int idx = ui->mode_comboBox->currentIndex();
    PerformanceWidget w(this, (idx == 0) ? 1 : 0);
    w.exec();
}

void MainWidget::slot_stabletest_widget()
{
    StableTestDialog dlg(this);
    dlg.exec();
}

void MainWidget::setup()
{
    setMinimumSize(800,600);
    setWindowTitle(QStringLiteral("IM测试demo"));

    setWindowIcon(QIcon(":/images/appicon.png"));

    m_outputWindow = new OutputWindow();

    QVBoxLayout * mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        qDebug() << "error layout is not exist";
        return;
    }

    //init db
    SqlManager::createInstance()->getSb();

    setupServer();
    setupMode();

    mainLayout->addWidget(m_outputWindow);

    connect(ui->toolButton_funcitons, SIGNAL(clicked(bool)),
            this, SLOT(slot_function_widget()));
    connect(ui->toolButton_perftest, SIGNAL(clicked(bool)),
            this, SLOT(slot_perftest_widget()));
    connect(ui->toolButton_stable, SIGNAL(clicked(bool)),
            this, SLOT(slot_stabletest_widget()));
    connect(ui->btn_appkey_setting, SIGNAL(clicked(bool)),
            this, SLOT(slot_config_widget()));
    connect(ui->mode_comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slot_switch_mode()));

    update_label_status();
}

void MainWidget::setupServer()
{
    QStringListModel *model = new QStringListModel;

    model->setData(model->index(0), ServerZone_China, Qt::UserRole + 1);
    model->setData(model->index(1), ServerZone_Singapore, Qt::UserRole + 1);
    model->setData(model->index(2), ServerZone_America, Qt::UserRole + 1);
    model->setData(model->index(3), ServerZone_HongKong, Qt::UserRole + 1);
    model->setData(model->index(4), ServerZone_Korea, Qt::UserRole + 1);
    model->setData(model->index(5), ServerZone_Australia, Qt::UserRole + 1);
    model->setData(model->index(6), ServerZone_Deutschland, Qt::UserRole + 1);
    model->setData(model->index(7), ServerZone_Brazil, Qt::UserRole + 1);
    model->setData(model->index(8), ServerZone_India, Qt::UserRole + 1);
    model->setData(model->index(9), ServerZone_Japan, Qt::UserRole + 1);
    model->setData(model->index(10), ServerZone_Ireland, Qt::UserRole + 1);
    model->setData(model->index(11), ServerZone_Thailand, Qt::UserRole + 1);
    model->setData(model->index(12), ServerZone_Taiwan, Qt::UserRole + 1);

    model->setStringList(QStringList() << QStringLiteral("中国") << QStringLiteral("新加坡")
                         << QStringLiteral("美国") << QStringLiteral("香港") << QStringLiteral("韩国")
                         << QStringLiteral("澳洲") << QStringLiteral("德国") << QStringLiteral("巴西")
                         << QStringLiteral("印度") << QStringLiteral("日本") << QStringLiteral("爱尔兰")
                         << QStringLiteral("泰国") << QStringLiteral("台湾"));

    ui->zone_comboBox->setModel(model);
    connect(ui->zone_comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slot_change_server(int)));
}

void MainWidget::setupMode()
{
    QStringListModel *model = new QStringListModel;
    model->setStringList(QStringList() << QStringLiteral("测试服")
                         << QStringLiteral("正式服")
                         << QStringLiteral("测试服2"));
    model->setData(model->index(0), 2, Qt::UserRole + 1);
    model->setData(model->index(1), 0, Qt::UserRole + 1);
    model->setData(model->index(2), 9999, Qt::UserRole + 1);

    ui->mode_comboBox->setModel(model);
}

MainWidget::~MainWidget()
{
    delete ui;
}
