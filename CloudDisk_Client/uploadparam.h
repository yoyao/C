#ifndef UPLOADPARAM_H
#define UPLOADPARAM_H

#include <QWidget>
#include <QString>
#include <QNetworkAccessManager>
#include <QTcpSocket>
#include <QFile>

class UploadParam
{
public:
    UploadParam();
    UploadParam(QWidget *wid,QString &Path,QNetworkAccessManager *netman);


public:
QWidget *wid;
QString Path;
QNetworkAccessManager *netman;
QFile *file;
QTcpSocket *socket;
};

#endif // UPLOADPARAM_H
