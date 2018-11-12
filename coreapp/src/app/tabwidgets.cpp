#include "tabwidgets.h"

#include "requestwidget.h"
#include "testdemo.h"

YoumeTabWidget::YoumeTabWidget(QWidget *parent)
    :QTabWidget(parent)
{
    m_requestWidget = new RequestWidget();
    m_actionsWidget = new ActionsTestWidget();

    addTab(m_requestWidget, QString("Requet Tool"));
    addTab(m_actionsWidget, QString("Test Demo"));
}
