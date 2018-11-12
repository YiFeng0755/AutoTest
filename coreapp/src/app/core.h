#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "globalsetting.h"
class Core : public QObject
{
    Q_OBJECT
public:
    ~Core();
    static Core *instance();

protected:
    explicit Core(QObject *parent = 0);
    void initialize();

private:
    GlobalSetting *_main_settings;
    static Core* _core_instance;
};

#endif // CORE

