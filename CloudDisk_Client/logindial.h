#ifndef LOGINDIAL_H
#define LOGINDIAL_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "userinfo.h"

namespace Ui {
class LoginDial;
}

class LoginDial : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDial(QWidget *parent = 0);
    ~LoginDial();

protected:
    virtual void paintEvent(QPaintEvent *event);
virtual void closeEvent(QCloseEvent *);
private slots:
    void on_btn_login_clicked();

private:
    int ReadConfig();
    int Login(QByteArray &json, UserInfo &user);
private:
    Ui::LoginDial *ui;
    QNetworkAccessManager *netman;
};

#endif // LOGINDIAL_H
