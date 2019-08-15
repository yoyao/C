/*
 * Proxy on the client side to connect to servers using MD5SIG.
 *
 * Example:
 *   echo "correct horse battery staple" > pw.txt
 *   chmod 600 pw.txt
 *   ssh -oProxyCommand="./tcpauth-client-proxy -P pw.txt %h %p" shell.example.com
 */
/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This is not an official Google product.
 */

#include<errno.h>
#include<netdb.h>
#include<netinet/tcp.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>

#include"common.h"

const char* password = NULL;

void usage(int err)
{
    printf("Usage: %s [options] <host> <port>\n"
           "    -h                   Show this usage text.\n"
           "    -P <password file>   File containing MD5SIG password.\n"
           "", argv0);
    exit(err);
}

typedef struct {
    int src;
    int dst;
} reader;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int shutting_down_var = 0;
static void set_shutting_down()
{
    int n;
    if ((n = pthread_mutex_lock(&mutex)))
    {
        xerror("mutex lock error: code %d", n);
    }
    shutting_down_var = 1;
    if ((n = pthread_mutex_unlock(&mutex)))
    {
        xerror("mutex lock error: code %d", n);
    }
}

static int shutting_down()
{
    int n;
    int ret;
    if ((n = pthread_mutex_lock(&mutex)))
    {
        xerror("mutex lock error: code %d", n);
    }
    ret = shutting_down_var;
    if ((n = pthread_mutex_unlock(&mutex)))
    {
        xerror("mutex lock error: code %d", n);
    }
    return ret;
}

// read data from one and write to the other.
static void* reader_main(void* rin)
{
    const reader* r = (reader*)rin;
    char buf[1024];
    for (;;)
    {
        ssize_t n = read(r->src, buf, sizeof(buf));
        if (shutting_down())
        {
            return NULL;
        }
        if (n == 0)
        {
            set_shutting_down();
            return NULL;
        }
        if (0 > n)
        {
            xerror("read(%d): %s", r->src, strerror(errno));
        }
        const char* p = buf;
        while (n > 0)
        {
            const ssize_t wn = write(r->dst, p, n);
            if (0 > wn)
            {
                xerror("write(): %s", strerror(errno));
            }
            n -= wn;
            p += wn;
        }
    }
}

// Handle a new connection after connect() returns.
// * Enable MD5SIG
// * Funnel data back and forth.
int handle(int fd)
{
    if (1)
    {
        struct tcp_md5sig md5sig;
        socklen_t t = sizeof(struct sockaddr_storage);

        memset(&md5sig, 0, sizeof(md5sig));
        strncpy((char*)md5sig.tcpm_key, password, TCP_MD5SIG_MAXKEYLEN);
/*
这两个函数或者返回与某个套接字关联的本地协议地址（getsockname），或者返回与某个套接字关联的外地协议地址即得到对方的地址（getpeername）。

int getsockname(int sockfd,struct sockaddr* localaddr,socklen_t *addrlen);
int getpeername(int sockfd,struct sockaddr* peeraddr,socklen_t *addrlen);

　　均返回：若成功则为0，失败则为-1
　　getpeername只有在连接建立以后才调用，否则不能正确获得对方地址和端口，所以它的参数描述字一般是已连接描述字而非监听套接口描述字。
    没有连接的UDP不能调用getpeername，但是可以调用getsockname和TCP一样，它的地址和端口不是在调用socket就指定了，而是在第一次调用sendto函数以后。
    已经连接的UDP，在调用connect以后，这2个函数（getsockname，getpeername）都是可以用的。但是这时意义不大，因为已经连接（connect）的UDP已经知道对方的地址。

　　需要这两个函数的理由如下：

　在一个没有调用bind的TCP客户上，connect成功返回后，getsockname用于返回由内核赋予该连接的本地IP地址和本地端口号。

　在以端口号为0调用bind（告知内核去选择本地临时端口号）后，getsockname用于返回由内核赋予的本地端口号。

　在一个以通配IP地址调用bind的TCP服务器上，与某个客户的连接一旦建立（accept成功返回），getsockname就可以用于返回由内核赋予该连接的本地IP地址。在这样的调用中，套接字描述符参数必须是已连接套接字的描述符，而不是监听套接字的描述符。

　当一个服务器的是由调用过accept的某个进程通过调用exec执行程序时，它能够获取客户身份的唯一途径便是调用getpeername。
*/
        if (getpeername(fd, (struct sockaddr*)&md5sig.tcpm_addr, &t))
        {
            xerror("getpeername(): %.100s", strerror(errno));
        }
        md5sig.tcpm_keylen = strlen((char*)md5sig.tcpm_key);
        //设置报文启用MD5
        if (-1 == setsockopt(fd,IPPROTO_TCP, TCP_MD5SIG,&md5sig, sizeof(md5sig)))
        {
            xerror("setsockopt(TCP_MD5SIG): %.100s",strerror(errno));
        }
    }

    // Start one half of the data shuffling.
    pthread_t other;
    reader other_r;
    other_r.src = fd;
    other_r.dst = STDOUT_FILENO;
    //新开线程接收信息
    if (pthread_create(&other, NULL, &reader_main, &other_r))
    {
        xerror("pthread_create(): %s", strerror(errno));
    }

    // Start the other half.
    reader this_r;
    this_r.src = STDIN_FILENO;
    this_r.dst = fd;
    reader_main(&this_r);
    int n;
    if ((n = pthread_join(other, NULL)))
    {
        xerror("pthread_join: %s", strerror(n));
    }
    exit(0);
}

