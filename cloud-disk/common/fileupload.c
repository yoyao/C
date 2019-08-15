
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
#include "include/cJSON.h"
#include <time.h>

#if 1
int main(int argc, char *argv[])
{
    //如果大于0 就是POST过来的请求体 是有数据的 要是GET就会是0
    //
    while (FCGI_Accept() >= 0)
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        int count = 0,ret=0,len,fd=-1,issucc=-1;
        len=atoi(contentLength);
        //返回报文必须带有Content-type: text/html  要不就无法显示 报错
        printf("Content-type: text/html\r\n");
        //HTTP响应报文中响应头最后的一行
        printf("\r\n");

        char *p_content=NULL;
        char *p_json=NULL;
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
            char *tmp=strstr(p_content,"}\r\n\r\n");
            //+2是为了把大括号和\0复制进去
            p_json=(char*)calloc(sizeof(char),abs(tmp-p_content)+2);
            //+1是为了把大括号复制进去
            memcpy(p_json,p_content,abs(tmp-p_content)+1);

            cJSON *jdoc= cJSON_Parse(p_json);
            if(jdoc ==NULL)
            {
                ret=-1;
                break;
            }
            cJSON *cj_name=NULL;
            cj_name=cJSON_GetObjectItem(jdoc,"localname");
            if(cj_name==NULL)
            {
                ret=7;
                break;
            }

            cJSON *cj_size=NULL;
            cj_size=cJSON_GetObjectItem(jdoc,"datasize");
            if(cj_size==NULL)
            {
                ret=2;
                break;
            }
            cJSON *cj_filehash=NULL;
            cj_filehash =cJSON_GetObjectItem(jdoc,"filehash");
            if(cj_filehash==NULL)
            {
                ret=3;
                break;
            }
            cJSON *cj_user=cJSON_GetObjectItem(jdoc,"user");
            cJSON *cj_token=NULL;
            cj_token =cJSON_GetObjectItem(cj_user,"token");
            if(cj_token==NULL)
            {
                ret=4;
                break;
            }
            cJSON *cj_uid=NULL;
            cj_uid =cJSON_GetObjectItem(cj_user,"uid");
            if(cj_uid==NULL)
            {
                ret=5;
                break;
            }
            char path[1024]={0};
            sprintf(path,"/home/fgo/document/course/cloud-disk/common/%s",cj_name->valuestring);
            fd= open(path,O_CREAT|O_WRONLY|O_APPEND,0644);
            if(fd<0)
            {
                ret=6;
                break;
            }
//            p_data= (char*)calloc(sizeof(char),cj_size->valueint);
//            memcpy(p_data,,cj_size->valueint);
            int wlen= write(fd,tmp+5,cj_size->valueint);
            close(fd);
            if(wlen !=cj_size->valueint)
            {
                ret=-2;
                break;
            }
            cJSON *cj_isend=NULL;
            cj_isend=cJSON_GetObjectItem(jdoc,"isend");
            if(cj_isend !=NULL)
            {
                //上传到fdfs

            }

        }while(0);

        if(p_content!=NULL)
        {
            free(p_content);
            p_content=NULL;
        }
        if(p_json!=NULL)
        {
            free(p_json);
            p_json=NULL;
        }
        printf("{\"status\":%d,\"error\":%d,\"succ\":%d\"msg\":\"succ%d\"}",ret,ret,ret,ret);

        fflush(FCGI_stdout);
    }

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif
