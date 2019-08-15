#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include "include/upload_file.h"
#include <fcgi_config.h>
#include <fcgi_stdio.h>

extern char **environ;

//find 'substr' from a fixed-length buffer
//('full_data' will be treated as binary data buffer)
//return NULL if not found
char* memstr(char* full_data, int full_data_len, char* substr)
{
    if (full_data == NULL || full_data_len <= 0 || substr == NULL)
    {
        return NULL;
    }

    if (*substr == '\0')
    {
        return NULL;
    }

    int sublen = strlen(substr);

    int i;
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;
    for (i = 0; i < last_possible; i++)
    {
        if (*cur == *substr)
        {
            //assert(full_data_len - i >= sublen);
            if (memcmp(cur, substr, sublen) == 0)
            {
                //found
                return cur;
            }
        }
        cur++;
    }

    return NULL;
}


int PostFile_GetByResponseStr(PostFile *file, char *src, int len)
{
    int ret = 0;
    if (file == NULL || src == NULL || len<1)
    {
        printf("PostFile_GetByResponseStr argv is NULL\n");
        return 1;
    }
    char *p_front = NULL, p_back = NULL,*p_raw=src;
    char *filename = "filename=\"";
    char *type = "Content-Type:";
    char *endsign = "\r\n------WebKitFormBoundary";

    size_t namelen = strlen(filename);
    size_t typelen = strlen(type);
    size_t endlen = strlen(endsign);

    p_front = memstr(src, len, filename);
    if (p_front == NULL)
    {
        printf("PostFile_GetByResponseStr src is invalid\n");
        return 2;
    }
    //指向filename="]BV9N9V7GUTSYRGZBE%7F_7.gif"的文件名首位置]处
    src += (p_front-src) + namelen;
    //\"\r\n是文件名的结束
    p_front = memstr(src, len - abs(src - p_raw), "\"\r\n");
    if (p_front == NULL)
    {
        printf("PostFile_GetByResponseStr filename end not found\n");
        return 3;
    }
    file->name = (char*)calloc(abs(p_front - src), sizeof(char));
    if (file->name == NULL)
    {
        printf("PostFile_GetByResponseStr file->name = (char*)calloc error\n");
        return -1;
    }
    memcpy(file->name, src, abs(p_front - src));
    /*至此 得到文件名*/

    p_front = memstr(p_front, len - abs(p_front-p_raw), type);
    if (p_front == NULL)
    {
        printf("PostFile_GetByResponseStr content-type sign not found\n");
        return 3;
    }
    //src 指向Content-Type:image/gif的i的位置
    src = (p_front + typelen);
    //寻找Content-Type:image/gif的结束为止
    p_front = memstr(src, len - abs(src - p_raw), "\r\n\r\n");
    if (p_front == NULL)
    {
        printf("PostFile_GetByResponseStr content-type end not found\n");
        return 4;
    }
    file->type = (char*)calloc(abs(p_front - src), sizeof(char));
    if (file->type == NULL)
    {
        printf("PostFile_GetByResponseStr file->type = (char*)calloc error\n");
        return -2;
    }
    memcpy(file->type, src, abs(p_front - src));
    /*至此 得到Content-Type:*/

    //跳过image/gif后的\r\n\r\n指向POST数据的开始
    src=(p_front + 4);

    //寻找\r\n------WebKitFormBoundary为结束标记的地方 就可以真正拷贝出穿过来的数据域
    p_front = memstr(src, len - abs(src - p_raw), endsign);
    if (p_front == NULL)
    {
        printf("PostFile_GetByResponseStr data end sign not found\n");
        return 5;
    }
    file->datalen = abs(p_front - src);
    file->data = (char*)calloc(file->datalen, sizeof(char));
    memcpy(file->data, src, file->datalen);
    /*至此 得到完整的数据域*/

    return ret;
}

void PostFile_free(PostFile *pf)
{
    if (pf == NULL)
    {
        return;
    }
    if (pf->data != NULL)
    {
        free(pf->data);
        pf->data = NULL;
    }
    if (pf->name != NULL)
    {
        free(pf->name);
        pf->name = NULL;
    }
    if (pf->type != NULL)
    {
        free(pf->type);
        pf->type = NULL;
    }
    pf->datalen = 0;

}





int mainx(int argc, char *argv[])
{
    char **initialEnv = environ;
    char *p_result=NULL;

    char buf[1024]={0};

    //如果大于0 就是POST过来的请求体 是有数据的 要是GET就会是0
    while (FCGI_Accept() >= 0)
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        int count = 0,ret=-1,len;
        len=atoi(contentLength);
        //返回报文必须带有Content-type: text/html  要不就无法显示 报错
        printf("Content-type: text/html\r\n");
        //HTTP响应报文中响应头最后的一行
        printf("\r\n");
        if(len<1)
        {
            printf("file data not found\n");
            return 0;
        }
        p_result =(char*)calloc(sizeof(char),len);
        while (count<len)
        {
            p_result[count]=getchar();
            ++count;
        }
        PostFile file;
        do
        {
            if(count !=len)
            {
                printf("Read file length is not equal\n");
                break;
            }
            printf("<br>Content length %d,read count %d<br>",len,count);
            bzero(&file,sizeof(file));
            ret=PostFile_GetByResponseStr(&file,p_result,len);
            if(ret !=0)
            {
                printf("PostFile_GetByResponseStr error\n");
                break;
            }
            char filepath[128]={0};
            sprintf(filepath,"/home/fgo/picture/%s",file.name);
            int fd=open(filepath,O_CREAT|O_WRONLY,0644);
            if(fd<0)
            {
                printf("Save file failed\n");
                break;
            }
            //保存上传的数据
            write(fd,file.data,len);
            close(fd);
            printf("Save file successfull<br>");
            printf("the file name %s<br>",file.name);
            printf("the file type %s<br>",file.type);

        }while(0);

        PostFile_free(&file);
        free(p_result);
        p_result =NULL;
    } /* while */

    return 0;
}


