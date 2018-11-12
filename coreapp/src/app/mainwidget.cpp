#include "mainwidget.h"
#include "hostmanagerwidget.h"
#include "cryptwidget.h"
#include "tabwidgets.h"
#include "digyoumerequestconfig.h"

#include <QToolBar>
#include <QToolButton>
#include <QLayout>
#include <QAction>
#include <QMenu>
#include <QPushButton>

OutputWindow* MainWidget::m_outputWindow = NULL;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setup();
    createActions();
}

void MainWidget::createActions()
{
    m_hostmanagerAction = new QAction(QIcon(":/images/host_manager.png"),
                                      tr("&Host Manager"), this);
    m_aclAction = new QAction(QIcon(":/images/acl_item.png"),
                              tr("&Acl Tool"), this);
    m_cryptAction = new QAction(QIcon(":/image/host_manager.png"),
                                tr("&Crypte Tool"), this);

    m_youmeRequestConfigAction = new QAction(QIcon(""), tr("&Youme Config"), this);

    QMenu *utiltoolsMenu = new QMenu(this);
    utiltoolsMenu->addAction(m_hostmanagerAction);
    utiltoolsMenu->addAction(m_cryptAction);
    utiltoolsMenu->addAction(m_youmeRequestConfigAction);

    //m_hostmanagerAction->setMenu(utiltoolsMenu);

    QToolButton *utilToolButton = new QToolButton(this);
    utilToolButton->setMenu(utiltoolsMenu);
    utilToolButton->setText(tr("&Utils"));
    utilToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    utilToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    //utilToolButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    //utilToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    utilToolButton->setIcon(QIcon(":/images/host_manager.png"));
    //utilToolAction->setIcon(QIcon();
    //utilToolAction->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //utilToolButton->setStyleSheet("QToolButton::menu-indicator{image:None;}");

    QAction *db_action = new QAction(QIcon(""), tr("&SQL"), this);
    db_action->setToolTip(tr("Exe sql attcack"));

    m_mainToolBar->addWidget(utilToolButton);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(db_action);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_aclAction);

    m_hostmanagerAction->setToolTip(tr("Host Manager"));

    connect(m_hostmanagerAction,SIGNAL(triggered(bool)),this,SLOT(showHostmanagerWidget()));
    connect(m_cryptAction,SIGNAL(triggered(bool)), this,SLOT(showCryptTool()));
    connect(m_youmeRequestConfigAction, SIGNAL(triggered(bool)), this, SLOT(editYoumeConfig()));
}

void MainWidget::setup()
{
    setMinimumSize(800,600);
    setWindowTitle(tr("Appreate"));

    setWindowIcon(QIcon(":/images/app.ico"));

    m_mainToolBar = new QToolBar ;
    //m_mainToolBar->addAction(new QAction(tr("test"),this));
    m_mainToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_outputWindow = new OutputWindow();
    m_centralWidget = new YoumeTabWidget();

    QHBoxLayout * toolbarLayout = new QHBoxLayout ;
    toolbarLayout->addWidget(m_mainToolBar);
    toolbarLayout->setSpacing(0);
    toolbarLayout->setMargin(0);

    QVBoxLayout * mainLayout = new QVBoxLayout ;
    mainLayout->addLayout(toolbarLayout);

    mainLayout->addWidget(m_centralWidget);
    mainLayout->addWidget(m_outputWindow);
    setLayout(mainLayout);
}

void MainWidget::showHostmanagerWidget()
{
    HostManagerDialog hostManager(this);
    hostManager.exec();
}

void MainWidget::showCryptTool()
{
    CryptDialog dialog(this);
    dialog.exec();
}

void MainWidget::editYoumeConfig()
{
    ConfigDialog dialog(this);
    dialog.exec();
}

