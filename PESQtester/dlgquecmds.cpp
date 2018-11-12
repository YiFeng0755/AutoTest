
#include "dlgquecmds.h"
#include <QListView>
#include <QPushButton>
#include <QStringList>
#include <QLayout>
#include <QMap>
#include <QStringList>


QueueCmdsDialog::QueueCmdsDialog(QWidget *parent)
    :QDialog(parent)
{
    init();
}

QueueCmdsDialog::~QueueCmdsDialog()
{

}

void QueueCmdsDialog::init()
{
    m_listView = new QListView(this);

    QPushButton *cancel_btn = new QPushButton(tr("&cancel"), this);
    QPushButton *accep_btn = new QPushButton(tr("&accept"), this);

    connect(cancel_btn, SIGNAL(clicked()), this, SLOT(close()));
    connect(accep_btn, SIGNAL(clicked()), this, SLOT(accept()));

    QStringList default_cmds;
    default_cmds << "tc qdisc show"
                 << "tc qdisc add dev enp3s0 root netem loss 5%"
                 << "tc qdisc change dev enp3s0 root netem loss 10%"
                 << "tc qdisc change dev enp3s0 root netem loss 20%"
                 << "tc qdisc change dev enp3s0 root netem loss 30%"
                 << "tc qdisc change dev enp3s0 root netem loss 50%"
                 << "tc qdisc change dev enp3s0 root netem loss 70%"
                 << "tc qdisc del dev enp3s0 root netem loss 10%"
                 << "tc qdisc add dev enp3s0 root netem delay 50ms 10ms"
                 << "tc qdisc change dev enp3s0 root netem delay 100ms 20ms"
                 << "tc qdisc change dev enp3s0 root netem delay 200ms 50ms"
                 << "tc qdisc change dev enp3s0 root netem delay 400ms 100ms"
                 << "tc qdisc del dev enp3s0 root netem delay 400ms 100ms";
    int i = 0;
    foreach (QString cmd, default_cmds) {
        m_commands.insert(i++, cmd);
    }

    //m_listView->setModel();
}

void QueueCmdsDialog::accept()
{

    QDialog::accept();
}
