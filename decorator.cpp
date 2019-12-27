#include "decorator.h"

//==================================================================
Decorator::Decorator(Log *component, const std::string &sname)
    : m_component(component), system_name(sname)
{
}

//==================================================================
void Decorator::open_logfile(const std::string &filepath)
{
    m_component->open_logfile(filepath);
}

//==================================================================
Log &Decorator::operator<<(std::string msg)
{
    return *m_component << system_name << msg;
}

//==================================================================
Log &Decorator::operator<<(std::shared_ptr<Log> splog)
{
    return *splog;
}

//==================================================================
Log &Decorator::operator<<(int num)
{
    return *m_component << system_name << num;
}

//==================================================================
Log &Decorator::operator<<(typelog level)
{
    return *m_component << system_name << level;
}

//==================================================================
void Decorator::close_log() { m_component->close_log(); }
