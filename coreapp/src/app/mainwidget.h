#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "outputwindow.h"

class QToolBar;
class OutputWindow;
class YoumeTabWidget;
class MainWidget: public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget * parent = 0);
    static OutputWindow * messageHandler() {
        return m_outputWindow;
    }

private slots:
    void showHostmanagerWidget();
    void showCryptTool();
    void editYoumeConfig();

private:
    void setup();
    void createActions();

private:
    QToolBar*m_mainToolBar;
    QAction *m_hostmanagerAction;
    QAction *m_aclAction;
    QAction *m_cryptAction;
    QAction *m_dbAction;
    QAction *m_youmeRequestConfigAction;

    YoumeTabWidget *m_centralWidget;
    static OutputWindow *m_outputWindow; // Console ouptut window
};

#endif // MAINWIDGET_H
