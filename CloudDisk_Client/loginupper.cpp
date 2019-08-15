#include "loginupper.h"
#include "ui_loginupper.h"
#include <QMouseEvent>


LoginUpper::LoginUpper(QWidget *parent) : QWidget(parent), ui(new Ui::LoginUpper)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|this->windowFlags());
    QPixmap logobg(":/resource/logo.ico");
    this->ui->lab_logo->setPixmap(logobg);
    this->m_parent = parent;

    connect(this->ui->tbtn_setting,&QToolButton::clicked,[=](){
        emit this->SwitchTag(1);
    });
    connect(this->ui->tbtn_min,&QToolButton::clicked,[&](){
        this->m_parent->showMinimized();
    });


}

void LoginUpper::mouseMoveEvent(QMouseEvent *event)
{
    // 只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        // 窗口跟随鼠标移动
        // 窗口左上角点 = 鼠标当前位置 - 差值
        this->m_parent->move(event->globalPos() - m_pt);
    }
}

void LoginUpper::mousePressEvent(QMouseEvent *ev)
{
    // 如果鼠标左键按下
    if(ev->button() == Qt::LeftButton)
    {
        // 求差值 = 鼠标当前位置 - 窗口左上角点
        m_pt = ev->globalPos() - m_parent->geometry().topLeft();
    }

}

void LoginUpper::on_tbtn_close_clicked()
{
    if(this->m_parent !=NULL)
    {
        m_parent->close();
    }
    else
    {
        this->close();
    }
}

LoginUpper::~LoginUpper()
{
    delete ui;
}
