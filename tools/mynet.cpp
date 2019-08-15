/*
 * $Id: mynet.cpp,v 1.7 2007/03/14 07:30:30 lihh Exp $
 * Description: mynet modules for appui
 */

#include "mynet.h"
#include "linuxh.h"
/*
 * wrapper for socket read and write
 * multiread/multiwrite is allowd here
 */
static int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
{
    assert(serv_addr != NULL);

    return connect(sockfd, serv_addr, addrlen);
}

/*
static ssize_t Read(int fd, void *ptr1, size_t nbytes)
{
    ssize_t nread;
    ssize_t nleft = nbytes;
    char    *ptr=(char*)ptr1;

    assert(ptr1 != NULL);

    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if(nread < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        else if (nread == 0)
            break;
        ptr += nread;
        nleft -= nread;
    }

    // return length of data read from socket
    return (nbytes - nleft);
}

static ssize_t Write(int fd, void *ptr1, size_t len)
{
    ssize_t res = 0;
    char    *buf = (char*)ptr1;

    assert(ptr1 != NULL);

    while (len > 0)
    {
        do {
            res = write (fd, buf, len);
        } while (res < 0 && errno == EINTR);
        if (res <= 0)
            break;
        buf += res;
        len -= res;
    }

    return res;
}
*/

int Close(int fd)
{
    return close(fd);
}

/*
 * net functions for use
 */
