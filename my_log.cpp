#include "my_log.h"
#define L_WIDTH 12

//==================================================================
Log& My_Log::operator<<(std::string msg)
{
    stream_buffer << msg;
    return *this;
}

Log& My_Log::operator<<(std::shared_ptr<Log> splog)
{
    return *splog;
}

//==================================================================
Log& My_Log::operator<<(int num)
{
    stream_buffer << " " << num;
    return *this;
}

//==================================================================
Log& My_Log::operator<<(typelog level)
{
    // add time to log
    logfile << "\n" + current_time() + " | ";
    // add level to log
    std::string resized_lebel = get_label(level);
    resized_lebel.resize(L_WIDTH);
    logfile << resized_lebel + " | ";
    // add system_name to log
    std::string resized_system_name;
    stream_buffer >> resized_system_name;
    resized_system_name.resize(L_WIDTH);
    logfile << resized_system_name + " | ";
    // add message to log
    stream_buffer >> logfile.rdbuf();
    stream_buffer.clear();
    return *this;
}

//==================================================================
My_Log::My_Log() {}

//==================================================================
void My_Log::open_logfile(const std::string& filepath)
{
    logfile.open(filepath);
    logfile << "Log started by My_Log at " << current_time() + "\n";
}

//==================================================================
void My_Log::close_log()
{
    logfile << "\n\nLog closed by My_Log at " << current_time();
    logfile.close();
}

//==================================================================
std::string My_Log::get_label(typelog type)
{
    std::string label;
    switch (type)
    {
    case DEBUG:
        label = "DEBUG";
        break;
    case INFO:
        label = "INFO";
        break;
    case WARNING:
        label = "WARNING";
        break;
    case LOG_ERROR:
        label = "ERROR";
        break;
    case CRITICAL:
        label = "CRITICAL";
        break;
    }
    return label;
}

//==================================================================
std::string My_Log::current_time()
{
    time(&rawtime);
    std::string s(ctime(&rawtime));
    s.resize(s.size() - 1);
    return s;
}
