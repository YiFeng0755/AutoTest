#include "mainwidget.h"
#include "outputwindow.h"
#include "ui_mainwidget.h"
#include "settings.h"
#include "pesqthread.h"

#include <QToolBar>
#include <QLayout>
#include <QToolButton>
#include <QProcess>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QPair>

OutputWindow* MainWidget::m_outputWindow = NULL;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    m_init(false)
{
    ui->setupUi(this);
    resize(800, 600);
    setWindowTitle(QStringLiteral("PESQ控制台"));
    setWindowIcon(QIcon(":/res/appicon.png"));
    m_outputWindow = new OutputWindow();
    //m_thread = new PESQThread(this);
    m_thread = new PESQThread();

    ui->progressBar->setValue(0);
    int count = GlobalSettings::instance()->getCommandList().count();
    ui->progressBar->setRange(1, GlobalSettings::instance()->rounds() * count);
    ui->progressBar->setVisible(false);

    this->layout()->addWidget(m_outputWindow);

    connect(NetworkController::createInstance(), SIGNAL(dataRecved(QByteArray)), this ,
            SLOT(updateRecvTextEditor(QByteArray)));
    connect(NetworkController::createInstance(), SIGNAL(statusRecved(QByteArray)), this,
            SLOT(updateStatus(QByteArray)));

    connect(m_thread, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));
    connect(m_thread, SIGNAL(statusChanged(QString)), this, SLOT(updateStatusString(QString)));
    connect(m_thread, SIGNAL(requestedServer()), this, SLOT(detectStatus()));

    connect(m_thread, SIGNAL(deldelay(int)), this, SLOT(delDelay(int)), Qt::QueuedConnection);
    connect(m_thread, SIGNAL(setdelay(int)), this, SLOT(setDelay(int)), Qt::QueuedConnection);
    connect(m_thread, SIGNAL(delloss(int)), this ,SLOT(delLoss(int)), Qt::QueuedConnection);
    connect(m_thread, SIGNAL(setloss(int)), this, SLOT(setLoss(int)), Qt::QueuedConnection);
    connect(m_thread, SIGNAL(pResultDataReady(QString, QString)), this,
            SLOT(setupScoreTable(QString, QString)), Qt::QueuedConnection);
    connect(m_thread, SIGNAL(dResultDataReady(QString, QString)), this,
            SLOT(setupDelayTable(QString, QString)));

    connect(ui->btn_delay, SIGNAL(clicked()), this, SLOT(setDelay()));
    connect(ui->btn_del_delay, SIGNAL(clicked()), this, SLOT(delDelay()));
    connect(ui->btn_loss, SIGNAL(clicked()), this, SLOT(setLoss()));
    connect(ui->btn_del_loss, SIGNAL(clicked()), this, SLOT(delLoss()));
    connect(ui->btn_jitter_setting, SIGNAL(clicked()), this, SLOT(setJitterDelay()));
    connect(ui->btn_user_cmd, SIGNAL(clicked()), this, SLOT(setCustomCmd()));

    connect(ui->btn_run, SIGNAL(clicked()), this, SLOT(runPESQ()));
    connect(ui->btn_setting, SIGNAL(clicked()), this, SLOT(slot_setting()));

    connect(ui->btn_export, SIGNAL(clicked()), this, SLOT(exportToCsv()));
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(clearTask()));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(detectStatus()));
    m_timer->start(10000);

    connect(m_thread, SIGNAL(finished()),
            this, SLOT(slot_thread_finished()));

    NetworkController::createInstance()->request_status();
    setupComboBox();
    setupSliders();
    syncSliders();

    QStandardItemModel *model1 = new QStandardItemModel();
    ui->score_tableView->setModel(model1);

    QStandardItemModel *model2 = new QStandardItemModel();
    ui->delay_tableView->setModel(model2);
}

void MainWidget::updateRecvTextEditor(QByteArray data)
{
    ui->textEdit_response->clear();
    ui->textEdit_response->append(data);
}

void MainWidget::updateStatusString(QString text)
{
    ui->label_status->setText(text);
}

void MainWidget::setDelay(int v)
{
    int value = (v == 0) ? ui->slider_delay->value() : v;
    if (!m_init) {
        NetworkController::createInstance()->request_server("add", "delay", value);
    } else {
        NetworkController::createInstance()->request_server("change", "delay", value);
    }
    m_init = true;
}

