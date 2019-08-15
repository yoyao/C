#ifdef __cplusplus
extern "C"
{
#endif
#ifndef lint
static const char rcsid[] = "$Id: threaded.c,v 1.9 2001/11/20 03:23:21 robs Exp $";
#endif /* not lint */

#include <fcgi_config.h>
#include <fcgi_stdio.h>
#include <pthread.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "fcgiapp.h"


#define THREAD_COUNT 20

static int counts[THREAD_COUNT];

static void *doit(void *a)
{
    int rc, i, thread_id = (int)a;
    pid_t pid = getpid();
    FCGX_Request request;
    char *server_name;

    FCGX_InitRequest(&request, 0, 0);

    for (;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

        static pthread_mutex_t counts_mutex = PTHREAD_MUTEX_INITIALIZER;

        /* Some platforms require accept() serialization, some don't.. */
        pthread_mutex_lock(&accept_mutex);
        rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);

        if (rc < 0)
            break;

        server_name = FCGX_GetParam("SERVER_NAME", request.envp);

        FCGX_FPrintF(request.out,
                     "Content-type: text/html\r\n"
                     "\r\n"
                     "<title>FastCGI Hello! (multi-threaded C, fcgiapp library)</title>"
                     "<h1>FastCGI Hello! (multi-threaded C, fcgiapp library)</h1>"
                     "Thread %d, Process %ld<p>"
                     "Request counts for %d threads running on host <i>%s</i><p><code>",
                     thread_id, pid, THREAD_COUNT, server_name ? server_name : "?");

        sleep(20);

        pthread_mutex_lock(&counts_mutex);
        ++counts[thread_id];
        for (i = 0; i < THREAD_COUNT; i++)
            FCGX_FPrintF(request.out, "%5d " , counts[i]);
        pthread_mutex_unlock(&counts_mutex);

        FCGX_Finish_r(&request);
    }

    return NULL;
}

int main(void)
{
    while (FCGI_Accept() >= 0)
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        int count = 0,ret=0,len;
        len=atoi(contentLength);
        //返回报文必须带有Content-type: text/html  要不就无法显示 报错
        printf("Content-type: text/html\r\n");
        //HTTP响应报文中响应头最后的一行
        printf("\r\n");
        sleep(10);
        printf("hello world");
    }

    return 0;
}


#ifdef __cplusplus
}
#endif
