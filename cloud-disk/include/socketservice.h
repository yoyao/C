#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <errno.h>

#include "include/socketclientex.h"

class SocketService
{
public:
    SocketService();
    ~SocketService();

    int Bind();
    int Listen(int n);
    int Accept();
    int ReuseAddress();
    void SetTcpMd5(bool isuse);
    std::string getAddr() const;
    void setAddr(const std::string &value);

    uint16_t getPort() const;
    void setPort(const uint16_t &value);

    int getFd() const;

    std::string getMd5Pwd() const;
    void setMd5Pwd(const std::string &value);

private:
    int setMd5Signature();
    int Init();
    void SockaddrToString(const sockaddr_in *saddr, std::string &address, int &port);
    void StringToSockaddr(sockaddr_in *saddr, const std::string &address, int port);
private:

    struct sockaddr_in sock_addr;
    struct tcp_md5sig md5sig;
    bool ismd5;
    std::string md5_pwd;
    int fd;
    std::string addr;
    uint16_t port;
};




#endif // SOCKETSERVICE_H
