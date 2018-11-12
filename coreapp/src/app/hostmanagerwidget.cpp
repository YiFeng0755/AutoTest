
#include <QSplitter>
#include <QLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QToolBar>
#include <QToolButton>
#include <QTextEdit>
#include <QPushButton>
#include <QFileSystemWatcher>
#include "hostsmanager.h"
#include "hostmanagerwidget.h"

/**
 * @brief HostManagerWidget::HostManagerWidget
 * @param parent
 * This widget is supplied for manager the Windows hosts filr or Mac OSX host file
 * You can add or delete the ip-host map
 * You can modif y the ip-host-host map
 *
 */

HostManagerDialog::HostManagerDialog( QWidget *parent)
    : QDialog(parent)
   , m_fileView(new QTextEdit())
   , m_manager(new HostManager )
{
    initializeTableView();

    createToolButtons();

    setup();

    loadHostFile();
}

HostManagerDialog::~HostManagerDialog()
{
    if (m_manager) {
        delete m_manager ;
        m_manager = 0 ;
    }
}

void HostManagerDialog::createToolButtons()
{
    QToolButton * m_reloadButton = new QToolButton();
    m_reloadButton->setText(tr("&Reload"));
    m_reloadButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QToolButton * m_addButton = new QToolButton(this);
    m_addButton->setText(tr("&Add"));
    m_addButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_addButton->setIcon(QIcon(":/images/add_item.png"));
    connect(m_addButton,SIGNAL(clicked(bool)),this,SLOT(addItem()));

    QToolButton * m_removeButton = new QToolButton(this);
    m_removeButton->setText("&Remove");
    m_removeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_removeButton->setIcon(QIcon(":/images/minus_item.png"));
    connect(m_removeButton,SIGNAL(clicked(bool)),this,SLOT(removeItem()));

    m_toolBar = new QToolBar(this);
    m_toolBar->setProperty("border",true);
    m_toolBar->addWidget(m_reloadButton);
    m_toolBar->addWidget(m_addButton);
    m_toolBar->addWidget(m_removeButton);
}

void HostManagerDialog::loadHostFile( )
{
    m_fileView->clear();

    QHash<QString,HostManager::IP > iphosts = m_manager->ipHosts();

    m_fileView->setDocument(new QTextDocument(m_manager->readAllConf()));
    //m_fileView->setReadOnly(true);
}

void HostManagerDialog::setup()
{
    setMinimumSize(600,500);
    setWindowTitle(tr("Host file manager"));

    QVBoxLayout * tableLayout = new QVBoxLayout ;
    tableLayout->addWidget(m_tableview);
    tableLayout->addWidget(m_toolBar);
    tableLayout->setMargin(0);
    tableLayout->setSpacing(0);

    QHBoxLayout * hLayout = new QHBoxLayout ;

    hLayout->addLayout(tableLayout);
    hLayout->addWidget(m_fileView);

    QPushButton * saveButton = new QPushButton(tr("&Save"));
    QPushButton * closeButton = new QPushButton(tr("&Close"));

    connect(saveButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    QHBoxLayout * buttonLayout = new QHBoxLayout ;
    buttonLayout->addWidget(saveButton );
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout ;

    QFrame * frame = new QFrame(this);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameStyle(QFrame::Sunken);

    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(frame);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void HostManagerDialog::addItem()
{
     QModelIndex index = m_tableview->currentIndex();
     if (!index.isValid()) {
         return ;
     }
     QStandardItem * item1 = new QStandardItem ;
     QStandardItem * item2 = new QStandardItem ;
     QList<QStandardItem*> itemlist ;
     itemlist << item1 << item2 ;
     m_model->insertRow(index.row(),itemlist);
}

void HostManagerDialog::removeItem()
{
    QModelIndex index = m_tableview->currentIndex();
    if (!index.isValid()) {
        return  ;
    }
    m_model->removeRow(index.row());
}

void HostManagerDialog::accept()
{
   QString text =  m_fileView->document()->toPlainText();
   m_manager->saveDataToDisk(text);
   return QDialog::accept();

}

void HostManagerDialog::reload()
{
    m_model->clear();
    loadDataFromFile();
}

void HostManagerDialog::initializeTableView()
{
    m_tableview = new QTableView ;
    QStringList  headerlist  ;
    headerlist << tr("Host") << tr("Ip");

    m_tableview->horizontalHeader()->setStretchLastSection(true);
    m_tableview->verticalHeader()->setHidden(true);

    m_model = new QStandardItemModel();
    m_model->setHorizontalHeaderLabels(headerlist);
    m_tableview->setModel(m_model);

    loadDataFromFile();
}

void HostManagerDialog::loadDataFromFile()
{
    m_model->clear();

    QHash<QString, HostManager::IP > iphosts = m_manager->ipHosts();

    QHashIterator<QString,HostManager::IP > iter(iphosts);
    while (iter.hasNext()) {
        iter.next();
        QString host = iter.key();
        HostManager::IP ip = iter.value();
        QStandardItem * item_host = new QStandardItem ;
        item_host->setText(host);
        QStandardItem * item_ip = new QStandardItem ;
        item_ip->setText(ip.ip);
        item_ip->setData(true,Qt::UserRole + 1);
        QList<QStandardItem* > itemlist ;
        itemlist << item_host << item_ip ;
        m_model->appendRow(itemlist);
    }
}
