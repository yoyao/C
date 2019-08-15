#include <QDebug>
#include <QTableWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QToolButton>
#include <QIcon>
#include <QStringList>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include <QCryptographicHash>
#include <QNetworkConfiguration>
#include <QTcpSocket>
#include <QTextCodec>
#include "httpresponse.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"

//上传过程中每次上传的字节数为200k
#define UPLOAD_BUFF_SIZE  1024*200

const QString IPAddr="192.168.82.25";
const QString Port="31763";

UserInfo MainWindow::log_user;

MainWindow::MainWindow(QWidget *parent) :QWidget(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //初始化线程池 最大，最小和任务队列最大数量都是5个 也就是最大允许5个任务同时进行上传
    this->threadpool= threadpool_create(5,5,5);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    this->setWindowFlags(Qt::FramelessWindowHint|this->windowFlags());

    /*
    shm.setKey(tr("shm_user"));
    shm.attach();
    char *p_shm_user=(char*)shm.data();
    if(!shm.isAttached())
    {
        QMessageBox::about(this,tr("error"),tr("Login User Not Found"));
        exit(0);
    }
    memcpy(&log_user,p_shm_user,sizeof(log_user));
    */
    QString username;
    username="Nami";
    log_user.SetName(username);
    log_user.SetUid(3);
    //username=log_user.GetName();
    this->ui->left_wg->SetUserName(username);

    this->netman =Common::GetNetManager();

    connect(this->ui->left_wg,&MainHeader::SigGetFileList,this,&this->GetFileList);
    connect(this->ui->left_wg,&MainHeader::SigToUpload,this,&this->ToUpload);
    connect(this->ui->left_wg,&MainHeader::SigGetMyShare,this,&this->GetMyShare);
    connect(this->ui->left_wg,&MainHeader::SigGetShares,this,&this->GetShares);
    connect(this->ui->left_wg,&MainHeader::SigGetTransRecord,this,&this->GetTransRecord);
    connect(this,&MainWindow::OnFileCheckResponse,this,&MainWindow::FileCheckResponse);

    ui->listWidget->setViewMode(QListView::IconMode);   //设置显示图标模式
    ui->listWidget->setIconSize(QSize(80, 80));         //设置图标大小
    ui->listWidget->setGridSize(QSize(120, 120));       //设置item大小

    // 设置QLisView大小改变时，图标的调整模式，默认是固定的，可以改成自动调整
    ui->listWidget->setResizeMode(QListView::Adjust);   //自动适应布局
    // 设置列表可以拖动，如果想固定不能拖动，使用QListView::Static
    ui->listWidget->setMovement(QListView::Static);

    // listWidget 右键菜单
    // 发出 customContextMenuRequested 信号
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    GetFileList(0);

}

void MainWindow::GetFileList(int index)
{
    this->ui->stackedWidget->setCurrentIndex(index);
    this->ui->listWidget->clear();

    QString req_url=QString("http://"+IPAddr+":"+Port+"/filelist");
    QUrl url(req_url);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json;charset=utf-8");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) \
                      Chrome/59.0.3071.115 Safari/537.36");
                      QString req_str=QString("{\"uid\":%1,\"timespan\":%2,\"token\":\"this is token\"}")
            .arg(QString::number(log_user.GetUid()))
            .arg(QString::number(QDateTime::currentDateTime().toTime_t()));
            qDebug()<<"get file list request data: "<<req_str;

    QNetworkReply* reply = NULL;
    reply=this->netman->post(request,req_str.toUtf8());
    //reply=manager->post(request,req_str.toUtf8());
    connect(reply, &QNetworkReply::readyRead, [=](){

        if(reply->error() !=QNetworkReply::NoError)
        {
            QMessageBox::about(this,tr("error"),tr("net link failed"));
            qDebug()<<"net work errorno:"<<reply->error();
            return;
        }
        if(reply->isReadable())
        {
            // 读sever回写的数据
            QByteArray jsonData = reply->readAll();
            qDebug()<<jsonData;
            QJsonDocument doc=QJsonDocument::fromJson(jsonData);
            if(!doc.isObject())
            {
                QMessageBox::about(this,tr("error"),tr("recv data from server error!"));
                return;
            }
            QJsonObject obj=doc.object();
            if(obj.value("status").toInt()!=0)
            {
                QMessageBox::about(this,tr("error"),tr("Get data from server error!"));
                return;
            }
            qDebug()<<obj.value("status").toInt();
            int count=0,i=0;
            count=obj.value("count").toInt();
            qDebug()<<"count is "<<count;
            if(!obj.value("list").isArray())
            {
                QMessageBox::about(this,tr("error"),tr("Get list data from server error\ntry again!"));
                return;
            }
            QJsonArray list=obj.value("list").toArray();
            for(i=0;i<count;++i)
            {
                QJsonObject user=list.at(i).toObject();
                if(user.empty())
                {
                    QMessageBox::about(this,tr("error"),tr("Analysis data error\ntry again!"));
                    return;
                }
                QIcon icon(":/resource/filetype/jpg.png");
                QListWidgetItem *item=new QListWidgetItem;
                item->setToolTip(user.value(tr("localname")).toString());
                item->setIcon(icon);
                item->setText(user.value(tr("localname")).toString());
                QHBoxLayout hlay;
                hlay.setMargin(10);
                this->ui->listWidget->addItem(item);
                qDebug()<<user.value("localname").toString();
            }
        }

    });

}

