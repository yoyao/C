
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/mysql_op.h"
#include "include/fileinfo_op.h"
#include <time.h>

#define DB_USER_    "root"
#define DB_PWD_     "123"
#define DB_DB_NAME_ "clouddisk"

void printfileinfo(FileInfo *p_file,int len);
static void file_copy_attr(FileInfo *p_file, const char *field, const char *attr);
time_t strtotime_t(const char *time);
void CreateUser(UserInfo *user);
void CreateFileinfo(FileInfo *file);

int mainxx(int argc, char *argv[])
{
    int ret=0;
    FileInfo file,file1;
    UserInfo user;
    FileResult result;
    bzero(&file,sizeof(file));
    bzero(&file1,sizeof(file1));
    bzero(&user,sizeof(user));
    bzero(&result,sizeof(result));
    CreateFileinfo(&file);
    CreateUser(&user);

    puts("Read connect\n");

    MYSQL *mysql_handle=NULL;
    mysql_handle=mysql_m_conn(DB_USER_, DB_PWD_,DB_DB_NAME_);
    if(mysql_handle==NULL)
    {
        puts("mysql_m_conn error\n");
        return 0;
    }

    char sql[1024]={0};
    sprintf(sql,"select * from fileinfo where md5code='%s' and size=%ld ","8f86c9fb576a51cdfa6d0a469b816368",2276868);
    MYSQL_RES *p_res= mysql_m_query(mysql_handle,sql);
    if(p_res ==NULL)
    {
        puts("Read connect\n");
    }
    file_get_by_result(p_res,&file1);
    printfileinfo(&file1,1);
    return 0;
}


//插入一条文件信息
my_ulonglong file_insert( MYSQL *handle, FileInfo *p_file)
{
    int ret=0;
    char sql_insert[2048]={0};
    if(p_file ==NULL)
    {
        puts("file_insert file is NULL");
        return -1;
    }
    sprintf(sql_insert,"INSERT INTO `fileinfo` ( `fileowner`, `localname`, `fdfsname`, `size`, `filetype`, `uuid`, `md5code`, `stat`, `isshared`) VALUES (%d,'%s','%s',%ld,%d,UUID(),'%s',%d,%d)",p_file->fileowner,p_file->localname,p_file->fdfsname,p_file->size,p_file->filetype,p_file->md5code,p_file->stat,p_file->isshared);

    ret=mysql_none_query(handle,sql_insert);


    return ret;
}
//通过文件id 获取文件内容
int file_get_by_user(MYSQL *handle, UserInfo * user, FileResult *p_result)
{
    int ret=-1;

    char sql_select[128]={0};
    if(p_result ==NULL)
    {
        puts("file_insert file is NULL");
        return -1;
    }
    sprintf(sql_select,"SELECT * FROM fileinfo where fileowner=%d and stat!=1 order by addtime desc",user->uid);

    MYSQL_RES *p_res=NULL;
    MYSQL_FIELD *fields=NULL;
    do
    {
        p_res=mysql_m_query(handle,sql_select);
        if(p_res ==NULL)
        {
            print_error(handle,"file_get_byid error");
            ret= -1;
            break;
        }

        fields=mysql_fetch_fields(p_res);

        //获得的查出的数据的子段个数
        unsigned int fleng= mysql_num_fields(p_res);
        if(fleng <1)
        {
            ret= 0;
            break;
        }
        unsigned int rowcount=mysql_num_rows(p_res);
        if(rowcount <1)
        {
            ret= 0;
            break;
        }
        p_result->len=rowcount;
        /*
fid,fileowner,localname,fdfsname,size,filetype,uuid,md5code,addtime
stat,isshared
        */
        p_result->result=(FileInfo*)calloc(sizeof(FileInfo),rowcount);
        int i;
        for(i=0;i<rowcount;++i)
        {
            MYSQL_ROW row=NULL;
            row=mysql_fetch_row(p_res);
            int j;
            for(j=0;j<fleng;++j)
            {
                file_copy_attr(&(p_result->result[i]),fields[j].name,row[j]);
            }
        }

    }while(0);

    if(p_res !=NULL)
    {
        mysql_free_result(p_res);
    }
    return ret;
}

void file_free_fileresult(FileResult *p_fr)
{
    if(p_fr->len>0)
    {
        if(p_fr->result !=NULL)
        {
            free(p_fr->result);
            p_fr->result=NULL;
        }
        p_fr->len=0;
    }
}
/*UPDATE `fileinfo` SET uuid=UUID() where fid=1*/
my_ulonglong file_update(MYSQL *handle, FileInfo *file)
{
    if(handle==NULL||file==NULL)
    {
        return -1;
    }
    /*
UPDATE fileinfo` SET `fid`='2', `fileowner`='1', `localname`='诗诗(1).png', `fdfsname`='wKhSGVlxnLaAV_tSACK-BJXAP4s363.png', `size`='2239597', `filetype`='2', `uuid`='3db08a42-6ebc-11e7-86ad-000c29fc2cb2', `md5code`='sfgdfjdzfgwerereirubcvzxzdggafd', `addtime`='2017-07-22 17:00:42', `stat`='0', `isshared`='0' WHERE (`fid`='2');
    */
    int ret=0;
    char sql[1024]={0};
    char timestr[32]={0};
    struct tm *tm_t=localtime(&file->addtime);
    strftime(timestr,32,"%Y-%m-%d %H:%M:%S",tm_t);
    sprintf(sql,"UPDATE `fileinfo` SET `fileowner`=%d, `localname`='%s', `fdfsname`='%s', `size`='%ld', `filetype`='%d', `uuid`='%s', `md5code`='%s', `addtime`='%s', `stat`='%d', `isshared`='%d' WHERE (`fid`='%d')",file->fileowner,file->localname,file->fdfsname,file->size,file->filetype,file->uuid,file->md5code,timestr,file->stat,file->isshared,file->fid);

    my_ulonglong afrows=0;
    afrows=mysql_none_query(handle,sql);

    return afrows;
}

