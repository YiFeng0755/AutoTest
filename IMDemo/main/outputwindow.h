#ifndef OUTPUTWINDOW
#define OUTPUTWINDOW

#include <QWidget>

class QTextEdit;
class QToolBar;
class OutputWindow : public QWidget
{
    Q_OBJECT
public:
    explicit  OutputWindow(QWidget * parent = 0);

public slots:
    void showMessages(QByteArray message);

private slots:
    void debug();
    void info();

private:
    void setup();

private:
    QTextEdit *m_editor;
    QToolBar * m_toolBar;
};

#endif // OUTPUTWINDOW_H

