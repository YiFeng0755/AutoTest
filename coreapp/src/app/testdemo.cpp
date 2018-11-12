#include "testdemo.h"
#include "ui_testdemo.h"

ActionsTestWidget::ActionsTestWidget(QWidget *parent)
    :QWidget(parent),
      ui(new Ui::TestDemo)
{
    ui->setupUi(this);
}


