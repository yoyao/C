#ifndef FILEINFO_OP_H
#define FILEINFO_OP_H

#include <time.h>
#include <mysql/mysql.h>
#include "include/userinfo_op.h"

typedef struct _fileinfo
{
    int    fid;//文件的唯一标识符
    int    fileowner;//文件属主
    char   localname[128];//
    char   fdfsname[128];
    long int size;
    int    filetype;
    char   uuid[64];
    char   md5code[64];
    time_t addtime;
    int    stat;
    int    isshared;//是否分享
} FileInfo;

typedef struct _fileresult
{
    FileInfo *result;
    int    len;//是否分享
} FileResult;

//执行插入语句
my_ulonglong file_insert(MYSQL *handle, FileInfo *p_file);
//通过用户获取其上传的所有文件
int file_get_by_user(MYSQL *handle, UserInfo *user, FileResult *p_result);
my_ulonglong file_update(MYSQL *handle,FileInfo *file);
int file_get_by_result(MYSQL_RES *result, FileInfo *file);
void file_free_fileresult(FileResult *p_fr);










#endif // FILEINFO_OP_H
