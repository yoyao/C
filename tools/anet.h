

#ifndef ANET_H
#define ANET_H

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256

#if defined(__sun)
#define AF_LOCAL AF_UNIX
#endif

int anetTcpConnect(char *err, char *addr, int port);
int anetTcpNonBlockConnect(char *err, char *addr, int port);
int anetUnixConnect(char *err, char *path);
int anetUnixNonBlockConnect(char *err, char *path);
int anetRead(int fd, char *buf, int count);
int anetResolve(char *err, char *host, char *ipbuf);
int anetTcpServer(char *err, int port, char *bindaddr);
int anetUnixServer(char *err, char *path, mode_t perm);
int anetTcpAccept(char *err, int serversock, char *ip, int *port);
int anetUnixAccept(char *err, int serversock);
int anetWrite(int fd, char *buf, int count);
int anetNonBlock(char *err, int fd);
int anetEnableTcpNoDelay(char *err, int fd);
int anetDisableTcpNoDelay(char *err, int fd);
int anetTcpKeepAlive(char *err, int fd);
int anetPeerToString(int fd, char *ip, int *port);
int anetKeepAlive(char *err, int fd, int interval);

/** added for UDP by lmy  **/
int anetUdpServer(char *err, int port, char *bindaddr);
int anetUdpConnect(char *err, const char *addr, const int port);
int anetUdpWrite(int fd, const char *addr, const int port, char *buf, int count);
int anetUdpRead(int fd, char *addr, int *port, char *buf, int count);
#endif
