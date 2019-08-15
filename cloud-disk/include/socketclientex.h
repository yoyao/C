#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <iostream>
#include <string>

#ifdef WIN32

#include <WS2tcpip.h>
#include <winsock2.h>

typedef SOCKET SOCKETex_t;

#define SEX_INVALID_SOCKET INVALID_SOCKET
#define ioctl ioctlsocket
#define Errcode GetLastError()
#define close closesocket
#define S_PROTO_TCP IPPROTO_TCP
#define SEX_EINPROGRESS  WSAEWOULDBLOCK
#define SEX_FIONBIO  FIONBIO
#define ioctl ioctl
#endif

#ifdef linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef int SOCKETex_t;

#define SEX_INVALID_SOCKET -1
#define Errcode errno
#define close close
#define ioctl fcntl
#define S_PROTO_TCP 0
#define SEX_EINPROGRESS  EINPROGRESS
#define SEX_FIONBIO  O_NONBLOCK

#endif

enum SEX_ERR_TYPE
{
    SEX_NONE_ERR=0,
    SEX_TIME_OUT = 1,
    SEX_ERROR=2
};

class SocketClientEx
{

    public:
        SocketClientEx();

        int Init();
        int ReverseInit(int fd);
        /*
         *     *连接方法，带超时时间
         *         */
        SEX_ERR_TYPE Connect2(std::string &ip, unsigned short port, unsigned int timeout);
        /*
         *     连接方法
         *         */
        SEX_ERR_TYPE Connect(int timeout);
        /*
         *     *断开连接
         *         */
        SEX_ERR_TYPE DisConnect();
        /*
         *     *接收信息
         *         */
        int Receive(std::string &msg, unsigned int timeout);
        /*
         *     *接收信息
         *         */
        int Receive(char* msg, unsigned int len, unsigned int timeout);
        /*
         *     *发送信息
         *         */
        int Send(std::string &msg, unsigned int timeout);
        /*
         *     *发送信息
         *         */
        int Send(const char* msg, unsigned int len, unsigned int timeout);
        /*
         *     *发送信息
         *         */
        int ReuseAddr();


        ~SocketClientEx();

        SOCKETex_t getHandle() const;
        void setHandle(const SOCKETex_t &value);

        std::string getAddress() const;
        void setAddress(const std::string &value);

        int getPort() const;
        void setPort(int value);

    private:
        void SetBlock(bool b);
        void SockaddrToString(const sockaddr_in *saddr, std::string &address, int &port);
        void StringToSockaddr(sockaddr_in *saddr, const std::string &address, int port);

    private:
        SOCKETex_t handle;
        fd_set fs;
        bool isconnected;
        std::string address;
        int port;
        struct sockaddr_in sock_addr;
        struct tcp_md5sig md5sig;
        bool ismd5;
        std::string md5_pwd;

    protected:

};

