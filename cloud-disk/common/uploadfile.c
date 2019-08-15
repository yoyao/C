#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include "include/cJSON.h"

#define LISTEN_PORT 31763
#define READ_BUFF_SIZE (1024*4)
/*
 * typedef void (*evconnlistener_cb)(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
*/

typedef struct _transinfo
{
    int fd;
    int datasize;//数据域大小
    int readsize;//已经读取的大小
    char hash[33];
} TranInfo;

void OnClientConnect(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *client, int socket_len, void *arg);
void OnRequest(struct evhttp_request *request,void *arg);
int WorkSaveFile(struct bufferevent *bufev,int fd);
int mainxxx(int argc, char *argv[])
{
    int lsfd,ret=0;

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(LISTEN_PORT);
    struct event_base *p_evbase=NULL;
    p_evbase=event_base_new();
    if(p_evbase==NULL)
    {
        perror("create event_base error");
        return 0;
    }
    struct evhttp *p_http=NULL;
    p_http=evhttp_new(p_evbase);
    if(p_http==NULL)
    {
        perror("create evhttp_new error");
        return 0;
    }
    struct evconnlistener *listener=NULL;

    struct evhttp_bound_socket *p_bound_socket=NULL;
    p_bound_socket=evhttp_bind_socket_with_handle(p_http,"0.0.0.0",LISTEN_PORT);

    evhttp_set_gencb(p_http,OnRequest,(void*)p_http);

    //设置特定路径的回调方法
    // evhttp_set_cb(p_http,)

    printf("\nProgram Start........\n\n");
    event_base_dispatch(p_evbase);
    //只接收POST请求
    evhttp_set_allowed_methods(p_http,EVHTTP_REQ_POST);


    evconnlistener_free(listener);
    evhttp_free(p_http);
    event_base_free(p_evbase);
    return 0;
}

void OnRequest(struct evhttp_request *request,void *arg)
{
    //printf("client %s request\n",evhttp_request_get_host(request));
    struct evhttp_connection *p_conn= evhttp_request_get_connection(request);
    char *ip=NULL;
    int port=NULL;
    evhttp_connection_get_peer(p_conn,&ip,(uint16_t*)&port);
    printf("client %s:%d request\n",ip,port);
    sleep(10);
    struct evbuffer *databuf=evbuffer_new();
    evbuffer_add(databuf,"hello world",11);
    evhttp_send_reply(request,200,"OK",databuf);

}

void OnSockReadable(struct bufferevent *bufev,void *arg);
void OnSockWriteable();
void OnsocketEvent(struct bufferevent *bufev, short events, void *arg);

int main(int argc, char *argv[])
{
    int lsfd,ret=0;
    lsfd=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(LISTEN_PORT);
    struct event_base *p_evbase=NULL;
    p_evbase=event_base_new();
    if(p_evbase==NULL)
    {
        perror("create event_base error");
        return 0;
    }
    struct evconnlistener *listener=NULL;
    listener=evconnlistener_new_bind(p_evbase,OnClientConnect,(void*)p_evbase,LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,SOMAXCONN,(struct sockaddr*)&addr,sizeof(addr));

    evconnlistener_enable(listener);

    printf("\nProgram Start........\n\n");
    event_base_dispatch(p_evbase);

    evconnlistener_free(listener);
    event_base_free(p_evbase);
    return 0;
}

void OnClientConnect(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *client, int socket_len, void *arg)
{
    struct sockaddr_in *addr=(struct sockaddr_in*)client;
    char ip[24]={0};
    inet_ntop(AF_INET,&addr->sin_addr.s_addr,ip,socket_len);
    printf("client %s:%d connect\n",ip,ntohs(addr->sin_port));
    struct bufferevent *bufev=NULL;
    bufev=bufferevent_socket_new((struct event_base*)arg,fd,BEV_OPT_CLOSE_ON_FREE|EV_PERSIST|EV_ET);
    if(bufev==NULL)
    {
        printf("set bufevent error\n");
        return;
    }
    TranInfo *p_tran=(TranInfo*)calloc(sizeof(TranInfo),1);
    p_tran->fd=-1;
    bufferevent_setcb(bufev,OnSockReadable,NULL,OnsocketEvent,p_tran);
    //只监听读事件
    bufferevent_enable(bufev,EV_READ);

}