void file_copy_attr(FileInfo *p_file,const char *field,const char *attr)
{
    /*
 * fid,fileowner,localname,fdfsname,size,filetype,uuid,md5code,addtime
stat,isshared
*/
    if(p_file==NULL||field==NULL||attr==NULL)
    {
        return;
    }
    size_t len=strlen(attr);

    if(strcmp(field,"fid")==0)
    {
        p_file->fid=atoi(attr);
        return ;
    }
    if(strcmp(field,"fileowner")==0)
    {
        p_file->fileowner=atoi(attr);
        return ;
    }
    if(strcmp(field,"localname")==0)
    {
        memcpy(p_file->localname,attr,len);
        return ;
    }
    if(strcmp(field,"fdfsname")==0)
    {
        memcpy(p_file->fdfsname,attr,len);
        return ;
    }
    if(strcmp(field,"size")==0)
    {
        p_file->size=atoi(attr);
        return ;
    }
    if(strcmp(field,"filetype")==0)
    {
        p_file->filetype=atoi(attr);
        return ;
    }
    if(strcmp(field,"uuid")==0)
    {
        memcpy(p_file->uuid,attr,len);
        return ;
    }
    if(strcmp(field,"md5code")==0)
    {
        memcpy(p_file->md5code,attr,len);
        return ;
    }
    if(strcmp(field,"addtime")==0)
    {
        p_file->addtime= strtotime_t(attr);
        return ;
    }
    if(strcmp(field,"stat")==0)
    {
        p_file->stat=atoi(attr);
        return ;
    }
    if(strcmp(field,"isshared")==0)
    {
        p_file->isshared=atoi(attr);
        return ;
    }
}

void printfileinfo(FileInfo *p_file, int len)
{
    int i;
    for(i=0;i<len;++i)
    {
        printf(" `fileowner`:%d \n `localname`:'%s' \n `fdfsname`:'%s' \n `size`:%ld \n `filetype`: %d \n `uuid`:'%s' \n `md5code`:'%s' \n `stat`:%d\n `isshared`:%d \n",p_file[i].fileowner,p_file[i].localname,p_file[i].fdfsname,p_file[i].size,p_file[i].filetype,p_file[i].uuid,p_file[i].md5code,p_file[i].stat,p_file[i].isshared);
        puts("============================================");
    }

}
//时间字符串转换成 time_t类型
time_t strtotime_t(const char *timestr)
{
    time_t timesp=0;
    struct tm tm_t;
    bzero(&tm_t,sizeof(tm_t));
    strptime(timestr,"%Y-%m-%d %H:%M:%S",&tm_t);
    timesp=mktime(&tm_t);

    return timesp;
}

void CreateFileinfo(FileInfo *file)
{
    file->fid=2;
    file->addtime=time(NULL);
    char *name="wKhSGVlxnKOAY8BeACgf5W3Yx0E492.png";
    memcpy(file->fdfsname,name,strlen(name));
    file->fileowner=2;
    file->filetype=2;
    file->isshared=0;
    file->fileowner=2;
    name="腾讯全国地名LocList.xml";
    memcpy(file->localname,name,strlen(name));
    name="ljioeutoadlljhl";
    memcpy(file->md5code,name,strlen(name));
    name="3db08a42-6ebc-11e7-86ad-000c29fc2cb2";
    memcpy(file->uuid,name,strlen(name));

    file->size=2629605;
    file->stat=0;

}

void CreateUser(UserInfo *user)
{

    user->uid=2;
    char *p="nami";
    memcpy(user->name,p,strlen(p));
    p="111";
    memcpy(user->passwd,p,strlen(p));
    user->deptid=3;
    user->sex=2;
    user->age=19;
    p="nami@qq.com";
    memcpy(user->email,p,strlen(p));
    user->stat=0;
    user->rectime=1500800843;

}

int file_get_by_result( MYSQL_RES *result, FileInfo *file)
{
    int ret=0;
    if(result==NULL || file==NULL)
    {
        return -1;
    }
    MYSQL_RES *p_res=result;
    MYSQL_FIELD *fields=NULL;

    fields=mysql_fetch_fields(p_res);

    //获得的查出的数据的子段个数
    unsigned int fleng= mysql_num_fields(p_res);
    if(fleng <1)
    {
        return -1;
    }
    unsigned int rowcount=mysql_num_rows(p_res);
    if(rowcount <1)
    {
        return 1;
    }
    /*
fid,fileowner,localname,fdfsname,size,filetype,uuid,md5code,addtime
stat,isshared
        */

    int i;
    for(i=0;i<rowcount;++i)
    {
        MYSQL_ROW row=NULL;
        row=mysql_fetch_row(p_res);
        int j;
        for(j=0;j<fleng;++j)
        {
            file_copy_attr(file,fields[j].name,row[j]);
        }
    }
    return ret;
}
