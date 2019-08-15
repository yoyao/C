
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <fcgi_config.h>
#include <fcgi_stdio.h>
#include "include/userinfo_op.h"
#include "include/fileinfo_op.h"
#include "include/cJSON.h"
#include <time.h>

extern char **environ;

#define DB_USER_    "root"
#define DB_PWD_     "123"
#define DB_DB_NAME_ "clouddisk"
#if 1
int main(int argc, char *argv[])
{
    char *p_result=NULL;
    char buf[1024]={0};
    char *p_users=NULL;
    //如果大于0 就是POST过来的请求体 是有数据的 要是GET就会是0
    while (FCGI_Accept() >= 0)
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        int count = 0,ret=0,len;
        len=atoi(contentLength);
        //返回报文必须带有Content-type: text/html  要不就无法显示 报错
        printf("Content-type: text/html\r\n");
        //HTTP响应报文中响应头最后的一行
        printf("\r\n");

        char *p_content=NULL;
        UserInfo user;
        FileResult fileres;
        bzero(&fileres,sizeof(fileres));
        bzero(&user,sizeof(user));

        MYSQL *mysql_handle=NULL;
        mysql_handle=mysql_m_conn(DB_USER_, DB_PWD_,DB_DB_NAME_);
        if(mysql_handle==NULL)
        {
            puts("mysql_m_conn error\n");
            return -1;
        }

        do
        {
            if(len<1)
            {
                ret=1;
                break;
            }
            p_content= (char*)calloc(sizeof(char),len);
            while (count<len)
            {
                p_content[count]=getchar();
                ++count;
            }
            memcpy(buf,p_content,len);
            cJSON *jdoc= cJSON_Parse(p_content);
            if(jdoc ==NULL)
            {
                ret=2;
                break;
            }

            cJSON *cjuid=NULL;
            cjuid=cJSON_GetObjectItem(jdoc,"uid");
            if(cjuid==NULL)
            {
                ret=2;
                break;
            }
            user.uid=cjuid->valueint;

            cJSON *cjtimespan=NULL;
            cjtimespan =cJSON_GetObjectItem(jdoc,"timespan");
            if(cjtimespan==NULL)
            {
                ret=3;
                break;
            }
            cJSON *cjtoken=NULL;
            cjtoken =cJSON_GetObjectItem(jdoc,"token");
            if(cjtoken==NULL)
            {
                ret=3;
                break;
            }
            file_get_by_user(mysql_handle,&user,&fileres);
            /*
             * fid
fileowner,localname,fdfsname,size,filetype,uuid,md5code,addtime,stat,isshared
             {"status":0,"timespan":23,"msg":"","count":2,"list":[
{},
{}
]}
            */
            printf("{\"status\":0,\"timespan\":%ld,\"msg\":\"\",\"count\":%d,\"list\":[",time(NULL),fileres.len);
            if(fileres.len>0)
            {
                for(count=0;count<fileres.len;++count)
                {
                    printf("{\"fid\":%d,\"stat\":%d,\"localname\":\"%s\",\"size\":%d,\"filetype\":%d,\"addtime\":%ld,\"isshared\":%d}",fileres.result[count].fid,fileres.result[count].stat,fileres.result[count].localname,fileres.result[count].size,fileres.result[count].filetype,fileres.result[count].addtime,fileres.result[count].isshared);
                    if(count!=fileres.len-1)
                    {
                        putchar(',');
                    }
                }
            }
            printf("]}");
            file_free_fileresult(&fileres);

        }while(0);

        /*
{"status":1,"timespan":3425,"token":"","user":{"uid":1,"name":"","pwd":""}}
*/
        mysql_close(mysql_handle);
        mysql_handle=NULL;

        if(p_content!=NULL)
        {
            free(p_content);
            p_content=NULL;
        }
        fflush(FCGI_stdout);
    }

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif
