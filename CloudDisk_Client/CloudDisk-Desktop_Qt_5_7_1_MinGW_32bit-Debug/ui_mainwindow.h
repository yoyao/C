/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <mainheader.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QHBoxLayout *horizontalLayout;
    MainHeader *left_wg;
    QWidget *right_wg;
    QHBoxLayout *horizontalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page_list;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_8;
    QListWidget *listWidget;
    QWidget *page_upload;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_2;
    QLabel *label_2;
    QWidget *page_myshare;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_5;
    QLabel *label_3;
    QWidget *page_shares;
    QHBoxLayout *horizontalLayout_6;
    QWidget *widget_4;
    QLabel *label_4;
    QTableWidget *tabwg_shares;
    QWidget *page_trans;
    QHBoxLayout *horizontalLayout_7;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_9;
    QTabWidget *tabWidget;
    QWidget *tab_upload;
    QHBoxLayout *horizontalLayout_10;
    QTextEdit *textEdit;
    QWidget *tab_download;
    QWidget *tab_record;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(1068, 670);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resource/logo_ico.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(MainWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        left_wg = new MainHeader(MainWindow);
        left_wg->setObjectName(QStringLiteral("left_wg"));

        horizontalLayout->addWidget(left_wg);

        right_wg = new QWidget(MainWindow);
        right_wg->setObjectName(QStringLiteral("right_wg"));
        right_wg->setStyleSheet(QStringLiteral(""));
        horizontalLayout_2 = new QHBoxLayout(right_wg);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(right_wg);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page_list = new QWidget();
        page_list->setObjectName(QStringLiteral("page_list"));
        horizontalLayout_3 = new QHBoxLayout(page_list);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(page_list);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_8 = new QHBoxLayout(widget);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(widget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setFrameShape(QFrame::NoFrame);
        listWidget->setFrameShadow(QFrame::Raised);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget->setResizeMode(QListView::Adjust);
        listWidget->setSpacing(30);

        horizontalLayout_8->addWidget(listWidget);


        horizontalLayout_3->addWidget(widget);

        stackedWidget->addWidget(page_list);
        page_upload = new QWidget();
        page_upload->setObjectName(QStringLiteral("page_upload"));
        horizontalLayout_4 = new QHBoxLayout(page_upload);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(page_upload);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(150, 80, 132, 38));

        horizontalLayout_4->addWidget(widget_2);

        stackedWidget->addWidget(page_upload);
        page_myshare = new QWidget();
        page_myshare->setObjectName(QStringLiteral("page_myshare"));
        horizontalLayout_5 = new QHBoxLayout(page_myshare);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        widget_5 = new QWidget(page_myshare);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        label_3 = new QLabel(widget_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(130, 120, 132, 38));

        horizontalLayout_5->addWidget(widget_5);

        stackedWidget->addWidget(page_myshare);
        page_shares = new QWidget();
        page_shares->setObjectName(QStringLiteral("page_shares"));
        horizontalLayout_6 = new QHBoxLayout(page_shares);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(page_shares);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        label_4 = new QLabel(widget_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(450, 40, 132, 38));
        tabwg_shares = new QTableWidget(widget_4);
        tabwg_shares->setObjectName(QStringLiteral("tabwg_shares"));
        tabwg_shares->setGeometry(QRect(160, 110, 801, 511));

        horizontalLayout_6->addWidget(widget_4);

        stackedWidget->addWidget(page_shares);
        page_trans = new QWidget();
        page_trans->setObjectName(QStringLiteral("page_trans"));
        horizontalLayout_7 = new QHBoxLayout(page_trans);
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(page_trans);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_9 = new QHBoxLayout(widget_3);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        tabWidget = new QTabWidget(widget_3);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_upload = new QWidget();
        tab_upload->setObjectName(QStringLiteral("tab_upload"));
        horizontalLayout_10 = new QHBoxLayout(tab_upload);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        textEdit = new QTextEdit(tab_upload);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        horizontalLayout_10->addWidget(textEdit);

        tabWidget->addTab(tab_upload, QString());
        tab_download = new QWidget();
        tab_download->setObjectName(QStringLiteral("tab_download"));
        tabWidget->addTab(tab_download, QString());
        tab_record = new QWidget();
        tab_record->setObjectName(QStringLiteral("tab_record"));
        tabWidget->addTab(tab_record, QString());

        horizontalLayout_9->addWidget(tabWidget);


        horizontalLayout_7->addWidget(widget_3);

        stackedWidget->addWidget(page_trans);

        horizontalLayout_2->addWidget(stackedWidget);


        horizontalLayout->addWidget(right_wg);


        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(4);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CloudDisk", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">\344\270\212\344\274\240\345\210\227\350\241\250</span></p></body></html>", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">\346\210\221\347\232\204\345\210\206\344\272\253</span></p></body></html>", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">\345\210\206\344\272\253\345\210\227\350\241\250</span></p></body></html>", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_upload), QApplication::translate("MainWindow", "\344\270\212\344\274\240\350\256\260\345\275\225", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_download), QApplication::translate("MainWindow", "\344\270\213\350\275\275\350\256\260\345\275\225", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_record), QApplication::translate("MainWindow", "\344\274\240\350\276\223\350\256\260\345\275\225", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
