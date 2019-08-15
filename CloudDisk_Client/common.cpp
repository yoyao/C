#include "common.h"

QNetworkAccessManager* Common::m_netManager = new QNetworkAccessManager;

Common::Common()
{

}

QNetworkAccessManager *Common::GetNetManager()
{
    // 该对象一般一个应用程序中有一个就够了，无需new多个
    return  m_netManager;
}

void Common::CreateHttpRequest(QByteArray &result,QByteArray &data,const char* operation,const char *method="POST")
{
    result=QByteArray(method)
            .append(" /")
            .append(operation)
            .append(" HTTP/1.1\r\n")
            .append("Host: 192.168.82.25\r\n")
            .append("Content-Type: application/json;charset=utf-8\r\n")
            .append("Connection: keep-alive\r\n")
            .append("Upgrade-Insecure-Requests: 1\r\n")
            .append("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36\r\n")
            .append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n")
            .append("Accept-Encoding: gzip, deflate\r\n")
            .append("Content-Length: ")
            .append(QString::number(data.length()))
            .append("\r\n")
            .append("Accept-Language: zh-CN,en,zh;q=0.8,zh-TW;q=0.6\r\n\r\n")
            .append(data);
}
