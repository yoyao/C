
#pragma once

#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <sys/time.h>

using namespace std;


#ifdef LOG_DEBUG

#define LOG_ERR_MODEL true
#define LOG_INFO_MODEL true
#define LOG_WARN_MODEL true

#else

#define LOG_ERR_MODEL  false
#define LOG_INFO_MODEL false
#define LOG_WARN_MODEL false

#endif

#define WLOG_INFO(b,s,...)  LogHelper::Info(b,s,__VA_ARGS__);
#define WLOG_WARNING(b,s,...)  LogHelper::Warning(b,s,__VA_ARGS__);
#define WLOG_ERROR(b,s,...)  LogHelper::Error(b,s,__VA_ARGS__);

class LogHelper
{
public:
    LogHelper();
    ~LogHelper();

public:
    enum LogLevel
    {
        INFO = 0,
        WARNING = 1,
        ERROR = 2
    };
    static void FormattTime(time_t t, char *formstr, size_t size, const char*formatt);
    static void Error(bool isdebug, const char *format,...);
    static void Info(bool isdebug,const char *format,...);
    static void Warning(bool isdebug, const char *format,...);
    static void GetTimestamp(char *buff,int len);
    static void GetTimestamp(std::string *result);

private:
    static std::once_flag flag;
    static std::string CurrentPath;
    static FILE *file;
    static void writeLog(bool isdebug, const char *timestr, const char *logstr);
    static void Init();
    static std::mutex lock;
};