/* connect to a remote host with timeout control(us) */
int MyConnectO(const char *host, int port, int *timeout, const char *name)
{
    char buf[1024];
    fd_set rset, wset;
    struct hostent he, *p;
    struct sockaddr_in sin;
    socklen_t len = 0;
    int sock = -1, ret = 0, err = 0, flags = 0, on = 1;
    struct timeval t;

    assert(host != NULL);

    if (*timeout < 0)
        *timeout = 0;
    int old_timeout = *timeout;

    // invalid server
    if (host[0] == 0 || port == 0) {
        return -1;
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {


        return -1;
    }

    bzero(&sin, sizeof(sin));
    sin.sin_addr.s_addr = inet_addr(host);
    if ((sin.sin_addr.s_addr) == INADDR_NONE) {	/* host is not numbers-and-dots ip address */
        ret = gethostbyname_r(host, &he, buf, sizeof(buf), &p, &err);
        if (ret < 0) {


            close(sock);
            return -1;
        }
        memcpy(&sin.sin_addr.s_addr, he.h_addr, sizeof(sin.sin_addr.s_addr));
    }
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
    ret = Connect(sock, (struct sockaddr *) &sin, sizeof(sin));
    if (ret == 0) {
        fcntl(sock, F_SETFL, flags);

        return sock;
    } else {
        if (errno != EINPROGRESS) {	/* cannot connect to the host */

            close(sock);
            return -1;
        }

        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        wset = rset;
        t.tv_sec = *timeout / 1000000, t.tv_usec = *timeout % 1000000;
        ret = select(sock + 1, &rset, &wset, NULL, &t);
        if (t.tv_sec * 1000000 + t.tv_usec < 10)
            t.tv_sec = 0, t.tv_usec = 0;
        *timeout = t.tv_sec * 1000000 + t.tv_usec;
        if (ret == 0) {

            close(sock);
            return -1;
        }

        if (FD_ISSET(sock, &rset) || FD_ISSET(sock, &wset)) {
            len = sizeof(err);
            ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
            if (ret < 0) {
                if (err)
                    errno = err;

                close(sock);
                return -1;
            }
            if (err) {
                errno = err;

                close(sock);
                return -1;
            }
            fcntl(sock, F_SETFL, flags);

            return sock;
        } else {

            close(sock);
            return -1;
        }
    }

    return -1;
}

/* read data from a socket with timeout control(us) */
int MyReadO(int sock, void *buf, ssize_t len, int *timeout, const char *name)
{
    int ret;
    struct timeval t;
    fd_set rset;
    ssize_t nleft, nread;
    char *ptr;

    if (*timeout < 0)
        *timeout = 0;
    int old_timeout = *timeout;

    // invalid sock
    if (sock < 0) {

        return -1;
    }

    nleft = len, nread = 0, ptr = (char *) buf;
    t.tv_sec = *timeout / 1000000, t.tv_usec = *timeout % 1000000;

    while (nleft > 0)
    {
        // select socket
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        ret = select(sock + 1, &rset, NULL, NULL, &t);
        if (t.tv_sec * 1000000 + t.tv_usec < 10)
            t.tv_sec = 0, t.tv_usec = 0;
        if (ret <= 0)
        {
            if (ret != 0)
            {

            }
            else
            {
                break;
            }
        }
        // read data
        nread = read(sock, ptr, nleft);
        if (nread < 0) {
            if (errno == EINTR)
            {	//系统中断处理
                continue;
            }

            break;
        } else if (nread == 0)
        {
            break;
        }
        ptr += nread;
        nleft -= nread;
    }
    *timeout = t.tv_sec * 1000000 + t.tv_usec;

    /* return the length of data read from server */


    return (len - nleft);
}

/* read data from a socket with timeout control(us) */
int MyReadOS(int sock, void *buf, ssize_t len, int *timeout, const char *name,const char *stop)
{
    int ret;
    struct timeval t;
    fd_set rset;
    ssize_t nleft, nread;
    char *ptr;

    if (*timeout < 0)
        *timeout = 0;
    int old_timeout = *timeout;

    // invalid sock
    if (sock < 0)
    {
        return -1;
    }

    nleft = len, nread = 0, ptr = (char *) buf;
    t.tv_sec = *timeout / 1000000, t.tv_usec = *timeout % 1000000;

    while (nleft > 0)
    {
        // select socket
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        ret = select(sock + 1, &rset, NULL, NULL, &t);
        if (t.tv_sec * 1000000 + t.tv_usec < 10)
        {
            t.tv_sec = 0;
            t.tv_usec = 0;
        }
        if (ret <= 0)
        {
            if (ret != 0)
            {

            }
            else
            {
                break;
            }
        }
        // read data
        nread = read(sock, ptr, nleft);
        if (nread < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            break;
        }
        else if (nread == 0)
        {

            break;
        }
        ptr += nread;
        nleft -= nread;

        // check whether there is need to stop reading
        if (stop != NULL && strstr((char *) buf, stop) != NULL)
        {
            break;
        }
    }
    *timeout = t.tv_sec * 1000000 + t.tv_usec;

    /* return the length of data read from server */


    return (len - nleft);
}

/* write data to a socket with timeout control(us) */
int MyWriteO(int sock, void *buf, ssize_t len, int *timeout, const char *name)
{
    int ret;
    struct timeval t;
    fd_set wset;
    ssize_t nleft, nwrite;
    char *ptr;


    if (*timeout < 0)
        *timeout = 0;
    int old_timeout = *timeout;

    // invalid sock
    if (sock < 0) {

        return -1;
    }

    nleft = len, nwrite = 0, ptr = (char *) buf;
    t.tv_sec = *timeout / 1000000, t.tv_usec = *timeout % 1000000;

    while (nleft > 0) {
        // select socket
        FD_ZERO(&wset);
        FD_SET(sock, &wset);
        ret = select(sock + 1, NULL, &wset, NULL, &t);
        if (t.tv_sec * 1000000 + t.tv_usec < 10)
            t.tv_sec = 0, t.tv_usec = 0;
        if (ret <= 0) {
            if (ret != 0)
                ;
            else
                ;
            return -1;
        }
        // write data
        nwrite = write(sock, ptr, nleft);
        if (nwrite < 0) {
            if (errno == EINTR) {	//系统中断处理
                continue;
            }

            break;
        } else if (nwrite == 0) {

            break;
        }
        ptr += nwrite;
        nleft -= nwrite;
    }
    *timeout = t.tv_sec * 1000000 + t.tv_usec;

    /* return the length of data read from server */
    
    return (len - nleft);
}

/* close the socket */
int
MyClose(int fd)
{

    return Close(fd);
}

/* open tcp listen port */
int MyTcpListen(int port, int queue)
{
    int listenfd;
    const int on = 1;
    struct sockaddr_in sin;

    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {

        return -1;
    }

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        close(listenfd);

        return -1;
    }

    (queue <= 0) ? queue = 5 : queue;
    if (listen(listenfd, queue) < 0) {
        close(listenfd);

        return -1;
    }

    return listenfd;
}

/*
 * related functions for use
 */
/* vim: set ts=8 sw=4 sts=4 tw=78 noet: */
