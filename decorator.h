#pragma once
#include "log.h"

//============================================================
class Decorator : public Log
{
  public:
    Decorator(Log *component, const std::string &sname);

    void open_logfile(const std::string &filepath) override;

    virtual Log &operator<<(std::string msg) override;

    virtual Log &operator<<(std::shared_ptr<Log> splog) override;

    virtual Log &operator<<(int num) override;

    virtual Log &operator<<(typelog level) override;

    void close_log() override;

  private:
    Log *m_component;
    const std::string &system_name;
};
