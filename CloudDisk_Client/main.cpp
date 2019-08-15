#include "mainwindow.h"
#include <QApplication>
#include "logindial.h"
#include "mainheader.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDial log;
    MainWindow man;
    //w.show();
    //log.show();
    man.show();
    return a.exec();
}
