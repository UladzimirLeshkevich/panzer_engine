#include "log_manager.h"

//==================================================================
std::shared_ptr<Log> LogManager::get_logger(const std::string &system_name)
{
    auto it = loggerMap.find(system_name);
    if (it != loggerMap.end())
    {
        return it->second;
    }
    else
    {
        logger = std::shared_ptr<Log>(new Decorator(my_logger, system_name));
        loggerMap.emplace(system_name, logger);
        return std::move(logger);
    }
}

//==================================================================
std::shared_ptr<Log> LogManager::logger = nullptr;
My_Log *LogManager::my_logger = new My_Log;
std::map<const std::string, std::shared_ptr<Log>> LogManager::loggerMap;
