
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include "anet.h"

static void anetSetError(char *err, const char *fmt, ...)
{
    va_list ap;

    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, ANET_ERR_LEN, fmt, ap);
    va_end(ap);
}

int anetNonBlock(char *err, int fd)
{
    int flags;

    /* Set the socket non-blocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        anetSetError(err, "fcntl(F_GETFL): %s", strerror(errno));
        return ANET_ERR;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        anetSetError(err, "fcntl(F_SETFL,O_NONBLOCK): %s", strerror(errno));
        return ANET_ERR;
    }
    return ANET_OK;
}


/**
keepalive选项表示如果一个连接上7200s后没有任何数据发送，则设置了这个选项的本端向对端发送keepalive保活报文，它会有如下三种结果：

对端回复ACK。则本端TCP认为该连接依然存活。继续等7200s后再发送keepalive报文。
对端回复RESET。说明对端进程已经重启，本端的应用程序应该关闭该连接。
没有对端的任何回复。则本端做重试，如果重试9次（前后重试间隔为75秒）仍然不可达，则向应用程序返回错误信息，ETIMEOUT（无任何应答）或EHOST
*/
/* Set TCP keep alive option to detect dead peers. The interval option
 * is only used for Linux as we are using Linux-specific APIs to set
 * the probe send time, interval, and count. */
int anetKeepAlive(char *err, int fd, int interval)
{
    int val = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == -1)
    {
        anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
        return ANET_ERR;
    }

#ifdef __linux__
    /* Default settings are more or less garbage, with the keepalive time
     * set to 7200 by default on Linux. Modify settings to make the feature
     * actually useful. */

    /* Send first probe after interval. */
    val = interval;
    /**
TCP_KEEPIDLE
对一个连接进行有效性探测之前运行的最大非活跃时间间隔，默认值为 14400（即 2 个小时）
就是说这个连接俩小时还没有给我发送数据，我就像对短发送保活报文
    */
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
        anetSetError(err, "setsockopt TCP_KEEPIDLE: %s\n", strerror(errno));
        return ANET_ERR;
    }

    /* Send next probes after the specified interval. Note that we set the
     * delay as interval / 3, as we send three probes before detecting
     * an error (see the next setsockopt call). */
    val = interval/3;
    if (val == 0)
    {
        val = 1;
    }
    /**
TCP_KEEPINTVL
两个探测的时间间隔，默认值为 150 即 75 秒
    */
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
        anetSetError(err, "setsockopt TCP_KEEPINTVL: %s\n", strerror(errno));
        return ANET_ERR;
    }
    /**
TCP_KEEPCNT
关闭一个非活跃连接之前进行探测的最大次数，默认为 8 次
*/
    /* Consider the socket in error state after three we send three ACK
     * probes without getting a reply. */
    val = 3;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
        anetSetError(err, "setsockopt TCP_KEEPCNT: %s\n", strerror(errno));
        return ANET_ERR;
    }
#endif

    return ANET_OK;
}

static int anetSetTcpNoDelay(char *err, int fd, int val)
{
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == -1)
    {
        anetSetError(err, "setsockopt TCP_NODELAY: %s", strerror(errno));
        return ANET_ERR;
    }
    return ANET_OK;
}

int anetEnableTcpNoDelay(char *err, int fd)
{
    return anetSetTcpNoDelay(err, fd, 1);
}

int anetDisableTcpNoDelay(char *err, int fd) 
{
    return anetSetTcpNoDelay(err, fd, 0);
}


int anetSetSendBuffer(char *err, int fd, int buffsize)
{
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize)) == -1)
    {
        anetSetError(err, "setsockopt SO_SNDBUF: %s", strerror(errno));
        return ANET_ERR;
    }
    return ANET_OK;
}

int anetTcpKeepAlive(char *err, int fd)
{
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes)) == -1) {
        anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
        return ANET_ERR;
    }
    return ANET_OK;
}

