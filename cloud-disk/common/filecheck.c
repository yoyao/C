
#ifdef __cplusplus
extern "C"
{
#endif
#include <sys/errno.h>
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
extern void printfileinfo(FileInfo *p_file, int len);

#define DB_USER_    "root"
#define DB_PWD_     "123"
#define DB_DB_NAME_ "clouddisk"

#if 1
int main(int argc, char *argv[])
{
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

        MYSQL *mysql_handle=NULL;
        MYSQL_RES *p_result=NULL;
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
            p_content= (char*)calloc(sizeof(char),len+1);
            while (count<len)
            {
                p_content[count]=getchar();
                ++count;
            }
            cJSON *jdoc= cJSON_Parse(p_content);
            if(jdoc ==NULL)
            {
                ret=-1;
                break;
            }
            cJSON *cj_size=NULL;
            cj_size=cJSON_GetObjectItem(jdoc,"filesize");
            if(cj_size==NULL)
            {
                ret=2;
                break;
            }
            cJSON *cj_name=NULL;
            cj_name=cJSON_GetObjectItem(jdoc,"localname");
            if(cj_name==NULL)
            {
                ret=7;
                break;
            }
            cJSON *cj_filehash=NULL;
            cj_filehash =cJSON_GetObjectItem(jdoc,"filehash");
            if(cj_filehash==NULL)
            {
                ret=3;
                break;
            }

            jdoc=cJSON_GetObjectItem(jdoc,"user");
            cJSON *cj_token=NULL;
            cj_token =cJSON_GetObjectItem(jdoc,"token");
            if(cj_token==NULL)
            {
                ret=4;
                break;
            }
            cJSON *cj_uid=NULL;
            cj_uid =cJSON_GetObjectItem(jdoc,"uid");
            if(cj_uid==NULL)
            {
                ret=5;
                break;
            }
          //  printf(p_content);
          //  printf("cj_name:%s,cj_size:%ld,cj_filehash:%s,cj_token:%s,cj_uid:%d",cj_name->valuestring,cj_size->valueint,cj_filehash->valueint,cj_token->valuestring,cj_uid->valueint);
            char sql[1024]={0};
            sprintf(sql,"select * from fileinfo where md5code='%s' and stat=0 ",cj_filehash->valuestring);

            p_result=mysql_m_query(mysql_handle,sql);
            if(p_result ==NULL)
            {
                ret=6;
                break;
            }
            FileInfo file;
            bzero(&file,sizeof(file));
            ret= file_get_by_result(p_result,&file);
            if(ret !=0 && ret !=1)
            {
                ret=6;
                break;
            }

            //查了 但是查出的数据行为0  就是没有这个数据
            if(file.fid==0)
            {
                ret=0;
                break;
            }
            //此文件已经存在 而且是它本人上传的
            if(file.fileowner ==cj_uid->valueint)
            {
                ret=201;
                break;
            }

            file.fileowner=cj_uid->valueint;
            strcpy(file.localname,cj_name->valuestring);
            my_ulonglong affect=0;
            affect=file_insert(mysql_handle,&file);
            //文件秒传失败
            if(affect<1)
            {
                ret=300;
                break;
            }
            ret=200;
        }while(0);

        if(mysql_handle!=NULL)
        {
            mysql_close(mysql_handle);
            mysql_handle=NULL;
        }
        if(p_content!=NULL)
        {
            free(p_content);
            p_content=NULL;
        }
        if(p_result!=NULL)
        {
            mysql_free_result(p_result);
            p_result=NULL;
        }
      printf("{\"status\":%d,\"error\":%d,\"msg\":\"succ\"}",ret,ret);

        fflush(FCGI_stdout);
    }

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif
