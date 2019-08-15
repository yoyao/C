#include "logindial.h"
#include "ui_logindial.h"
#include <QPicture>
#include <QPainter>
#include <QMessageBox>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QDateTime>
#include <QSharedMemory>
#include "userinfo.h"
#include "common.h"
#include "mainwindow.h"

LoginDial::LoginDial(QWidget *parent) :QDialog(parent),ui(new Ui::LoginDial)
{
    ui->setupUi(this);

    this->netman=Common::GetNetManager();

    this->ui->stackedWidget->setCurrentIndex(0);
    this->setWindowFlags(Qt::FramelessWindowHint|this->windowFlags());

    connect(this->ui->wg_head,&LoginUpper::SwitchTag,this,[=](int index){
        // QMessageBox::about(this,tr("about"),QString::number(index));
        this->ui->stackedWidget->setCurrentIndex(index);
    });

    connect(this->ui->btn_net_cancel,&QPushButton::clicked,this,[=](){
        // QMessageBox::about(this,tr("about"),QString::number(index));
        this->ui->stackedWidget->setCurrentIndex(0);
    });

    ReadConfig();
}

void LoginDial::paintEvent(QPaintEvent *event)
{
    //初始化画家类 并且告诉它 准备在 this中画画
    QPainter p(this);
    QPixmap img(":/resource/login_bk.jpg");
    p.drawPixmap(0,0,this->width(),this->height(),img);
}


int LoginDial::ReadConfig()
{
    QFile file(tr("F:/Itcast/Projects/QT/CloudDisk/disk.conf"));

    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(this,tr("about"),tr("Client initialization failed"));
        return -1;
    }
    QByteArray arr=file.readAll();
    file.close();

    arr=arr.simplified();
    QJsonDocument conf;
    conf=QJsonDocument::fromJson(arr);
    QJsonObject obj;
    obj=  conf.object();
    QJsonValue val=obj.value(tr("server"));
    obj=val.toObject();
    this->ui->txt_ip->setText(obj.value("ip").toString());
    this->ui->txt_port->setText(QString::number(obj.value("port").toInt()));
    return 0;
}

void LoginDial::on_btn_login_clicked()
{
    this->ui->btn_login->setEnabled(false);
    this->ui->btn_login->setText(tr("登录中..."));
    int ret=0;
    //"{\"account\":\"\",\"passwd\":\"\"}"
    QString req_str=QString("{\"account\":\"%1\",\"passwd\":\"%2\",\"timespan\":%3}")
            .arg(this->ui->txt_account->text())//账号
            .arg(this->ui->txt_pwd->text())//密码
            .arg(QString::number(QDateTime::currentDateTime().toTime_t()));//当前时间戳

    qDebug()<<"Request data:"<<req_str;
    QString requrl= QString("http://%1:%2/login").arg(this->ui->txt_ip->text()).arg(this->ui->txt_port->text());
    qDebug()<<"Request path:"<<requrl;
    QUrl url(requrl);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json;charset=utf-8");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) \
                      Chrome/59.0.3071.115 Safari/537.36");

                      QNetworkReply *reply= netman->post(request,req_str.toUtf8());
            connect(reply, &QNetworkReply::readyRead, [=](){
        if(reply->isReadable())
        {
            if(reply->error()!=QNetworkReply::NoError)
            {
                qDebug()<<QString("errorno:")<<QString::number(reply->error());
                QMessageBox::about(this,tr("Network error"),tr("Network not link to server"));
                return;
            }

            if(reply->isReadable())
            {
                UserInfo user;
                int ret=0;
                QByteArray arr=reply->readAll();
                qDebug()<<"Server response:"<<arr;
                ret=Login(arr,user);
                if(ret!=0)
                {
                    QMessageBox::about(this,tr("Login error"),tr("Account or password error!\nPlease change account!"));
                    return;
                }
                QSharedMemory shm;
                QString key=tr("shm_user");
                shm.setKey(key);
                if(!shm.create(sizeof(UserInfo)))
                {
                    QMessageBox::about(this,tr("Create shared memery error!"), shm.errorString());
                    return;
                }

                char *p_shm_user=(char*)shm.data();
                memcpy(p_shm_user,&user,sizeof(user));
                shm.attach();
                reply->close();
                MainWindow *mainw=new MainWindow();
                mainw->show();
                this->close();
            }
            if(reply->isOpen())
            {
                reply->close();
            }

        }

    });

}


int LoginDial::Login(QByteArray &json, UserInfo &user)
{
    int ret=0;
    QJsonDocument doc;
    doc=QJsonDocument::fromJson(json);
    if(!doc.isObject())
    {
        return -1;
    }
    QJsonObject obj;
    obj= doc.object();

    if(obj.value("status").toInt()!=0)
    {
        QMessageBox::about(this,tr("error"),tr("Login failed!"));
        this->ui->btn_login->setText(tr("登录"));
        return 0;
    }

    qDebug()<<"Login Success!";
    this->ui->btn_login->setText(tr("登录"));
    obj=obj.value("user").toObject();

    user=UserInfo::FromJsonString(obj);

    qDebug()<<"Login success rectime:"<<user.GetRectime().toString("yyyy-MM-dd hh:mm:ss");

    this->ui->btn_login->setEnabled(true);


    return ret;
}


void LoginDial::closeEvent(QCloseEvent *)
{
    if(this->netman!=NULL)
    {
        //delete this->netman;
    }
}
LoginDial::~LoginDial()
{
    if(this->ui !=NULL)
    {
         delete ui;
    }
}
