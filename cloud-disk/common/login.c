
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
#include "include/cJSON.h"
#include <time.h>

extern char **environ;

#define DB_USER_    "root"
#define DB_PWD_     "123"
#define DB_DB_NAME_ "clouddisk"

int main(int argc, char *argv[])
{
    char *p_result=NULL;
    char buf[2048]={0};

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

            cJSON *jdoc= cJSON_Parse(p_content);
            if(jdoc ==NULL)
            {
                ret=2;
                break;
            }

            cJSON *cjacc=NULL;
            cjacc=cJSON_GetObjectItem(jdoc,"account");
            if(cjacc==NULL)
            {
                ret=2;
                break;
            }
            cJSON *cjpwd=NULL;
            cjpwd =cJSON_GetObjectItem(jdoc,"passwd");
            if(cjpwd==NULL)
            {
                ret=3;
                break;
            }
            ret= user_get_by_account_pwd(mysql_handle,&user,cjacc->valuestring,cjpwd->valuestring);
            if(ret !=0||user.uid==0)
            {
                ret=4;
                break;
            }
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
        /*
        uid,name,passwd,deptid,sex,age,email,stat,rectime,mark
        */
        //没有找到用户
        if(user.uid==0)
        {
            sprintf(buf,"{\"status\":%d,\"timespan\":%ld,\"token\":\"ERROR\",\"user\":{\"uid\":-1,\"name\":\"NULL\",\"pwd\":\"NULL\"}}",ret,time(NULL));
        }
        else
        {
            sprintf(buf,"{\"status\":%d,\"timespan\":%ld,\"token\":\"successful\",\"user\":{\"uid\":%d,\"name\":\"%s\",\"passwd\":\"%s\",\"deptid\":%d,\"sex\":%d,\"age\":%d,\"email\":\"%s\",\"stat\":%d,\"rectime\":%ld}}",ret,time(NULL),user.uid,user.name,user.passwd,user.deptid,user.sex,user.age,user.email,user.stat,user.rectime);
            //存redis中


        }

        fwrite(buf,len,sizeof(char),FCGI_stdout);
        fflush(FCGI_stdout);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
