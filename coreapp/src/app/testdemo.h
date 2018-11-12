#ifndef TESTDEMO
#define TESTDEMO

#include <QWidget>

namespace Ui {
   class TestDemo;
}

class ActionsTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ActionsTestWidget(QWidget *parent = 0);

private:
    Ui::TestDemo *ui;
};


#endif // TESTDEMO

