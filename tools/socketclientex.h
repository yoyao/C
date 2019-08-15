
#pragma once

#ifndef SOCKETCLIENTEX_H
#define SOCKETCLIENTEX_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux

#include <sys/fcntl.h>
#include <sys/errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <netinet/tcp.h>
#include <netdb.h>

#define INVALID_SOCKET -1
#define Errcode errno
#define close close

#define S_PROTO_TCP 0
#define SEX_EINPROGRESS  EINPROGRESS

#endif

#define _in_
#define _out_
#define _in_out_

class SocketClientEx
{
public:
    SocketClientEx();

    int Attach(int fd);
    /*
    *连接方法，带超时时间(微秒级),默认2秒超时
    */
    int Connect2(std::string &ip, unsigned short port, unsigned int timeout=2000000);
    /*
    *连接方法，带超时时间(微秒级),默认2秒超时
    */
    int Connect2(const char *ip, unsigned short port, unsigned int timeout=2000000);
    /*
    *断开连接
    */
    int Close();
    /*
    *接收信息，带超时时间(微秒级),默认2秒超时
    */
    int Receive(_out_ std::string *msg, unsigned int timeout=2000000);
    /*
    *接收信息,带超时时间(微秒级),默认2秒超时
    */
    int Receive(_out_ char* msg, unsigned int len, unsigned int timeout=2000000);
    /*
    *发送信息,带超时时间(微秒级),默认2秒超时
    */
    int Send(_in_ std::string &msg, unsigned int timeout=2000000);
    /*
    *发送信息,带超时时间(微秒级),默认2秒超时
    */
    int Send(const char* msg, unsigned int len, unsigned int timeout=2000000);
    /*
    *獲取對端的地址
    */
    std::string GetPeerAddress() const;
    /*
    *獲取對端的端口
    */
    uint16_t GetPeerPort() const;
    /*
    *獲取本地鏈接使用的端口
    */
    uint16_t GetConnPort() const;

    ~SocketClientEx();

private:
    void getuseport();
    void Reset();
    int MyConnectO(const char *host, int port, int *timeout, const char *name);
    int MyReadO(int sock, void *buf, ssize_t len, int *timeout, const char *name);
    int MyReadOS(int sock, void *buf, ssize_t len, int *timeout, const char *name,const char *stop);
    int MyWriteO(int sock, void *buf, ssize_t len, int *timeout, const char *name);

protected:
    std::string address;
    uint16_t peerport;
    uint16_t clientport;
    int handle;
    bool isconnected;


};


#endif // SOCKETCLIENTEX_H
