#include "include/socketclientex.h"

SocketClientEx::SocketClientEx() :handle(SEX_INVALID_SOCKET),isconnected(false)
{

#ifdef WIN32

    WORD sversion = MAKEWORD(2, 2);
    WSADATA data;
    WSAStartup(sversion, &data);

#endif

    handle = SEX_INVALID_SOCKET;
}

int SocketClientEx::Init()
{
    this->handle = socket(PF_INET, SOCK_STREAM, S_PROTO_TCP);
    if (SEX_INVALID_SOCKET == this->handle)
    {
        std::cout << "create socket error" << std::endl;
        return -1;
    }

    bzero(&this->sock_addr,sizeof(this->sock_addr));
    StringToSockaddr(&this->sock_addr,this->address,this->port);
    //set nonblock
    SetBlock(false);

    return 0;
}
//反向初始化 给一个网络描述符
int SocketClientEx::ReverseInit(int fd)
{
   this->handle=fd;
   socklen_t len;
   if(getpeername(fd,(struct sockaddr*)&this->sock_addr,&len)==0)
   {
      SockaddrToString(&this->sock_addr,this->address,this->port);
      this->isconnected =true;
   }
   return 0;
}

SEX_ERR_TYPE  SocketClientEx::Connect2(std::string &ip, unsigned short port, unsigned int timeout)
{
    SEX_ERR_TYPE errc=SEX_ERROR;

    if (SEX_INVALID_SOCKET == handle)
    {
        int ret= Init();
        if(ret !=0)
        {
            return errc;
        }
    }

    int ret;
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv.sin_addr);

    errc = SEX_NONE_ERR;

    do
    {
        //设置为非阻塞模式
        // SetBlock(false);
        ret = connect(handle, (struct sockaddr*)&serv, sizeof(serv));
        if(ret ==0)
        {
            break;
        }
        if (ret == -1 && errno == SEX_EINPROGRESS)
        {
            struct timeval timer;
            timer.tv_sec = timeout;
            timer.tv_usec = 0;

            FD_ZERO(&fs);
            FD_SET(handle, &fs);
            /*
               select会阻塞直到检测到事件或则超时，如果超时，select会返回0，
               如果检测到事件会返回1，如果异常会返回-1，如果是由于信号中断引起的异常errno==EINTR
               */
            ret = select(handle+1, NULL, &fs, NULL, &timer);
            if (ret == 0)
            {
                errc = SEX_TIME_OUT;
                break;
            }

        }
        else if (ret == -1)
        {
            errc = SEX_ERROR;
            break;
        }

    } while (0);

    if(errc==SEX_NONE_ERR)
    {
        isconnected = true;
        this->address=ip;
        this->port=port;
        this->sock_addr=serv;
    }
    //设置阻塞模式
    //SetBlock(true);

    return errc;
}

SEX_ERR_TYPE SocketClientEx::Connect(int timeout)
{
    return Connect2(this->address,this->port,timeout);
}

/*
 *接收信息
 *返回接收到的字节数
 */
int SocketClientEx::Receive(std::string &msg, unsigned int timeout)
{
    if (!isconnected || handle == SEX_INVALID_SOCKET)
    {
        return 0;
    }

    char buff[1024] = { 0 };
    int count = 0;
    FD_ZERO(&fs);
    FD_SET(handle, &fs);

    while (1)
    {
        memset(buff, 0, sizeof(buff));
        size_t c = 0;
        c = Receive(buff, sizeof(buff), timeout);
        if (c > 0)
        {
            msg.append(buff, c);
            count += c;
            //小于就是读到了最后几个字节
            if(c<sizeof(buff))
            {
                break;
            }
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
int SocketClientEx::Receive(char* msg, unsigned int len, unsigned int timeout)
{
    if (!isconnected || handle == SEX_INVALID_SOCKET)
    {
        return 0;
    }

    int count = 0;
    FD_ZERO(&fs);
    FD_SET(handle, &fs);

    struct timeval timer;
    timer.tv_sec = timeout;
    timer.tv_usec = 0;

    int c = select(handle+1, &fs, NULL, NULL, &timer);
    if (c > 0)
    {
        count = recv(handle, msg, len, 0);
        std::cout<<"recv end count "<<count<<std::endl;
    }
    std::cout<<"c is "<<c<<" recv end\n";

    return count;
}
/*
 *发送信息
 *返回发送的字节数
 */
int SocketClientEx::Send(std::string &msg, unsigned int timeout)
{
    if (!isconnected || handle == SEX_INVALID_SOCKET)
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
    if (!isconnected || handle == SEX_INVALID_SOCKET)
    {
        return 0;
    }

    int count = 0;
    FD_ZERO(&fs);
    FD_SET(handle, &fs);

    struct timeval timer;
    timer.tv_sec = timeout;
    timer.tv_usec = 0;

    //非阻塞
    // this->SetBlock(false);
    count = send(handle, msg, len, 0);
    if(count<0)
    {
        int c = select(1, NULL, &fs, NULL, &timer);
        if (c > 0)
        {
            count = send(handle, msg, len, 0);
        }
    }
    //阻塞
    //  SetBlock(true);
    return count;

}

int SocketClientEx::ReuseAddr()
{
    if (handle != SEX_INVALID_SOCKET)
    {
        int val=1;
        return setsockopt(handle,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
    }
    return 0;
}


SEX_ERR_TYPE SocketClientEx::DisConnect()
{
    if (handle != SEX_INVALID_SOCKET)
    {
        close(handle);
    }
    return SEX_NONE_ERR;
}

SocketClientEx::~SocketClientEx()
{
    if (handle != SEX_INVALID_SOCKET)
    {
        close(handle);
    }
#ifdef WIN32
    WSACleanup();
#endif
}

void SocketClientEx::SockaddrToString(const sockaddr_in *saddr, std::string &address,int &port)
{
    char buf[128]={0};
    inet_ntop(AF_INET,&saddr->sin_addr,buf,sizeof(buf));
    std::cout<<buf<<std::endl;
    address=buf;
    port=ntohs(saddr->sin_port);
}

void SocketClientEx::StringToSockaddr(sockaddr_in *saddr,const std::string &address, int port)
{
    saddr->sin_family=AF_INET;
    saddr->sin_port=htons(port);
    inet_pton(AF_INET,address.c_str(),&saddr->sin_addr);

}

void SocketClientEx::SetBlock(bool b)
{
    int flag=fcntl(handle,F_GETFL);
    if(!b)
    {
        //设置为非阻塞模式
        flag |=O_NONBLOCK;
        fcntl(handle,F_SETFL,flag);
    }
    else
    {
        //设置阻塞模式
        flag &= ~O_NONBLOCK;
        fcntl(handle,F_SETFL,flag);
    }
}

SOCKETex_t SocketClientEx::getHandle() const
{
    return handle;
}

void SocketClientEx::setHandle(const SOCKETex_t &value)
{
    handle = value;
}

std::string SocketClientEx::getAddress() const
{
    return address;
}

void SocketClientEx::setAddress(const std::string &value)
{
    address = value;
}

int SocketClientEx::getPort() const
{
    return port;
}

void SocketClientEx::setPort(int value)
{
    port = value;
}
