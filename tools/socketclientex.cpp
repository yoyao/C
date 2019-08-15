#include "socketclientex.h"
#include <fcntl.h>

SocketClientEx::SocketClientEx() :peerport(0),clientport(0),isconnected(false)
{
    handle = INVALID_SOCKET;
}

int  SocketClientEx::Connect2(std::string &ip, unsigned short port, unsigned int timeout)
{

    return Connect2(ip.c_str(),port,timeout);
}
int  SocketClientEx::Connect2(const char *ip, unsigned short port, unsigned int timeout)
{
    int errc=0;
    errc=MyConnectO(ip,port,(int*)&timeout,NULL);
    if(errc !=-1)
    {
        this->handle=errc;
        isconnected=true;
    }
    return errc;
}

/*
*接收信息
*返回接收到的字节数
*/
int SocketClientEx::Receive(_out_ std::string *msg, unsigned int timeout)
{
    if (!isconnected || handle == INVALID_SOCKET)
    {
        return 0;
    }
    char buff[1024] = { 0 };
    int count = 0;

    while (1)
    {
        memset(buff, 0, sizeof(buff));
        int c = 0;
        c = Receive(buff, sizeof(buff), timeout);
        if (c > 0)
        {
            msg->append(buff,c);
            count += c;
            continue;
        }
        break;
    }

    return count;
}
/*
*接收信息
*返回接收到的字节数
*/
int SocketClientEx::Receive(_out_ char* msg, unsigned int len, unsigned int timeout)
{
    if (!isconnected || handle == INVALID_SOCKET)
    {
        return 0;
    }
    int count=0;
    count=MyReadO(this->handle,msg,len,(int*)&timeout,NULL);


    return count;
}
/*
*发送信息
*返回发送的字节数
*/
int SocketClientEx::Send(_in_ std::string &msg, unsigned int timeout)
{
    if (!isconnected || handle == INVALID_SOCKET)
    {
        return 0;
    }
    int count = 0;

    count=Send(msg.c_str(), msg.length(),timeout);

    return count;
}
/*
*发送信息
*返回发送的字节数
*/
int SocketClientEx::Send(const char* msg, unsigned int len, unsigned int timeout)
{
    if (!isconnected || handle == INVALID_SOCKET)
    {
        return 0;
    }

    //  char buff[1024] = { 0 };
    int count = 0;
    const char *name="";
    count =MyWriteO(this->handle,(char*)msg,len,(int*)&timeout,name);

    return count;

}


int SocketClientEx::Close()
{
    Reset();
    return 0;
}

/*
*獲取對端的地址
*/
std::string SocketClientEx::GetPeerAddress()const
{
    return this->address;
}

/*
*獲取對端的端口
*/
uint16_t SocketClientEx::GetPeerPort() const
{
    return this->peerport;
}
/*
*獲取本地鏈接使用的端口
*/
uint16_t SocketClientEx::GetConnPort() const
{
    return this->clientport;
}

void SocketClientEx::getuseport()
{
    struct sockaddr_in local;
    memset(&local,0,sizeof(local));
    socklen_t slen=sizeof(local);
    int ret= getsockname(this->handle,reinterpret_cast<struct sockaddr*>(&local),&slen);
    if(ret==0)
    {
        this->clientport=ntohs(local.sin_port);
    }

}
//通过已经建立连接的socket初始化一个客户端
int SocketClientEx::Attach(int fd)
{
    struct sockaddr_in local;
    memset(&local,0,sizeof(local));
    socklen_t slen=sizeof(local);
    int ret= getpeername(fd,reinterpret_cast<struct sockaddr*>(&local),&slen);
    if(ret!=0)
    {
        return -1;
    }
    Reset();
    char ipaddr[20]={0};
    inet_ntop(AF_INET,&local.sin_addr.s_addr,ipaddr,sizeof(ipaddr));
    this->address=ipaddr;//服务端的地址
    this->peerport=ntohs(local.sin_port);//服务端的端口
    this->isconnected=true;
    this->handle=fd;

    getuseport();//获取客户端使用的端口

    return 0;
}

void SocketClientEx::Reset()
{
    if (handle != INVALID_SOCKET)
    {
        close(handle);
    }

    this->address.clear();
    this->peerport=0;
    this->clientport=0;
    this->handle=INVALID_SOCKET;
    this->isconnected=false;
}

int SocketClientEx::MyConnectO(const char *host, int port, int *timeout, const char *name)
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
    ret = connect(sock, (struct sockaddr *) &sin, sizeof(sin));
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
int SocketClientEx::MyReadO(int sock, void *buf, ssize_t len, int *timeout, const char *name)
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
int SocketClientEx::MyReadOS(int sock, void *buf, ssize_t len, int *timeout, const char *name,const char *stop)
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
int SocketClientEx::MyWriteO(int sock, void *buf, ssize_t len, int *timeout, const char *name)
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


SocketClientEx::~SocketClientEx()
{
    Reset();
}