void MainWidget::delDelay(int v)
{
    m_init = false;
    int value = (v == 0) ? ui->slider_delay->value(): v;
    NetworkController::createInstance()->request_server("del", "delay", value);
}

void MainWidget::setLoss(int v)
{
    int value = (v == 0) ? ui->slider_loss->value() : v;
    if (!m_init) {
        NetworkController::createInstance()->request_server("add", "loss", value);
    } else {
        NetworkController::createInstance()->request_server("change", "loss", value);
    }
    m_init = true;
}

void MainWidget::delLoss(int v)
{
    m_init = false;
    int value = (v == 0) ? ui->slider_loss->value() : v;

    NetworkController::createInstance()->request_server("del", "loss", value);
}

void MainWidget::setJitterDelay()
{
    int jitter_ms = ui->combo_jitter->currentText().remove("ms").toInt();
    //get jitter value

    int jitter_delay = ui->combo_jitter_delay->currentText().remove("ms").toInt();

    QString set_jitter_cmd = "";
    if (!m_init) {
        set_jitter_cmd =
                QString("tc qdisc add dev enp3s0 root netem delay %1ms %2ms")
                .arg(jitter_delay).arg(jitter_ms);
    } else {
        set_jitter_cmd = QString("tc qdisc change dev enp3s0 root netem delay %1ms %2ms")
                 .arg(jitter_delay).arg(jitter_ms);
    }

    NetworkController::createInstance()->request_server(set_jitter_cmd);
    m_init = true;
}

void MainWidget::setCustomCmd()
{
    QString cmdstr = ui->combo_custom_editor->currentText();
    NetworkController::createInstance()->request_server(cmdstr);
}

void MainWidget::updateStatus(QByteArray data)
{
    QString text = data;
    qDebug() << text;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return;
    }

    QJsonObject json = doc.object();
    QString msg = json["msg"].toString();
    int err = json["errno"].toInt();
    if (err != 0) {
        ui->lineEdit_status->clear();
        return;
    }
    QStringList msgs = msg.split("\n");
    QString showtext = "";
    foreach (QString m, msgs) {
        if (m.contains("enp3s0")) {
            showtext = m;
            break;
        }
    }
    ui->lineEdit_status->setText(showtext);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::setupComboBox()
{
    QStringList lossoptions;
    lossoptions << "5%" << "10%" << "20%" << "30%";

    QStringList delayoptions;
    delayoptions << "50ms" << "100ms" << "200ms" << "400ms";

    QStringList jitteroptions;
    jitteroptions << "10ms" << "20ms" << "40ms" << "50ms" << "100ms" << "200ms";

    QStringList custom_commands;
    custom_commands << "tc qdisc add dev enp3s0 root netem delay 100ms 10ms"
                    << "tc qdisc add dev enp3s0 root netem delay 100ms 10ms 30%"
                    << "tc qdisc add dev enp3s0 root netem loss 1%"
                    << "tc qdisc add dev enp3s0 root netem loss 1% 30%"
                    << "tc qdisc add dev enp3s0 root netem duplicate 1%"
                    << "tc qdisc add dev enp3s0 root netem corrupt 1%"
                    << "tc qdisc add dev enp3s0 root netem delay 10ms reorder 25% 50%";


    ui->combodelay->setEditable(true);
    ui->combo_loss->setEditable(true);
    ui->combo_jitter->setEditable(true);

    ui->combo_loss->addItems(lossoptions);
    ui->combodelay->addItems(delayoptions);
    ui->combo_jitter->addItems(jitteroptions);
    ui->combo_jitter_delay->addItems(delayoptions);
    ui->combo_custom_editor->addItems(custom_commands);

    connect(ui->combo_loss, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(syncSliders()));
    connect(ui->combodelay, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(syncSliders()));
    connect(ui->combo_jitter, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(syncSliders()));
    connect(ui->combo_jitter_delay, SIGNAL(currentTextChanged(QString)),
            this, SLOT(syncSliders()));
}

