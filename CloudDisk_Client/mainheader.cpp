#include "mainheader.h"
#include "ui_mainheader.h"
#include <QPainter>

MainHeader::MainHeader(QWidget *parent) :QWidget(parent),ui(new Ui::MainHeader)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|this->windowFlags());
    this->m_parent =parent;
}

void MainHeader::paintEvent(QPaintEvent *event)
{
    //初始化画家类 并且告诉它 准备在 this中画画
    QPainter p(this);
    QPixmap img(":/resource/header_bk.jpg");
    p.drawPixmap(0,0,this->width(),this->height(),img);
}

MainHeader::~MainHeader()
{
    delete ui;
}

void MainHeader::SetUserName(QString &name)
{
    this->ui->tbtn_user->setText(name);
}

void MainHeader::on_tbtn_filelist_clicked()
{
    emit   SigGetFileList(0);

}

void MainHeader::on_tbtn_upload_clicked()
{
    emit  SigToUpload(1);
}

void MainHeader::on_tbtn_myshare_clicked()
{
    emit  SigGetFileList(2);
}

void MainHeader::on_tbtn_share_list_clicked()
{
    emit  SigGetMyShare(3);

}

void MainHeader::on_tbtn_trans_clicked()
{
    emit  SigGetTransRecord(4);
}

void MainHeader::on_tbtn_exit_clicked()
{
    if(this->m_parent!=NULL)
    {
        this->m_parent->close();
    }
}

