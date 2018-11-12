#include "core.h"

Core * Core::_core_instance = NULL;

Core *Core::instance()
{
    if (!_core_instance) {
        _core_instance = new Core();
    }
    return _core_instance;
}

Core::~Core()
{

}

Core::Core(QObject *parent)
    :QObject(parent)
{
    if (_main_settings) {
        delete _main_settings;
        _main_settings = 0;
    }
}

void Core::initialize()
{
    _main_settings = new GlobalSetting(); 
}