void OnSockReadable(struct bufferevent *bufev,void *arg)
{
    int len;
    int datalen=-1;
    int jsonlen=-1;
    int ret=0;
    char buff[READ_BUFF_SIZE]={0};
    char datalen_buf[4]={0};
    char jsonlen_buf[4]={0};
    char *p_json=NULL;

    TranInfo *p_tran=(TranInfo*)arg;

    while (1)
    {
        if(p_tran->fd<0)
        {
            len=bufferevent_read(bufev,(char*)&jsonlen,4);
            if(len==0)
            {
                ret=-1;
                //缓冲区没有内容但是强行读取
                break;
            }

            //printf("datalen %d\n",jsonlen);
            if(jsonlen>210000)
            {
                printf("data too large\n");
                ret=-1;
                break;
            }
            p_json=(char*)calloc(sizeof(char),jsonlen+1);
            len=bufferevent_read(bufev,p_json,jsonlen);
            if(len==0 && errno==EAGAIN)
            {
                ret=-1;
                break;
            }
            printf("json string :\n%s\n",p_json);
            cJSON *jdoc= cJSON_Parse(p_json);
            if(jdoc ==NULL)
            {
                ret=1;
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

            p_tran->datasize=cj_size->valueint;

            char path[1024]={0};
            sprintf(path,"/home/fgo/document/course/cloud-disk/common/%s",cj_name->valuestring);

            p_tran->fd= open(path,O_CREAT|O_WRONLY|O_APPEND,0644);
            if(p_tran->fd<0)
            {
                ret=6;
                break;
            }
            free(p_json);
        }
        else
        {
            len=bufferevent_read(bufev,buff,READ_BUFF_SIZE);
            //printf("read length %d\n",len);
            if(len==0)
            {
                ret=-1;
                break;
            }
            int wlen= write(p_tran->fd,buff,len);
            if(wlen !=len)
            {
                ret=-2;
                break;
            }
            p_tran->readsize+=wlen;
            char result[512]={0};
            sprintf(result,"{\"status\":%d,\"error\":%d,\"succ\":%d,\"msg\":\"succ%d\"}",ret,ret,ret,ret);
            if(p_tran->readsize==p_tran->datasize)
            {
                bufferevent_write(bufev,result,strlen(result));
                printf("The part end\n");
                close(p_tran->fd);
                p_tran->fd=-1;
                p_tran->readsize=0;
                p_tran->datasize=0;
            }
              memset(buff,0,len);
        }
    }


}

int WorkSaveFile(struct bufferevent *bufev,int fd)
{
    char *p_content;
    char *p_back=NULL;
    char *p_json=NULL;
    int len;
    int datalen=-1;
    int ret=0;
    //  int fd;

    do
    {
        char *tmp=NULL;
        strstr(p_content,"}\r\n\r\n");
        //+2是为了把大括号和\0复制进去
        p_json=(char*)calloc(sizeof(char),abs(tmp-p_content)+2);

        //+1是为了把大括号复制进去
        memcpy(p_json,p_content,abs(tmp-p_content)+1);
        printf("pjson %s\n",p_json);
        cJSON *jdoc= cJSON_Parse(p_json);
        if(jdoc ==NULL)
        {
            ret=1;
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
        //  *pfd=fd;
        int wlen= write(fd,tmp+5,cj_size->valueint);
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

    if(p_json!=NULL)
    {
        free(p_json);
        p_json=NULL;
    }
    return ret;
}

void OnsocketEvent(struct bufferevent *bufev, short events, void *arg)
{
    if(events&BEV_EVENT_EOF)
    {
        printf("client exit\n");
        TranInfo *p_tran=(TranInfo*)arg;
        if(p_tran !=NULL)
        {
            free(p_tran);
        }
        bufferevent_free(bufev);


    }
}
