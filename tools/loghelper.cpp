
#include "loghelper.h"
#include <limits.h>
#include <unistd.h>
#include <string.h>

std::once_flag LogHelper::flag;
FILE *LogHelper::file=NULL;
std::string LogHelper::CurrentPath="";
std::mutex LogHelper::lock;

LogHelper::LogHelper()
{
}

LogHelper::~LogHelper()
{
    if(file !=NULL)
    {
        fclose(file);
    }
}

size_t get_executable_path( char* processdir,char* processname, size_t len)
{
    char* path_end;
    if(readlink("/proc/self/exe", processdir,len) <=0)
        return -1;
    path_end = strrchr(processdir,  '/');
    if(path_end == NULL)
        return -1;
    ++path_end;
    strcpy(processname, path_end);
    *path_end = '\0';

    return (size_t)(path_end - processdir);
}

static std::string GetCurPath()
{
    char path[PATH_MAX]={0};
    char processname[1024]={0};
    get_executable_path(path, processname, sizeof(path));
    return path;
}

void LogHelper::writeLog(bool isdebug,const char *timestr, const char *logstr)
{
    std::lock_guard<std::mutex> locker(lock);
    if (isdebug)
    {
        std::cout<<timestr<<" "<<logstr << std::endl;
        return;
    }

    Init();
    if (file == NULL)
    {
        std::cout<<"file is NULL"<<std::endl;
        return;
    }
    // fwrite(timestr, strlen(timestr), 1, file);
    //参数的值不能正常显示，原因是参数传递错误，fprintf不能正确处理va_list类型的参数
    fprintf(file, "%s %s\n",timestr,logstr);
    fflush(file);
    fclose(file);
    file=NULL;
}

void LogHelper::Error(bool isdebug,const char *format,...)
{
    char pbString[2048]={0};
    va_list	arg;
    va_start(arg, format);
    vsprintf(pbString, format, arg);
    va_end(arg);

    char timestr[256] = { 0 };
    const char *levstr="[ERROR]";
    GetTimestamp(timestr,sizeof(timestr));
    strcat(timestr, " ");
    strcat(timestr,levstr);

    writeLog(isdebug,timestr,pbString);
}

void LogHelper::Info(bool isdebug, const char *format,...)
{
    char pbString[2048]={0};
    va_list	arg;
    va_start(arg, format);
    vsprintf(pbString, format, arg);
    va_end(arg);

    char timestr[256] = { 0 };
    const char *levstr="[INFO]";
    GetTimestamp(timestr,sizeof(timestr));
    strcat(timestr, " ");
    strcat(timestr,levstr);

    writeLog(isdebug,timestr,pbString);
}

void LogHelper::Warning(bool isdebug,const char *format,...)
{
    char pbString[2048]={0};
    va_list	arg;
    va_start(arg, format);
    vsprintf(pbString, format, arg);
    va_end(arg);

    char timestr[256] = { 0 };
    const char *levstr="[WARNING]";
    GetTimestamp(timestr,sizeof(timestr));
    strcat(timestr, " ");
    strcat(timestr,levstr);
    writeLog(isdebug,timestr,pbString);
}


void LogHelper::FormattTime(time_t t, char *formstr, size_t size, const char*formatt)
{
    if (formstr == NULL)
    {
        return;
    }
    if (formatt == NULL)
    {
        formatt = "%Y-%m-%d %H:%M:%S";
    }
    struct tm ptm ;
    localtime_r(&t,&ptm);
    strftime(formstr, size, formatt, &ptm);
}

void LogHelper::Init()
{
    CurrentPath = GetCurPath();
    char timestr[64] = { 0 };
    FormattTime(time(NULL), timestr, sizeof(timestr), "%Y-%m-%d");
    CurrentPath += "/mylog-";
    CurrentPath+=timestr;
    CurrentPath+=".log";

    file = fopen(CurrentPath.c_str(), "ab");
    if (file == NULL)
    {
        file = fopen(CurrentPath.c_str(), "w");
        if (file == NULL)
        {
            return;
        }
        fclose(file);
        file = fopen(CurrentPath.c_str(), "ab+");
    }

}


void LogHelper::GetTimestamp(std::string *result)
{
    char sdate[256]={0};
    GetTimestamp(sdate,sizeof(sdate));
    *result=sdate;
}

void LogHelper::GetTimestamp(char *buff, int len)
{
    struct timeval tv;
    struct tm *dc=NULL;
    char sdate[128]={0};

    gettimeofday(&tv,NULL);
    memset(sdate,0,sizeof(sdate));
    dc = localtime(&tv.tv_sec);
    strftime(sdate,sizeof(sdate),"%Y-%m-%d %H:%M:%S",dc);
    sprintf(buff,"%s.%.3lu.%.3lu",sdate,tv.tv_usec/1000,tv.tv_usec%1000);

}
