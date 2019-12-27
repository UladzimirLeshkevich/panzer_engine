#include "log.h"

Log &operator<<(std::shared_ptr<Log> logger, std::string str)
{
    return *logger << str;
}

Log &operator<<(std::shared_ptr<Log> logger, int value)
{
    return *logger << value;
}

Log &operator<<(std::shared_ptr<Log> logger, typelog /*type*/)
{
    return *logger;
}
