#include "include/socketservice.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/types.h>

SocketService::SocketService():ismd5(false)
{

}

SocketService::~SocketService()
{

}

int SocketService::Init()
{
    fd=socket(PF_INET,SOCK_STREAM,0);
    if(fd<0)
    {
        std::cout<<"socket init error"<<std::endl;
        return -1;
    }

    bzero(&this->sock_addr,sizeof(this->sock_addr));

    StringToSockaddr(&this->sock_addr,this->addr,this->port);

    if(ismd5)
    {
        //  setMd5Signature();
    }


    return 0;
}

void SocketService::SockaddrToString(const sockaddr_in *saddr, std::string &address,int &port)
{
    char buf[128]={0};
    inet_ntop(AF_INET,&saddr->sin_addr,buf,sizeof(saddr->sin_addr));
    address=buf;
    port=ntohs(saddr->sin_port);
}

void SocketService::StringToSockaddr(sockaddr_in *saddr,const std::string &address, int port)
{
    saddr->sin_family=AF_INET;
    saddr->sin_port=htons(port);
    inet_pton(AF_INET,address.c_str(),&saddr->sin_addr);
}

int SocketService::Bind()
{
    Init();
    return bind(this->fd,(struct sockaddr*)&sock_addr,sizeof(sock_addr));
}

int SocketService::Listen(int n)
{
    return listen(fd,n);
}

int SocketService::Accept()
{
    struct sockaddr_in client;
    socklen_t len;
    bzero(&client,sizeof(client));
    int ret=-1;
    ret=accept(this->fd,(struct sockaddr*)&client,&len);
    std::cout<<"client connect fd "<<ret<<std::endl;
    return ret;
}

int SocketService::ReuseAddress()
{
    int ret=0;
    int flag=1;
    ret=setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    return ret;
}

void SocketService::SetTcpMd5(bool isuse)
{
    this->ismd5=isuse;
}

std::string SocketService::getAddr() const
{
    return addr;
}

void SocketService::setAddr(const std::string &value)
{
    addr = value;

}

uint16_t SocketService::getPort() const
{
    return port;
}

void SocketService::setPort(const uint16_t &value)
{
    port = value;
}

int SocketService::getFd() const
{
    return fd;
}

int SocketService::setMd5Signature()
{
    int ret=0;
    ret=setsockopt(fd,IPPROTO_TCP,TCP_MD5SIG,&md5sig,TCP_MD5SIG_MAXKEYLEN);
    return ret;
}

std::string SocketService::getMd5Pwd() const
{
    return md5_pwd;
}

void SocketService::setMd5Pwd(const std::string &value)
{
    md5_pwd = value;
}
