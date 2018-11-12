#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "callbackwrapper.h"
#include "outputwindow.h"

namespace Ui {
class MainWidget;
}

class QProcess;
class MainWidget : public QWidget, public ImWrapper
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    static OutputWindow * messageHandler() {
        return m_outputWindow;
    }
    ~MainWidget();

private slots:
    void slot_perftest_widget();
    void slot_stabletest_widget();
    void slot_function_widget();
    void slot_config_widget();
    void slot_change_server(int index);
    void slot_switch_mode();

private:
    void update_label_status();
    void setup();
    void setupServer();
    void setupMode();

private:
    Ui::MainWidget *ui;
    static OutputWindow *m_outputWindow; // Console ouptut window

    QProcess *chatter_process;
};

#endif // MAINWIDGET_H