int anetResolve(char *err, char *host, char *ipbuf)
{
    struct sockaddr_in sa;

    sa.sin_family = AF_INET;
    if (inet_aton(host, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(host);
        if (he == NULL) {
            anetSetError(err, "can't resolve: %s", host);
            return ANET_ERR;
        }
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    strcpy(ipbuf,inet_ntoa(sa.sin_addr));
    return ANET_OK;
}

static int anetCreateSocket(char *err, int domain) {
    int s, on = 1;
    if ((s = socket(domain, SOCK_STREAM, 0)) == -1) {
        anetSetError(err, "creating socket: %s", strerror(errno));
        return ANET_ERR;
    }

    /* Make sure connection-intensive things like the redis benchmark
     * will be able to close/open sockets a zillion of times */
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        close(s);
        anetSetError(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
        return ANET_ERR;
    }
    return s;
}

#define ANET_CONNECT_NONE 0
#define ANET_CONNECT_NONBLOCK 1
static int anetTcpGenericConnect(char *err, char *addr, int port, int flags)
{
    int s;
    struct sockaddr_in sa;

    if ((s = anetCreateSocket(err,AF_INET)) == ANET_ERR)
        return ANET_ERR;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    if (inet_aton(addr, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(addr);
        if (he == NULL) {
            anetSetError(err, "can't resolve: %s", addr);
            close(s);
            return ANET_ERR;
        }
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    if (flags & ANET_CONNECT_NONBLOCK) {
        if (anetNonBlock(err,s) != ANET_OK)
            return ANET_ERR;
    }
    if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        if (errno == EINPROGRESS &&
                flags & ANET_CONNECT_NONBLOCK)
            return s;

        anetSetError(err, "connect: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }
    return s;
}

int anetTcpConnect(char *err, char *addr, int port)
{
    return anetTcpGenericConnect(err,addr,port,ANET_CONNECT_NONE);
}

int anetTcpNonBlockConnect(char *err, char *addr, int port)
{
    return anetTcpGenericConnect(err,addr,port,ANET_CONNECT_NONBLOCK);
}

int anetUnixGenericConnect(char *err, char *path, int flags)
{
    int s;
    struct sockaddr_un sa;

    if ((s = anetCreateSocket(err,AF_LOCAL)) == ANET_ERR)
        return ANET_ERR;

    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
    if (flags & ANET_CONNECT_NONBLOCK) {
        if (anetNonBlock(err,s) != ANET_OK)
            return ANET_ERR;
    }
    if (connect(s,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
        if (errno == EINPROGRESS &&
                flags & ANET_CONNECT_NONBLOCK)
            return s;

        anetSetError(err, "connect: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }
    return s;
}

int anetUnixConnect(char *err, char *path)
{
    return anetUnixGenericConnect(err,path,ANET_CONNECT_NONE);
}

int anetUnixNonBlockConnect(char *err, char *path)
{
    return anetUnixGenericConnect(err,path,ANET_CONNECT_NONBLOCK);
}

/* Like read(2) but make sure 'count' is read before to return
 * (unless error or EOF condition is encountered) */
int anetRead(int fd, char *buf, int count)
{
    int nread, totlen = 0;
    while(totlen != count) {
        nread = read(fd,buf,count-totlen);
        if (nread == 0) return totlen;
        if (nread == -1) return -1;
        totlen += nread;
        buf += nread;
    }
    return totlen;
}

/* Like write(2) but make sure 'count' is read before to return
 * (unless error is encountered) */
int anetWrite(int fd, char *buf, int count)
{
    int nwritten, totlen = 0;
    while(totlen != count) {
        nwritten = write(fd,buf,count-totlen);
        if (nwritten == 0) return totlen;
        if (nwritten == -1) return -1;
        totlen += nwritten;
        buf += nwritten;
    }
    return totlen;
}

static int anetListen(char *err, int s, struct sockaddr *sa, socklen_t len) {
    if (bind(s,sa,len) == -1) {
        anetSetError(err, "bind: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }

    /* Use a backlog of 512 entries. We pass 511 to the listen() call because
     * the kernel does: backlogsize = roundup_pow_of_two(backlogsize + 1);
     * which will thus give us a backlog of 512 entries */
    if (listen(s, 511) == -1) {
        anetSetError(err, "listen: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }
    return ANET_OK;
}

int anetTcpServer(char *err, int port, char *bindaddr)
{
    int s;
    struct sockaddr_in sa;

    if ((s = anetCreateSocket(err,AF_INET)) == ANET_ERR)
        return ANET_ERR;

    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bindaddr && inet_aton(bindaddr, &sa.sin_addr) == 0) {
        anetSetError(err, "invalid bind address");
        close(s);
        return ANET_ERR;
    }
    if (anetListen(err,s,(struct sockaddr*)&sa,sizeof(sa)) == ANET_ERR)
        return ANET_ERR;
    return s;
}

int anetUnixServer(char *err, char *path, mode_t perm)
{
    int s;
    struct sockaddr_un sa;

    if ((s = anetCreateSocket(err,AF_LOCAL)) == ANET_ERR)
        return ANET_ERR;

    memset(&sa,0,sizeof(sa));
    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
    if (anetListen(err,s,(struct sockaddr*)&sa,sizeof(sa)) == ANET_ERR)
        return ANET_ERR;
    if (perm)
        chmod(sa.sun_path, perm);
    return s;
}

static int anetGenericAccept(char *err, int s, struct sockaddr *sa, socklen_t *len) {
    int fd;
    while(1) {
        fd = accept(s,sa,len);
        if (fd == -1) {
            if (errno == EINTR)
                continue;
            else {
                anetSetError(err, "accept: %s", strerror(errno));
                return ANET_ERR;
            }
        }
        break;
    }
    return fd;
}

int anetTcpAccept(char *err, int s, char *ip, int *port) {
    int fd;
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    if ((fd = anetGenericAccept(err,s,(struct sockaddr*)&sa,&salen)) == ANET_ERR)
        return ANET_ERR;

    if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
    if (port) *port = ntohs(sa.sin_port);
    return fd;
}

int anetUnixAccept(char *err, int s) {
    int fd;
    struct sockaddr_un sa;
    socklen_t salen = sizeof(sa);
    if ((fd = anetGenericAccept(err,s,(struct sockaddr*)&sa,&salen)) == ANET_ERR)
        return ANET_ERR;

    return fd;
}

int anetPeerToString(int fd, char *ip, int *port) {
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);

    if (getpeername(fd,(struct sockaddr*)&sa,&salen) == -1) {
        *port = 0;
        ip[0] = '?';
        ip[1] = '\0';
        return -1;
    }
    if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
    if (port) *port = ntohs(sa.sin_port);
    return 0;
}

int anetSockName(int fd, char *ip, int *port) {
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);

    if (getsockname(fd,(struct sockaddr*)&sa,&salen) == -1) {
        *port = 0;
        ip[0] = '?';
        ip[1] = '\0';
        return -1;
    }
    if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
    if (port) *port = ntohs(sa.sin_port);
    return 0;
}



/** added for UDP by lmy **/

static int anetCreateUdpSocket(char *err, const int domain) {
    int s, on = 1;
    if ((s = socket(domain, SOCK_DGRAM, 0)) == -1) {
        anetSetError(err, "creating socket: %s", strerror(errno));
        return ANET_ERR;
    }

    /* Make sure connection-intensive things like the redis benchmark
     * will be able to close/open sockets a zillion of times */
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        close(s);
        anetSetError(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
        return ANET_ERR;
    }
    return s;
}

#define MAX_SENDBUF_SIZE (256 * 1024 * 1024)
static int anetUdpGenericConnect(char *err, const char *addr, const int port)
{
    int s;

    if ((s = anetCreateUdpSocket(err, AF_INET)) == ANET_ERR)
        return ANET_ERR;

    /* maximize_sndbuf */
    socklen_t intsize = sizeof(int);
    int last_good = 0;
    int min, max, avg;
    int old_size;

    /* Start with the default size. */
    if (getsockopt(s, SOL_SOCKET, SO_SNDBUF, &old_size, &intsize) != 0) {
        close(s);
        anetSetError(err, "getsockopt(SO_SNDBUF)");
        return ANET_ERR;
    }

    /* Binary-search for the real maximum. */
    min = old_size;
    max = MAX_SENDBUF_SIZE;

    while (min <= max) {
        avg = ((unsigned int)(min + max)) / 2;
        if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (void *)&avg, intsize) == 0) {
            last_good = avg;
            min = avg + 1;
        } else {
            max = avg - 1;
        }
    }

    return s;
}

int anetUdpServer(char *err, int port, char *bindaddr)
{
    int s;
    struct sockaddr_in sa;

    if ((s = anetCreateUdpSocket(err,AF_INET)) == ANET_ERR)
        return ANET_ERR;

    /* maximize_sndbuf */
    socklen_t intsize = sizeof(int);
    int last_good = 0;
    int min, max, avg;
    int old_size;
    
    /* Start with the default size. */
    if (getsockopt(s, SOL_SOCKET, SO_SNDBUF, &old_size, &intsize) != 0) {
        close(s);
        anetSetError(err, "getsockopt(SO_SNDBUF)");
        return ANET_ERR;
    }

    /* Binary-search for the real maximum. */
    min = old_size;
    max = MAX_SENDBUF_SIZE;

    while (min <= max) {
        avg = ((unsigned int)(min + max)) / 2;
        if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (void *)&avg, intsize) == 0) {
            last_good = avg;
            min = avg + 1;
        } else {
            max = avg - 1;
        }
    }

    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bindaddr && inet_aton(bindaddr, &sa.sin_addr) == 0) {
        anetSetError(err, "invalid bind address");
        close(s);
        return ANET_ERR;
    }

    if (bind(s,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
        anetSetError(err, "bind: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }
    return s;
}

int anetUdpConnect(char *err, const char *addr, const int port)
{
    return anetUdpGenericConnect(err, addr, port);
}

int anetUdpRead(int fd, char *addr, int *port, char *buf, int count)
{
    struct sockaddr_in sa;
    socklen_t sa_size;

    int nread = 0;
    nread = recvfrom(fd,buf,count, 0,(struct sockaddr*)&sa, &sa_size);
    if (nread == 0) {
        return 0;
    } else if (nread == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            nread = 0;
        }else {
            return -1;
        }
    }else {
        strncpy(addr, inet_ntoa(sa.sin_addr), 15);
        addr[15] = '\0';
        *port = ntohs(sa.sin_port);
    }

    return nread;
}

/* Like write(2) but make sure 'count' is read before to return
 * (unless error is encountered) */
int anetUdpWrite(int fd, const char *addr, const int port, char *buf, const int count)
{
    struct sockaddr_in sa;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    if (inet_aton(addr, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(addr);
        if (he == NULL) {
            perror("can't resolve addr");
            return -1;
        }
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }

    int nwritten, totlen = 0;
    while(totlen != count) {
        nwritten = sendto(fd, buf, count-totlen, 0, (struct sockaddr*)&sa, sizeof(sa));
        if (nwritten == 0) {
            return 0;
        } else if (nwritten == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                nwritten = 0;
            }else {
                return -1;
            }
        }

        totlen += nwritten;
        buf += nwritten;
    }
    return totlen;
}

