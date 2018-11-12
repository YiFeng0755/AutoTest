#include "mainwidget.h"
#include "tabwidget.h"

#include <QFrame>
#include <QLayout>

OutputWindow *MainWidget::m_outputWindow = nullptr;
MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
{
    setup();
}

void MainWidget::setup()
{
    setMinimumSize(800, 800);
    setWindowTitle(tr("AI 聊天"));

    m_tabwidget = new TabWidget(this);
    m_outputWindow = new OutputWindow(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QFrame *seperateLine = new QFrame(this);
    seperateLine->setFrameShape(QFrame::HLine);
    seperateLine->setFrameShadow(QFrame::Sunken);

    mainLayout->addWidget(m_tabwidget);
    mainLayout->addWidget(seperateLine);
    mainLayout->addWidget(m_outputWindow);

    mainLayout->setStretch(0, 100);
    mainLayout->setStretch(1, 40);

    setLayout(mainLayout);
}
