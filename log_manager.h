#pragma once
#include "decorator.h"
#include "log.h"
#include "my_log.h"

class LogManager
{
  public:
    LogManager() = delete;

    static std::shared_ptr<Log> get_logger(const std::string &system_name);

  private:
    static std::shared_ptr<Log> logger;
    static My_Log *my_logger; // one logger for all systems !!
    static std::map<const std::string, std::shared_ptr<Log>> loggerMap;
};