void MainWindow::GetMyShare(int index)
{
    this->ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::GetShares(int index)
{
    this->ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::GetTransRecord(int index)
{
    this->ui->stackedWidget->setCurrentIndex(index);
    this->ui->tabWidget->setCurrentIndex(0);
    test();
}

void MainWindow::FileCheckResponse(int status)
{
    QString msg;
    switch (status)
    {
    case 200:
    {
        msg=QString("秒传成功");
        break;
    }
    case 0:
    {
        //网盘内没有 需要新的上传

        break;
    }
    case 201:
    {
        msg=QString("文件已存在于您的文件夹内");
        break;
    }
    case 300:
    {
        msg=QString("秒传失败");
        break;
    }
    default:
    {
        msg=("上传失败");
        break;
    }
    }
    // qDebug()<<"upload file"<<msg;
    QMessageBox::about(this,tr("upload"),msg);
}

void MainWindow::ToUpload(int index)
{
    this->ui->stackedWidget->setCurrentIndex(index);

    QStringList filenames=QFileDialog::getOpenFileNames(this,tr("Select want upload files"),tr("d:/doc"));
    if(filenames.length()<1)
    {
        qDebug()<<"Not select file";
        return;
    }
    /* 上传报文
     * 0 首次上传 1没有后续数据，结束上传
        {"action":0,"localname":"shishi.jpg","filetype":2,"filesize":23353,"data":"","datalen":6465,"filehash":"ljglfg","user":{"name":"sanji","uid":2,"checkhash":"lkjlj"}}
    *询问报文
    * {"localname":"shishi.jpg","filesize":1321546,"filehash":"glanvljaey","user":{"name":"sanji","uid":2,"token":"lkjlj"}}
 */
    int i=filenames.length();
    for(;i>0;--i)
    {
        UploadParam *par=NULL;
        par=new UploadParam();
        // par= (UploadParam *)calloc(1,sizeof(UploadParam));
        //this,filenames.at(i),this->netman
        par->wid=this;
        par->Path=filenames.at(i-1);
        par->netman=Common::GetNetManager();
        threadpool_add(this->threadpool,MainWindow::UploadFile,par);
        //  UploadFile();
    }

}

void *MainWindow::UploadFile(void *arg)
{
    UploadParam *param=(UploadParam *)arg;
    QString path=param->Path;
    QFile file(path);

    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(param->wid,tr("file error"),path+" open failed");
        qDebug()<<path<<"UploadFile open failed\n";
        return NULL;
    }
    QFileInfo finfo(file);
    QByteArray file_arr=file.readAll();
    QString hashstr(QCryptographicHash::hash(file_arr,QCryptographicHash::Md5).toHex());
    qDebug()<<path<<":"<<hashstr;
    QString name=finfo.fileName();
    QByteArray req_data=QString("{\"filesize\":%1,\"filehash\":\"%2\",\"user\":{\"name\":\"%3\",\"uid\":%4,\"token\":\"lkjlj\"},\"localname\":\"%5\"}")
            .arg(file.size()).arg(hashstr).arg(log_user.GetName()).arg(log_user.GetUid()).arg(name).toUtf8();

    qDebug()<<"upload req_data:"<<req_data;

    QTcpSocket socket;
    QString host=IPAddr;
    socket.connectToHost(host,80);
    if(!socket.waitForConnected(20))
    {
        qDebug()<<"tcp connect timeout";
    }

    QByteArray datagram;
    Common::CreateHttpRequest(datagram,req_data,"filecheck","POST");

    qDebug()<<"upload send tcpsocket datagram:"<<datagram;
    socket.write(datagram);
    socket.waitForBytesWritten(50000);
    if(!socket.waitForReadyRead(50000))
    {
        qDebug()<<"tcp waitForReadyRead timeout";
        return NULL;
    }
    QByteArray resp=socket.readAll();

    HttpResponse response=HttpResponse::FromByteArray(resp);
    /*
HTTP/1.1 200 OK\r\nServer: nginx/1.10.1\r\nDate: Sat, 29 Jul 2017 00:41:44 GMT\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: keep-alive\r\n\r\n27\r\n{\"status\":201,\"error\":201,\"msg\":\"succ\"}\r\n0\r\n\r\n
*/
    resp=response.getData().toUtf8();

    qDebug()<<"response:"<<QString(resp);

    QJsonDocument doc=QJsonDocument::fromJson(resp);
    if(!doc.isObject())
    {
        qDebug()<<"QJsonDocument parse error upload "+path+" failed";
        return NULL;
    }

    // 响应报文格式{"status":0,"error":2,"msg":\"\"}
    QJsonDocument root=QJsonDocument::fromJson(resp);
    if(!root.isObject())
    {
        QMessageBox::about(param->wid,tr("upload error"),"Server response QJsonDocument parse error upload "+path+" failed");
        return NULL;
    }
    QJsonObject obj=root.object();
    int status=obj.value("status").toInt();
    MainWindow *p_main=(MainWindow *)param->wid;
    //网盘内没有此文件 需要上传
    if(status==0)
    {
        param->file=&file;
        param->socket=&socket;
        status= RealUploadFile(param);
    }


    emit p_main->OnFileCheckResponse(status);


    delete param;
    socket.close();

    file.close();

    return NULL;
}

int MainWindow::RealUploadFile(UploadParam *para)
{
    int ret=0;
    char buf[UPLOAD_BUFF_SIZE]={0};//
    QByteArray file_arr;
    QFile mfile(para->Path);
    QFileInfo finfo(mfile.fileName());

    if(!mfile.isOpen())
    {
        if(!mfile.open(QIODevice::ReadOnly))
        {
            return -1;
        }
    }
    file_arr=mfile.readAll();
    mfile.reset();
    QByteArray datagram,filehash;
    filehash=QCryptographicHash::hash(file_arr,QCryptographicHash::Md5).toHex();
    QByteArray req_str="{\"localname\":";
    req_str.append("\""+finfo.fileName()+"\",");
    req_str.append("\"filesize\":");
    req_str.append(""+QString::number(mfile.size())+",");
    req_str.append("\"isfirst\":");
    req_str.append("1,");
    req_str.append("\"filehash\":");
    req_str.append("\""+filehash+"\",");
    req_str.append("\"user\":");
    req_str.append("{");
    req_str.append("\"name\":");
    req_str.append("\""+log_user.GetName()+"\",");
    req_str.append("\"uid\":");
    req_str.append(""+QString::number(log_user.GetUid())+",");
    req_str.append("\"token\":");
    req_str.append("\"nami\"");
    req_str.append("},");

    QTcpSocket socket;
    QString host=IPAddr;
    socket.connectToHost(host,Port.toInt());
    if(!socket.waitForConnected(10000))
    {
        qDebug()<<"tcp connect timeout";
        return  -2;
    }
    int sign=0;
    QByteArray adddata;
    while (!mfile.atEnd())
    {
        adddata.clear();
        datagram.clear();
        adddata.append("\"timespan\":");
        adddata.append(""+QString::number(QDateTime::currentDateTime().toTime_t())+",");

        int len= mfile.read(buf,UPLOAD_BUFF_SIZE);
        //{"localname":"adgkjhkj","size":64546,"isfirst":1,"hash":"lfkhlf","timespan":461312,"data":"","datasize":"",user:{"name":"alsdg","uid":3,"token":"adfg"}}
        if(len<UPLOAD_BUFF_SIZE)
        {
            adddata.append("\"isend\":");
            adddata.append("1,");
        }
        adddata.append("\"sign\":"+QString::number(sign)+",");
        QByteArray databuf;
        databuf.append(buf,len);
        adddata.append("\"datasize\":");
        adddata.append(""+QString::number(databuf.length())+"");
        //adddata.append("}\r\n\r\n");
        adddata.append("}");
        QByteArray reque_data=req_str+adddata;
        //此时长度为完整的json字符串的长度
        len=reque_data.length();
        //追加上传文件的信息
        reque_data.append(databuf);

        reque_data.insert(0,(char*)&len,4);
        socket.write(reque_data);
        //发送数据报文
        if(!socket.waitForBytesWritten(5000))
        {
            qDebug()<<"tcp waitForBytesWritten timeout";
            break;
        }
        if(!socket.waitForReadyRead(5000))
        {
            qDebug()<<"tcp waitForReadyRead timeout";
            break;
        }
        QByteArray respdata=socket.readAll();
        qDebug()<<respdata;
        QJsonDocument doc=QJsonDocument::fromJson(respdata);
        if(!doc.isObject())
        {
            qDebug()<<"QJsonDocument parse error upload test func failed";
            break;
        }

        QJsonObject obj=doc.object();
        int status=obj.value("status").toInt();
        qDebug()<<"rsp status is "<<status;
        if(status!=0)
        {
            qDebug()<<"response status error";
            break;
        }
        ++sign;
    }
    qDebug()<<"sign "<<sign;
    socket.close();
    mfile.close();

    return ret;
}

void MainWindow::test()
{
    int ret=0;
    char buf[UPLOAD_BUFF_SIZE]={0};//
    QByteArray file_arr;
    QFile mfile("D:/doc/335979166434723169.jpg");
    QFileInfo finfo(mfile.fileName());

    if(!mfile.isOpen())
    {
        if(!mfile.open(QIODevice::ReadOnly))
        {
            return ;
        }
    }
    file_arr=mfile.readAll();
    mfile.reset();
    QByteArray datagram,filehash;
    filehash=QCryptographicHash::hash(file_arr,QCryptographicHash::Md5).toHex();
    QByteArray req_str="{\"localname\":";
    req_str.append("\""+finfo.fileName()+"\",");
    req_str.append("\"filesize\":");
    req_str.append(""+QString::number(mfile.size())+",");
    req_str.append("\"isfirst\":");
    req_str.append("1,");
    req_str.append("\"filehash\":");
    req_str.append("\""+filehash+"\",");
    req_str.append("\"user\":");
    req_str.append("{");
    req_str.append("\"name\":");
    req_str.append("\""+log_user.GetName()+"\",");
    req_str.append("\"uid\":");
    req_str.append(""+QString::number(log_user.GetUid())+",");
    req_str.append("\"token\":");
    req_str.append("\"nami\"");
    req_str.append("},");

    QTcpSocket socket;
    QString host=IPAddr;
    socket.connectToHost(host,Port.toInt());
    if(!socket.waitForConnected(10000))
    {
        qDebug()<<"tcp connect timeout";
        return  ;
    }
    int sign=0;
    QByteArray adddata;
    while (!mfile.atEnd())
    {
        adddata.clear();
        datagram.clear();
        adddata.append("\"timespan\":");
        adddata.append(""+QString::number(QDateTime::currentDateTime().toTime_t())+",");

        int len= mfile.read(buf,UPLOAD_BUFF_SIZE);
        //{"localname":"adgkjhkj","size":64546,"isfirst":1,"hash":"lfkhlf","timespan":461312,"data":"","datasize":"",user:{"name":"alsdg","uid":3,"token":"adfg"}}
        if(len<UPLOAD_BUFF_SIZE)
        {
            adddata.append("\"isend\":");
            adddata.append("1,");
        }
        adddata.append("\"sign\":"+QString::number(sign)+",");
        QByteArray databuf;
        databuf.append(buf,len);
        adddata.append("\"datasize\":");
        adddata.append(""+QString::number(databuf.length())+"");
        //adddata.append("}\r\n\r\n");
        adddata.append("}");
        QByteArray reque_data=req_str+adddata;
        //此时长度为完整的json字符串的长度
        len=reque_data.length();
        //追加上传文件的信息
        reque_data.append(databuf);

        reque_data.insert(0,(char*)&len,4);
        socket.write(reque_data);
        //发送数据报文
        if(!socket.waitForBytesWritten(5000))
        {
            qDebug()<<"tcp waitForBytesWritten timeout";
            break;
        }
        if(!socket.waitForReadyRead(5000))
        {
            qDebug()<<"tcp waitForReadyRead timeout";
            break;
        }
        QByteArray respdata=socket.readAll();
        qDebug()<<respdata;
        QJsonDocument doc=QJsonDocument::fromJson(respdata);
        if(!doc.isObject())
        {
            qDebug()<<"QJsonDocument parse error upload test func failed";
            break;
        }

        QJsonObject obj=doc.object();
        int status=obj.value("status").toInt();
        qDebug()<<"rsp status is "<<status;
        if(status!=0)
        {
            qDebug()<<"response status error";
            break;
        }
        ++sign;
    }
    qDebug()<<"sign "<<sign;
    socket.close();
    mfile.close();


}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        // 窗口跟随鼠标移动
        // 窗口左上角点 = 鼠标当前位置 - 差值
        this->move(event->globalPos() - m_pt);
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 如果鼠标左键按下
    if(event->button() == Qt::LeftButton)
    {
        // 求差值 = 鼠标当前位置 - 窗口左上角点
        m_pt = event->globalPos() - this->geometry().topLeft();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap bk(":/resource/title_bk3.jpg");
    painter.drawPixmap(0, 0, width(), height(), bk);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"Execute closeEvent";
    if(shm.isAttached())
    {
        shm.detach();
    }
    if(this->threadpool!=NULL)
    {
        threadpool_destroy(this->threadpool);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

