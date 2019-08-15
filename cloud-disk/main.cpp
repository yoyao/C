#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <boost/asio.hpp>
#include "model/UserInfo.pb.h"
#include "include/socketclientex.h"
#include "include/socketservice.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc<3)
    {
        std::cout<<"input addr&port"<<std::endl;
        return 0;
    }
    std::string ip;
    ip=argv[1];
    int port,ret;

    port=atoi(argv[2]);

    /*
    SEX_ERR_TYPE err=SEX_ERROR;
    SocketClientEx client;
    client.setAddress(ip);
    client.setPort(port);
    ret=client.Init();
    if(ret !=0)
    {
        std::cout<<"socket init failed\n";
        return 0;
    }
    client.ReuseAddr();

    err=client.Connect(3);
    if(err==SEX_TIME_OUT)
    {
        std::cout<<"connect to server time out err "<<err<<std::endl;
        return 0;
    }
    std::string msg("yes it is\n");
    ret= client.Send(msg,3);

    std::cout <<"send byte count "<<ret<<std::endl;

    std::string mesg;
    client.Receive(mesg,5);

    std::cout<<"recv msg:"<<mesg<<std::endl;

    client.DisConnect();
*/

   boost::asio::io_service ioserv;
   boost::asio::ip::tcp::socket handle;

   boost::asio::ip::tcp::endpoint ip;

   ioserv.run();
    SocketService service;
    SocketClientEx client;

    service.setAddr(ip);
    service.setPort(port);
    ret=service.Bind();
    service.Listen(SOMAXCONN);

    int fd= service.Accept();
    client.ReverseInit(fd);

    std::cout<<client.getAddress()<<":"<<client.getPort()<<std::endl;
    client.DisConnect();
    return 0;
}