void MainWidget::setupSliders()
{
    ui->slider_loss->setRange(0, 100);
    ui->slider_loss->setTickInterval(5);
    ui->slider_delay->setRange(0, 3000);
    ui->slider_delay->setTickInterval(50);
    ui->slider_jitter->setRange(0, 1000);
    ui->slider_jitter->setTickInterval(1);

    ui->slider_jitter_delay->setRange(0, 1000);
    ui->slider_jitter_delay->setTickInterval(50);

    connect(ui->slider_loss, SIGNAL(valueChanged(int)), this, SLOT(syncCombos()));
    connect(ui->slider_delay, SIGNAL(valueChanged(int)), this, SLOT(syncCombos()));
    connect(ui->slider_jitter, SIGNAL(valueChanged(int)), this, SLOT(syncCombos()));
    connect(ui->slider_jitter_delay, SIGNAL(valueChanged(int)), this, SLOT(syncCombos()));
}

void MainWidget::syncSliders()
{
    QString currentLossText = ui->combo_loss->currentText();
    QString currentDelayText = ui->combodelay->currentText();
    QString currentJitterText = ui->combo_jitter->currentText();
    QString currentJitterDelayText = ui->combo_jitter_delay->currentText();

    if (!currentLossText.isEmpty()) {
        //取出数字
       QString number = currentLossText.remove("%");
       int scale = number.toInt();
       ui->slider_loss->setValue(scale);
    }

    if (!currentDelayText.isEmpty()) {
        QString number = currentDelayText.remove("ms");
        int scale = number.toInt();
        ui->slider_delay->setValue(scale);
    }

    if (!currentJitterText.isEmpty()) {
        QString number = currentJitterText.remove("ms");
        int scale = number.toInt();
        ui->slider_jitter->setValue(scale);
    }

    if (!currentJitterDelayText.isEmpty()) {
        QString number = currentJitterDelayText.remove("ms");
        int scale = number.toInt();
        ui->slider_jitter_delay->setValue(scale);
    }
}

void MainWidget::syncCombos()
{
    int delay_value = ui->slider_delay->value();
    ui->combodelay->setEditText(QString("%1ms").arg(delay_value));

    int loss_value = ui->slider_loss->value();
    ui->combo_loss->setEditText(QString("%1%").arg(loss_value));

    int jitter_value = ui->slider_jitter->value();
    ui->combo_jitter->setEditText(QString("%1ms").arg(jitter_value));

    int jitter_delay_value = ui->slider_jitter_delay->value();
    ui->combo_jitter_delay->setEditText(QString("%1ms").arg(jitter_delay_value));
}

void MainWidget::runPESQ()
{
    //更新一下按钮的状态
    //ui->btn_run->setEnabled(false);

    if (ui->btn_run->text() == QStringLiteral("评分")) {
        ui->btn_run->setText(QStringLiteral("停止"));
        ui->progressBar->setValue(2);
        ui->progressBar->setVisible(true);
        m_timer->stop();
        ui->btn_setting->setEnabled(false);
        ui->btn_clear->setEnabled(false);
        ui->btn_export->setEnabled(false);

         //每次重新评分需要清理一下报告的model
        QStandardItemModel *score_model = qobject_cast<QStandardItemModel*>(ui->score_tableView->model());
        if (score_model) score_model->clear();

        QStandardItemModel *delay_model = qobject_cast<QStandardItemModel*>(ui->delay_tableView->model());
        if (delay_model) delay_model->clear();

        m_thread->start();
    } else if (ui->btn_run->text() == QStringLiteral("停止")) {
        m_timer->start(10000);
        ui->btn_run->setText(QStringLiteral("评分"));
        m_thread->stop();
        ui->progressBar->setVisible(false);

        ui->btn_setting->setEnabled(true);
        ui->btn_clear->setEnabled(true);
        ui->btn_export->setEnabled(true);
    }
}

void MainWidget::slot_thread_finished()
{
    ui->progressBar->setVisible(false);
    ui->label_status->setText("Done!");
    ui->btn_run->setEnabled(true);
    ui->btn_run->setText(QStringLiteral("评分"));
    ui->btn_setting->setEnabled(true);
    ui->btn_clear->setEnabled(true);
    ui->btn_export->setEnabled(true);
    clearTask();
}

void MainWidget::detectStatus()
{
    NetworkController::createInstance()->request_server("show", "");
}

void MainWidget::setPesqDir(const QString &newDir)
{
    Q_UNUSED(newDir);
}

