#include "httpresponse.h"

HttpResponse::HttpResponse()
{

}

HttpResponse HttpResponse::FromByteArray(QByteArray &format)
{
    /*
HTTP/1.1 200 OK\r\nServer: nginx/1.10.1\r\nDate: Sat, 29 Jul 2017 00:41:44 GMT\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: keep-alive\r\n\r\n27\r\n{\"status\":201,\"error\":201,\"msg\":\"succ\"}\r\n0\r\n\r\n
*/
    QString string(format),respline,data;
    HttpResponse resp;

    int index=-1,status;
    index=string.indexOf("\r\n");
    if(index<0)
    {
        return resp;
    }
    respline=string.left(index);

    status=respline.split(' ').at(1).toInt();

    index=string.indexOf("\r\n\r\n");
    if(index<0)
    {
        return resp;
    }
    data=string.right(string.length()-index-4);
    index=data.indexOf("\r\n");
    data=data.right(data.length()-index-2);
    index=data.indexOf("\r\n");
    data=data.left(index);

    resp.setData(data);
    resp.setRespline(respline);
    resp.setStatus(status);
    return resp;
}

QString HttpResponse::getData() const
{
    return data;
}

void HttpResponse::setData(const QString &value)
{
    data = value;
}

QString HttpResponse::getRespline() const
{
    return respline;
}

void HttpResponse::setRespline(const QString &value)
{
    respline = value;
}

int HttpResponse::getStatus() const
{
    return status;
}

void HttpResponse::setStatus(int value)
{
    status = value;
}
