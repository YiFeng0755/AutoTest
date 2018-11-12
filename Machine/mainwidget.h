#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "outputwidget.h"

//class QPushButton;
class TabWidget;
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    static OutputWindow * messageHandler() {
        return m_outputWindow;
    }

public slots:


private:
    void setup();

    TabWidget *m_tabwidget;
    static OutputWindow *m_outputWindow;
};

#endif // MAINWIDGET_H

