#include <sys/errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "include/userinfo_op.h"
#include "include/fileinfo_op.h"
#include "include/cJSON.h"
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <uuid.h>

void *threadfunc(void *arg);

#define DB_USER_    "root"
#define DB_PWD_     "123"
#define DB_DB_NAME_ "clouddisk"

extern void CreateFileinfo(FileInfo *file);

#define true 1
#define false 0

int main(int argc, char *argv[])
{   
    char *json="{\"name\":12}";

    int lsfd=-1,ret=0;
    lsfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_addr.s_addr=0;
    local.sin_family=AF_INET;
    local.sin_port=htons(24127);

    bind(lsfd,(struct sockaddr*)&local,sizeof(local));
    listen(lsfd,128);
    puts("\n Start accept...\n");
    while (1)
    {
        struct sockaddr_in client;
        bzero(&client,sizeof(client));
        int len=sizeof(client);
        int cfd= accept(lsfd,(struct sockaddr*)&client,&len);
        puts("Client connect\n");
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        pthread_create(&thread,&attr,threadfunc,(void*)cfd);
        pthread_attr_destroy(&attr);
    }

    return 0;
}

void *threadfunc(void *arg)
{
    int fd,filefd,flag,ret=0,len;
    fd=(int)arg;
    len=0;
    flag=fcntl(fd,F_GETFL);

    flag|=O_NONBLOCK;
    ret= fcntl(fd,F_SETFL,flag);//设置非阻塞
    if(ret!=0)
    {
        puts("nonblock error\n");
        close(fd);
        return NULL;
    }
    flag=false;
    filefd=-1;
    cJSON *root=NULL,
            *cj_length=NULL;
    char *content=NULL;

    char response[1024]={0};
    int sign=0;
    while (1)
    {
        char buf[1024]={0};
        char ch;
        len= recv(fd,&ch,sizeof(ch),MSG_DONTWAIT);
        //这代表对方未发送数据,但是连接未关闭
        if(len<0 && errno==EAGAIN)
        {
            if(sign)
            {
                printf("length %d\n",sign);
            }
            sign=0;
            continue;
        }
        //断开了连接
        if(len==0)
        {
            puts("client exit\n");
            close(fd);
            pthread_exit(NULL);
        }
        ++sign;
        printf("%c",ch);

        continue;

        do
        {
            //这是第一次传输 是通知下次报文长度消息 返回报文为0 就是说我知道了下次报文的长度
            if(flag==false)
            {

                //{"length":312,"timespan":314565,"hash","ghfssh","user":{"uid":1,"name":"adg","token","alkjgfyo"}}
                root=cJSON_Parse(buf);
                if(root==NULL)
                {
                    ret=-1;
                    break;
                }
                cj_length=cJSON_GetObjectItem(root,"length");
                if(cj_length==NULL)
                {
                    ret=-2;
                    break;
                }
                //长度为0 毫无意义
                if(cj_length->valueint<1)
                {
                    ret=1;
                    break;
                }
                // 意味着我知道下一次的报文长度是多少了
                flag=true;
            }

            if(flag==true)
            {
                cJSON *cj_filename=NULL;
                cJSON *cj_isend=NULL;
                cJSON *cj_data=NULL;
                cJSON *cj_datasize=NULL;
                content=calloc(sizeof(char),cj_length->valueint);
                if(content==NULL)
                {
                    ret=-1;
                    break;
                }
                root=cJSON_Parse(content);
                if(root==NULL)
                {
                    ret=-2;
                    break;
                }
                cj_filename=cJSON_GetObjectItem(root,"localname");
                if(cj_filename==NULL)
                {
                    ret=-3;
                    break;
                }
                cj_data=cJSON_GetObjectItem(root,"data");
                if(cj_data==NULL)
                {
                    ret=-4;
                    break;
                }
                cj_datasize=cJSON_GetObjectItem(root,"datasize");
                if(cj_datasize==NULL)
                {
                    ret=-5;
                    break;
                }

                char path[1024]={0};
                sprintf(path,"./%s",cj_filename->valuestring);
                if(filefd ==-1)
                {
                    filefd=open(path,O_CREAT|O_WRONLY|O_TRUNC|O_APPEND,0644);
                    if(filefd ==-1)
                    {
                        ret=2;
                        break;
                    }
                }
                //写入文件
                write(filefd,cj_data->valuestring,cj_datasize->valueint);
                cj_isend=cJSON_GetObjectItem(root,"isend");
                if(cj_isend !=NULL)
                {
                    printf("write file end\n");
                    close(filefd);
                    ret =200;
                }
                // 意味着我已知的长度已经接收到了,你应该给我再次确认报文的长度
                flag=true;
                free(content);
            }

        }while(0);
        time_t cur=time((void*)NULL);
        sprintf(response,"{\"status\":%d,\"timespan\":%ld}",ret,cur);
        //只要不等于0 就是上面的运行出错了
        if(ret !=0 && ret!=200)
        {
            printf("thread exit\n");
            pthread_exit(NULL);
        }
    }
}
