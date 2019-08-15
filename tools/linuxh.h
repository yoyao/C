#ifndef LINUXH_H
#define LINUXH_H

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>	/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>	/* for nonblocking */
#include    <sys/ioctl.h>
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>	/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>	/* for Unix domain sockets */
#include    <linux/limits.h>
#include	<math.h>
#include 	<stdarg.h>
//#include    <openssl/md5.h>
#include    <sys/vfs.h>
#include    <net/if.h>
#include <semaphore.h>


#include    <sys/timeb.h>
#include    <vector>
#include    <string>
#include 	<list>
#include   <deque>
#include    <map>
#include <set>
#include <algorithm>
using namespace std;

#endif