void MainWidget::changePlayAndRecDir()
{
    QFileDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    QStringList files = dlg.selectedFiles();
    qDebug() << files;
}

void MainWidget::slot_setting()
{
    SettingDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        GlobalSettings::instance()->setCommandList(dialog.commands());//存在内存中
    }
}

void MainWidget::setupScoreTable(QString cmdtitle, QString alltext)
{
    if (alltext.isEmpty())
        return;
    if (cmdtitle.isEmpty())
        cmdtitle = "normal";
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->score_tableView->model());
    if (!model)
        return;
    QStringList textitems = alltext.split("\n");
    QString titleline = textitems.at(0);
    //deal with tile line

    ui->score_tableView->setModel(model);

    QStringList titleLabels = titleline.split("\t");
    titleLabels.replace(0, QString("NetworkState"));
    titleLabels.removeAt(1);
    model->setHorizontalHeaderLabels(titleLabels);

    textitems.removeAt(0);//remove title

    Q_ASSERT(titleLabels.count() == 5);

    QList<QPointF> dataArray;
    // ((1.2, 1.3))

    QStringList line2 = textitems.at(1).split("\t");
    QString mode = line2.at(5);
    QString sample_freq = line2.at(4);
    foreach (QString itemText, textitems) {
        if (itemText.isEmpty()) {
            //有可能出现一些空行
            break;
        }
        QStringList items = itemText.split("\t");
        if (items.count() != 6) {
            qDebug() << " === Data parse error! Please check the file ===";
            qDebug() << " === The file pesq_result.txt may be daymaged ===";
            qDebug() << " === You can solve this problem by removing the file and rerun the program ===";
            break;
        }
        double s1 = items.at(2).trimmed().toDouble();
        double s2 = items.at(3).trimmed().toDouble();
        dataArray.append(QPointF(s1, s2));
    }
    //
    //计算平均数 calculate avg
    int total = dataArray.count();
    Q_ASSERT(total > 0);
    double First{0.0}, Second{0.0};
    foreach (QPointF p, dataArray) {
        First += p.x();
        Second += p.y();
    }

    double avg_p1 = First / total;
    double avg_p2 = Second / total;

    QStandardItem *item0 = new QStandardItem;
    item0->setText(cmdtitle);

    QStandardItem *item1 = new QStandardItem;
    item1->setText(QString::number(avg_p1, 'f', 3));

    QStandardItem *item2 = new QStandardItem;
    item2->setText(QString::number(avg_p2, 'f', 3));

    QStandardItem *item3 = new QStandardItem;
    item3->setText(sample_freq);

    QStandardItem *item4 = new QStandardItem;
    item4->setText(mode);

    model->appendRow(QList<QStandardItem*>() << item0
                     << item1 << item2 << item3 << item4);
    //ui->score_tableView->setModel(model);

    QFile file("process_result.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << QStringLiteral("文件打开失败") << file.errorString();
        return;
    }

    QTextStream out(&file);
    QString date = QDateTime::currentDateTime().toString("yy-MM-dd_hh:mm:ss");
    out << date << "\t" << cmdtitle << "\t";
    foreach (QPointF p, dataArray) {
        out << p.x() << "," << p.y() << "\t";
    }
    out << "\n";
    file.close();
}

