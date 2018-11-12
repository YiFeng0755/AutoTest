#include "outputwindow.h"
#include <QHBoxLayout>
#include <QTextEdit>
#include <QToolBar>
#include <QToolButton>
#include <QDebug>

OutputWindow::OutputWindow(QWidget *parent)
    :QWidget(parent)
{
    m_editor = new QTextEdit;
    m_editor->setReadOnly(true);

    m_toolBar = new QToolBar;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_toolBar);
    mainLayout->addWidget(m_editor);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setMargin(0);

    setup();
    setLayout(mainLayout);
}

void OutputWindow::showMessages(QByteArray message)
{
    m_editor->append(message);
}

void OutputWindow::debug()
{
    // TO do
    qDebug() << "Show debug messages";
}

void OutputWindow::info()
{
    qDebug() << "Show info messages";
}

void OutputWindow::setup()
{
    QToolButton *clearButton = new QToolButton();
    clearButton->setText("&Clear");

    QToolButton *debugButton = new QToolButton();
    debugButton->setText("&Debug");

    connect(clearButton, SIGNAL(clicked(bool)), m_editor, SLOT(clear()));
    connect(debugButton, SIGNAL(clicked(bool)), this, SLOT(debug()));

    m_toolBar->addWidget(clearButton);
    m_toolBar->addWidget(debugButton);
}
