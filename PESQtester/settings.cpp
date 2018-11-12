#include "settings.h"
#include "ui_settingdialog.h"
#include "ui_cmdlistwidget.h"

#include <QLayout>
#include <QStringListModel>
#include <QSettings>

CmdsWidget::CmdsWidget(QWidget *parent)
    :QWidget(parent),
      ui(new Ui::CmdsForm())
{
    ui->setupUi(this);
    setupList();
    connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(add_to_list()));
    connect(ui->btn_apply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->btn_remove, SIGNAL(clicked()), this, SLOT(remove_from_list()));
}

void CmdsWidget::add_to_list()
{
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->cmds_listView->model());
    int rows = model->rowCount();
    model->insertRow(rows);  
}

void CmdsWidget::remove_from_list()
{
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->cmds_listView->model());
    QModelIndex index = ui->cmds_listView->currentIndex();
    if (!index.isValid()) {
        return;
    }
    model->removeRow(index.row());
}

void CmdsWidget::apply()
{
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->cmds_listView->model());
    if (!model) return;
    QStringList cmdlist = model->stringList();
    GlobalSettings::instance()->setCommandList(cmdlist);

    int value = ui->rounds_spinBox->value();
    GlobalSettings::instance()->setRounds(value);
}

void CmdsWidget::setupList()
{
    QStringList commandslist;
    commandslist << "tc qdisc show"
                 << "tc qdisc add dev enp3s0 root netem loss 5%"
                 << "tc qdisc change dev enp3s0 root netem loss 10%"
                 << "tc qdisc change dev enp3s0 root netem loss 20%"
                 << "tc qdisc change dev enp3s0 root netem loss 30%"
                 << "tc qdisc change dev enp3s0 root netem loss 50%"
                 << "tc qdisc change dev enp3s0 root netem loss 70%"
                 << "tc qdisc del dev enp3s0 root netem loss 10%"
                 << "tc qdisc add dev enp3s0 root netem delay 50ms 10ms"  // 抖动10ms
                 << "tc qdisc change dev enp3s0 root netem delay 100ms 20ms" // 抖动20ms
                 << "tc qdisc change dev enp3s0 root netem delay 200ms 50ms" // 抖动50ms
                 << "tc qdisc change dev enp3s0 root netem delay 400ms 80ms" // 抖动80ms
                 << "tc qdisc del dev enp3s0 root netem delay 400ms 100ms"; // 抖动100ms

    QStringList operate_loss_actions;
    operate_loss_actions << "tc qdisc add dev enp3s0 root netem loss"
            << "tc qdisc change dev enp3s0 root netem loss"
            << "tc qdisc del dev enp3s0 root netem loss"
            << "null";

    QStringList operate_delay_actions;
    operate_delay_actions << "tc qdisc add dev enp3s0 root netem delay"
                         << "tc qdisc change dev enp3s0 root netem delay"
                         << "tc qdisc del dev enp3s0 root netem delay"
                         << "null";

    QStringListModel *model = new QStringListModel(this);

    if (GlobalSettings::instance()->getCommandList().isEmpty())
        model->setStringList(commandslist);
    else
        model->setStringList(GlobalSettings::instance()->getCommandList());

    ui->cmds_listView->setModel(model);

    ui->rounds_spinBox->setValue(GlobalSettings::instance()->rounds());
}

SettingDialog::SettingDialog(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    setupCmdWidget();
}

void SettingDialog::setupCmdWidget()
{
    //ui->tabWidget->addTab(new QWidget(), tr("&ParamsWidget"));
    ui->tabWidget->addTab(new CmdsWidget(), tr("&CmdWidget"));
}

QStringList SettingDialog::commands()
{
    int widget_cnt = ui->tabWidget->count();
    if (widget_cnt <= 0) //防止越界
        return QStringList();
    CmdsWidget* widget = qobject_cast<CmdsWidget*>(ui->tabWidget->widget(0));
    if (!widget)
        return QStringList();

    QStringListModel *model = qobject_cast<QStringListModel*>(widget->ui->cmds_listView->model());
    if (!model)
        return QStringList();
    return model->stringList();
}

void SettingDialog::accept()
{
    QStringList stringlist = commands();

    GlobalSettings::instance()->setCommandList(stringlist);
    CmdsWidget* widget = qobject_cast<CmdsWidget*>(ui->tabWidget->widget(0));
    if (!widget)
        return;
    widget->apply();

    return QDialog::accept();
}

void GlobalSettings::readSettings()
{
    QSettings settingfiles("pesqconfig.ini", QSettings::IniFormat);
    settingfiles.beginGroup("Default");
    m_commands = settingfiles.value("Commands").toStringList();
    m_state_intervals = settingfiles.value("ServerStateIntervals").toInt();
    m_rounds = settingfiles.value("Rounds", 20).toInt();
    settingfiles.endGroup();
}

void GlobalSettings::saveSettings()
{
    QSettings settingfiles("pesqconfig.ini", QSettings::IniFormat);
    settingfiles.beginGroup("Default");
    settingfiles.setValue("Commands", m_commands);
    settingfiles.setValue("ServerStateIntervals", m_state_intervals);
    settingfiles.setValue("Rounds", m_rounds);
    settingfiles.endGroup();
}

void GlobalSettings::setCommandList(const QStringList &cmds)
{
    m_commands = cmds;
}

void GlobalSettings::setStateIntervals(int secs)
{
    m_state_intervals = secs;
}

int GlobalSettings::getStateIntervals()
{
    return m_state_intervals;
}

QString GlobalSettings::getScoreRecCmdDirectory() const
{
    return QString("../PESQtools/tools/PlayAndRec2.exe"); //这个后续可以设置 目前写死它
}

QString GlobalSettings::getWavDelayRecCmdDirectory() const
{
    return QString("../PESQtools/tools/DELAY/PESQ.exe");
}

QString GlobalSettings::getWavDelayCmdDirectory() const
{
    return QString("../PESQtools/tools/WaveAlign.exe");
}