void MainWidget::setupDelayTable(QString cmdtitle, QString alltexts)
{
    qDebug() << alltexts;
    //"Offset: 12322, Delay: 770.13ms\r\n" @0x19b150: each line
    if (alltexts.isEmpty())
        return;

    QList<QPair<int, double> > dataArray;
    int total_line = 0;//total统计不根据\r\n，需要过滤掉空行

    QStringList alltextlist = alltexts.split("\r\n");

    foreach (QString eachline, alltextlist) {
        if (eachline.isEmpty())
            continue; //可能存在一些空行，首尾
        ++total_line;
        QStringList testlist = eachline.split(",");
        Q_ASSERT(testlist.count() == 2);

        QString offsetstr = testlist.at(0);
        int offset = offsetstr.remove("Offset:").trimmed().toInt();
        QString delayStr = testlist.at(1);
        delayStr = delayStr.remove("Delay:").trimmed();
        double delay = delayStr.remove("ms").toDouble();
        dataArray.append(qMakePair(offset, delay));
    }

    auto it = dataArray.begin();
    int sum_of_offset{0};
    double sum_of_delay{0.0};
    for (; it != dataArray.end(); ++it) {
         sum_of_offset += (*it).first;
         sum_of_delay += (*it).second;
    }

    int avg_offset = sum_of_offset / total_line;
    double avg_delay = sum_of_delay / total_line;

    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->delay_tableView->model());
    if (!model)
        return;

    model->setHorizontalHeaderLabels(QStringList() << "NetworkState" << "Offset" << "Delay");
    QStandardItem *item0 = new QStandardItem; item0->setText(cmdtitle); //待处理
    QStandardItem *item1 = new QStandardItem; item1->setText(QString::number(avg_offset));
    QStandardItem *item2 = new QStandardItem; item2->setText(QString::number(avg_delay, 'f', 2));

    model->appendRow(QList<QStandardItem*>() << item0 << item1 << item2);

    QFile file("delay_table.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << QStringLiteral("文件打开失败");
        return;
    }
    QTextStream out(&file);
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    out << date;
    for (auto it = dataArray.begin(); it != dataArray.end(); ++it) {
        out << (*it).second << "\t";
    }
    out << "\r\n";
    file.close();
}

void MainWidget::clearTask()
{
    NetworkController::createInstance()->request_server("del", "loss");
    NetworkController::createInstance()->request_server("del", "delay");
    NetworkController::createInstance()->request_status();
    QProcess process;
    process.setProgram(GlobalSettings::instance()->getScoreRecCmdDirectory());
    if (process.waitForFinished()) {
        process.close();
    }

    process.setProgram(GlobalSettings::instance()->getWavDelayCmdDirectory());
    process.close();

    process.setProgram(GlobalSettings::instance()->getWavDelayRecCmdDirectory());
    process.close();
}

void MainWidget::exportToCsv()
{
    QStandardItemModel *score_model = qobject_cast<QStandardItemModel*>(ui->score_tableView->model());
    QStandardItemModel *delay_model = qobject_cast<QStandardItemModel*>(ui->delay_tableView->model());

    if (!score_model || !delay_model
            || score_model->rowCount() == 0
            || delay_model->rowCount() == 0) {//有一个出错我们就停止导出，发出错误报告
        QMessageBox::warning(this, QStringLiteral("数据错误"), QStringLiteral("导出错误，确认是否有生成的数据，格式等问题!停止导出"), QMessageBox::Ok);
        return;
    }

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    //dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    QDir dir = dialog.directory();

    QString baseName = QString("report_%1.csv")
            .arg(QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss"));
    QString filename = dir.absoluteFilePath(baseName);

    //注释掉，是为了保持历史报告
//    QFileInfo fileInfo(filename);
//    if (fileInfo.exists()) { //文件存在就删掉它
//        if (!dir.remove(baseName))
//            qDebug() << QString("Remove file: %1 failed!Maybe not exits or system error") % baseName;
//    }

    //获取数据
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }
    file.write(",,PESQ socre report\r\n\r\n");

    for (int i = 0; i < score_model->columnCount(); ++i) {
        file.write(score_model->horizontalHeaderItem(i)->text().toLatin1());
        if (i != score_model->columnCount() - 1)
            file.write(",");
    }
    file.write("\r\n");

    for (int i = 0; i < score_model->rowCount(); ++i) {
        for (int j = 0; j < score_model->columnCount(); ++j) {
            file.write(score_model->item(i, j)->text().toLatin1());
            if (j != score_model->columnCount() - 1)
                file.write(",");
        }
        file.write("\r\n");
    }

    //delay report
    file.write(",,Delay statistic report\r\n\r\n");
    for (int i = 0; i < delay_model->columnCount(); ++i) {
        file.write(delay_model->horizontalHeaderItem(i)->text().toLatin1());
        if (i != delay_model->columnCount() - 1)
            file.write(",");
    }
    file.write("\r\n");
    for (int i = 0; i < delay_model->rowCount(); ++i) {
        for (int j = 0; j < delay_model->columnCount(); ++j) {
            file.write(delay_model->item(i, j)->text().toLatin1());
            if (j != delay_model->columnCount() - 1)
                file.write(",");
        }
        file.write("\r\n");
    }
    file.close();
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导出文件 %1 成功").arg(filename),
                             QMessageBox::Ok);
}
