#ifndef TABWIDGETS
#define TABWIDGETS

#include <QTabWidget>

class RequestWidget;
class ActionsTestWidget;
class YoumeTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit YoumeTabWidget(QWidget *parent= 0);

private:
    RequestWidget *m_requestWidget;
    ActionsTestWidget *m_actionsWidget;
};

#endif // TABWIDGETS

