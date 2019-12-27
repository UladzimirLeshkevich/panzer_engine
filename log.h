#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

enum typelog
{
    DEBUG,
    INFO,
    WARNING,
    LOG_ERROR,
    CRITICAL
};

//============================================================
class Log
{
public:
    virtual ~Log() {}

    virtual void open_logfile(const std::string& filepath) = 0;

    virtual void close_log() = 0;

    virtual Log& operator<<(std::string msg) = 0;

    virtual Log& operator<<(std::shared_ptr<Log> msg) = 0;

    virtual Log& operator<<(int num) = 0;

    virtual Log& operator<<(typelog level) = 0;

    friend Log& operator<<(std::shared_ptr<Log> logger, std::string /*str*/);

    friend Log& operator<<(std::shared_ptr<Log> logger, int /*value*/);

    friend Log& operator<<(std::shared_ptr<Log> logger, typelog /*type*/);
};
