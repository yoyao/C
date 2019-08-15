#ifndef COMMON_H
#define COMMON_H

#include <QNetworkAccessManager>

class Common
{
public:
    Common();
static QNetworkAccessManager* GetNetManager();
static void CreateHttpRequest(QByteArray &result, QByteArray &data, const char* operation, const char *method);
private:
static QNetworkAccessManager *m_netManager;
};

#endif // COMMON_H
