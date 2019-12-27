#pragma once
#include "log.h"
#include <sstream>

class My_Log : public Log
{
  public:
    virtual Log &operator<<(std::string msg) override;

    virtual Log &operator<<(std::shared_ptr<Log> splog) override;

    virtual Log &operator<<(int num) override;

    virtual Log &operator<<(typelog level) override;

    My_Log();

    void open_logfile(const std::string &filepath) override;

    void close_log() override;

  private:
    std::ofstream logfile;
    std::stringstream stream_buffer;
    time_t rawtime;
    std::string get_label(typelog type);
    std::string current_time();
};
