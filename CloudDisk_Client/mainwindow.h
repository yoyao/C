#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QMouseEvent>
#include <QSharedMemory>
#include "uploadparam.h"
#include "userinfo.h"
#include "threadpool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void OnNetWorkSendFinished(QNetworkReply *reply);
    static void* UploadFile(void *arg);
 static int RealUploadFile(UploadParam *para);
 void test();
signals:
    void ToReadyRead(QNetworkReply *reply);
    void OnFileCheckResponse(int code);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void ToUpload(int index);
    void GetFileList(int index);
    void GetMyShare(int index);
    void GetShares(int index);
    void GetTransRecord(int index);
    void FileCheckResponse(int status);
private:
   Ui::MainWindow *ui;
   QPoint m_pt;    // 差值: 鼠标当前位置 - 窗口左上角点
   static UserInfo log_user;
    QSharedMemory shm;
    threadpool_t *threadpool;
    QNetworkAccessManager *netman;

};

#endif // MAINWINDOW_H
