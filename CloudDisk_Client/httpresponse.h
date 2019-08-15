#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QString>
#include <QByteArray>
#include <QMap>

class HttpResponse
{
public:
    HttpResponse();
    static HttpResponse FromByteArray(QByteArray &format);
    QString getData() const;
    void setData(const QString &value);

    QString getRespline() const;
    void setRespline(const QString &value);

    int getStatus() const;
    void setStatus(int value);

private:
    int status;
    QString data;
    QString respline;
};

#endif // HTTPRESPONSE_H
