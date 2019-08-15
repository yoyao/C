#include "uploadparam.h"

UploadParam::UploadParam()
{

}

UploadParam::UploadParam(QWidget *wid, QString &Path, QNetworkAccessManager *netman)
{
    this->wid=wid;
    this->Path=Path;
    this->netman=netman;
}
