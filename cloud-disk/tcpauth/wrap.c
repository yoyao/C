/*
 * Wrap a server in MD5SIG. The TCP socket connected to the client is
 * passed on to the binary in fd 0 and 1.
 *
 * Example:
 *   echo "correct horse battery staple" > pw.txt
 *   chmod 600 pw.txt
 *   sudo ./tcpauth-wrap -p 12345 -P pw.txt -- /usr/sbin/sshd -i
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
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>

#include"common.h"

const char* next_binary = NULL;
char* const* next_args = NULL;
const char* default_opt_H = "::";
char* password = NULL;

void usage(int err)
{
    printf("Usage: %s [options] -- /path/to/binary [options to binary...]\n"
           "    -h                   Show this usage text.\n"
           "    -H <addr>            Address to listen to (default: \"%s\").\n"
           "    -p <port>            Port to listen on.\n"
           "    -P <password file>   File containing MD5SIG password.\n"
           "", argv0, default_opt_H);
    exit(err);
}

// Handle a new connection that's come in:
// * Enable MD5SIG
// * Exec next binary.

void handle(int fd)
{
    if (1)
    {
        struct tcp_md5sig md5sig;
        /*
在 IPv4 应用中，使用的通用地址结构为 struct sockaddr, 定义在 <sys/socket.h> 中，
在 IPv6 中，提出了一种新的通用地址结构： struct sockaddr_storage 定义在<netinet/in.h>中
*/
        socklen_t t = sizeof(struct sockaddr_storage);

        memset(&md5sig, 0, sizeof(md5sig));
        strncpy((char*)md5sig.tcpm_key, password, TCP_MD5SIG_MAXKEYLEN);
        //获取对端IP 并且复制到MD5签名的地址内
        if (getpeername(fd,(struct sockaddr*)&md5sig.tcpm_addr, &t))
        {
            xerror("getpeername(): %.100s", strerror(errno));
        }
        md5sig.tcpm_keylen = strlen((char*)md5sig.tcpm_key);
        if (-1 == setsockopt(fd,IPPROTO_TCP, TCP_MD5SIG,&md5sig, sizeof(md5sig)))
        {
            xerror("setsockopt(TCP_MD5SIG): %.100s", strerror(errno));
        }
    }
    /*
n_fd = dup2(fd3,STDOUT_FILENO）表示n_fd与fd3共享一个文件表项（它们的文件表指针指向同一个文件表项），n_fd在文件描述符表中的位置为 STDOUT_FILENO的位置，而原先的STDOUT_FILENO所指向的文件表项被关闭
*/
    //fd指向了标准输入  或者 指向标准输出
    if (0 > dup2(fd, STDIN_FILENO) || 0 > dup2(fd, STDOUT_FILENO))
    {
        xerror("dup2(): %s", strerror(errno));
    }
    //标准输入或标准输出被关闭
    close(fd);
    //当前进程被子进程替代
    execvp(next_binary, next_args);

    xerror("execv(): %s", strerror(errno));
}

// Main loop listening for connections and calling handle() on them.
int main_loop(int fd)
{
    for (;;)
    {
        int newsock;
        struct sockaddr_storage from;
        socklen_t fromlen;

        fromlen = sizeof(from);
        newsock = accept(fd, (struct sockaddr*)&from, &fromlen);
        if (0 > newsock)
        {
            fprintf(stderr, "%s: accept(): %s\n", argv0, strerror(errno));
            continue;
        }

        pid_t pid;
        switch (pid = fork())
        {
        case -1:
        {
            fprintf(stderr, "%s: fork(): %s\n", argv0, strerror(errno));
            close(newsock);
            continue;
        }
        case 0:
        {
            handle(newsock);
            exit(1);
        }
        default:
        {
            close(newsock);
            break;
        }

        }
    }
}

int main(int argc, char** argv)
{
    const char* port = NULL;  // -p <port>
    const char* node = default_opt_H;  // -H <addr>
    const char* password_file = NULL;  // -P <password file>

    argv0 = argv[0];

    int c;
    while (EOF != (c = getopt(argc, argv, "hH:p:P:")))
    {
        switch (c) {
        case 'h':
            usage(0);
        case 'H':
            node = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'P':
            password_file = optarg;
            break;
        default:
            usage(1);
        }
    }

    if (port == NULL)
    {
        fprintf(stderr, "%s: Need to specify port (-p)\n", argv0);
        exit(1);
    }

    {
        //通过给定的文件,获取文本中存储的密码字符串
        password = get_password(password_file);
        //
        next_binary = argv[optind];
        next_args = &argv[optind];
    }

    // Create socket and bind.
    int fd = -1;
    {
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        struct addrinfo *ai;
        /*
定义函数:
int getaddrinfo( const char *hostname, const char *service, const struct addrinfo *hints,
struct addrinfo **result );

函数说明:
      getaddrinfo函数能够处理名字到地址以及服务到端口这两种转换，返回的是一个sockaddr 结构的链而 不是一个地址清单。它具有协议无关性。
      hostname:一个主机名或者地址串(IPv4的点分十进制串或者IPv6的16进制串)
      service：一个服务名或者10进制端口号数串。
       hints：可以是一个空指针，也可以是一个指向某个addrinfo结构的指针，调用者在这个结构中填入关于期望返回的信息类型的暗示。举例来说：如果指定的服务既支持TCP也支持UDP，那么调用者可以把hints结构中的ai_socktype成员设置成SOCK_DGRAM使得返回的仅仅是适用于数据报套接口的信息。返回0： 成功，返回非0： 出错。
*/
        if (0 != getaddrinfo(node, port, &hints, &ai))
        {
            fprintf(stderr, "%s: getaddrinfo(%s, %s): %s\n", argv0, node, port, strerror(errno));
            exit(1);
        }

        for (const struct addrinfo *rp = ai; rp != NULL; rp = rp->ai_next)
        {
            fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (0 > fd)
            {
                fprintf(stderr, "%s: socket(): %s\n", argv0, strerror(errno));
                continue;
            }
            const int on = 1;
            if (0 > setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
            {
                fprintf(stderr, "%s: setsockopt(SO_REUSEADDR): %s\n", argv0, strerror(errno));
            }
            if (!bind(fd, rp->ai_addr, ai->ai_addrlen))
            {
                break;
            }
            if (0 != close(fd))
            {
                fprintf(stderr, "%s: closing failed socket: %s\n", argv0, strerror(errno));
            }
            fd = -1;
        }
        if (0 > fd)
        {
            xerror("Could not bind to \"%s\" port \"%s\"", node, port);
        }
        freeaddrinfo(ai);

        if (listen(fd, 5))
        {
            xerror("listen(): %s", strerror(errno));
        }
    }

    {
        fprintf(stderr, "Listening on %s port %s\n", node, port);
        fprintf(stderr, "Next binary: %s\n", next_binary);
        fprintf(stderr, "Args:\n");
        int c;
        for (c = 0; next_args[c]; c++)
        {
            fprintf(stderr, "  %s\n", next_args[c]);
        }
    }
    return main_loop(fd);
}
/* ---- Emacs Variables ----
 * Local Variables:
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vim: ts=8 sw=8
 */