int main(int argc, char** argv)
{
    const char* password_file = NULL;  // -P <password file>
    argv0 = argv[0];

    int c;
    while (EOF != (c = getopt(argc, argv, "hP:")))
    {
        switch (c)
        {
        case 'h':
            usage(0);
        case 'P':
            password_file = optarg;
            break;
        default:
            usage(1);
        }
    }

    if (optind + 2 != argc)
    {
        fprintf(stderr, "%s: Provide exactly two non-option args: host and port.\n", argv0);
        exit(1);
    }
    const char* node = argv[optind];
    const char* port = argv[optind+1];
    password = get_password(password_file);

    // Create socket and bind.
    int fd = -1;
    {
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_ADDRCONFIG;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        struct addrinfo *ai;
/*
IPv4中使用gethostbyname()函数完成主机名到地址解析，这个函数仅仅支持IPv4，且不允许调用者指定所需地址类型的任何信息，返回的结构只包含了用于存储IPv4地址的空间。IPv6中引入了getaddrinfo()的新API，它是协议无关的，既可用于IPv4也可用于IPv6。getaddrinfo函数能够处理名字到地址以及服务到端口这两种转换，返回的是一个addrinfo的结构（列表）指针而不是一个地址清单。这些addrinfo结构随后可由套接口函数直接使用。如此以来，getaddrinfo函数把协议相关性安全隐藏在这个库函数内部。应用程序只要处理由getaddrinfo函数填写的套接口地址结构。该函数在 POSIX规范中定义了。
函数原型
int getaddrinfo( const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result );

参数说明
hostname:一个主机名或者地址串(IPv4的点分十进制串或者IPv6的16进制串)
service：服务名可以是十进制的端口号，也可以是已定义的服务名称，如ftp、http等
hints：可以是一个空指针，也可以是一个指向某个addrinfo结构体的指针，调用者在这个结构中填入关于期望返回的信息类型的暗示。举例来说：如果指定的服务既支持TCP也支持UDP，那么调用者可以把hints结构中的ai_socktype成员设置成SOCK_DGRAM使得返回的仅仅是适用于数据报套接口的信息。
result：本函数通过result指针参数返回一个指向addrinfo结构体链表的指针。
返回值：0——成功，非0——出错
*/
        if (0 != getaddrinfo(node, port, &hints, &ai))
        {
            xerror("getaddrinfo(%s, %s): %s", node, port, strerror(errno));
        }

        for (const struct addrinfo *rp = ai; rp != NULL; rp = rp->ai_next)
        {
            fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (0 > fd)
            {
                fprintf(stderr, "%s: socket(): %s\n", argv0, strerror(errno));
                continue;
            }
            //连接成功就跳出循环
            if (!connect(fd, rp->ai_addr, ai->ai_addrlen))
            {
                break;
            }
            fprintf(stderr, "%s: connect failed: %s\n", argv0, strerror(errno));
            if (0 != close(fd))
            {
                fprintf(stderr, "%s: closing failed socket: %s\n", argv0, strerror(errno));
            }
            fd = -1;
        }
        if (0 > fd)
        {
            xerror("%s: Could not connect to %s %s\n", argv0, node, port);
        }
        freeaddrinfo(ai);
    }

    fprintf(stderr, "Connected to %s port %s\n", node, port);

    return handle(fd);
}
/* ---- Emacs Variables ----
 * Local Variables:
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vim: ts=8 sw=8
 */
