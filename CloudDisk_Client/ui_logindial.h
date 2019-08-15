/********************************************************************************
** Form generated from reading UI file 'logindial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIAL_H
#define UI_LOGINDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <loginupper.h>

QT_BEGIN_NAMESPACE

class Ui_LoginDial
{
public:
    QVBoxLayout *verticalLayout_3;
    LoginUpper *wg_head;
    QStackedWidget *stackedWidget;
    QWidget *page_login;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_6;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QWidget *widget_4;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *txt_account;
    QLabel *label_3;
    QLineEdit *txt_pwd;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_login;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_set;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget_8;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_7;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLineEdit *txt_ip;
    QLabel *label_5;
    QLineEdit *txt_port;
    QPushButton *btn_net_ok;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btn_net_cancel;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *LoginDial)
    {
        if (LoginDial->objectName().isEmpty())
            LoginDial->setObjectName(QStringLiteral("LoginDial"));
        LoginDial->resize(640, 480);
        LoginDial->setMinimumSize(QSize(640, 480));
        QIcon icon;
        icon.addFile(QStringLiteral(":/resource/logo_ico.ico"), QSize(), QIcon::Normal, QIcon::Off);
        LoginDial->setWindowIcon(icon);
        verticalLayout_3 = new QVBoxLayout(LoginDial);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        wg_head = new LoginUpper(LoginDial);
        wg_head->setObjectName(QStringLiteral("wg_head"));

        verticalLayout_3->addWidget(wg_head);

        stackedWidget = new QStackedWidget(LoginDial);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page_login = new QWidget();
        page_login->setObjectName(QStringLiteral("page_login"));
        verticalLayout_2 = new QVBoxLayout(page_login);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 83, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        widget_2 = new QWidget(page_login);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        widget_6 = new QWidget(widget_2);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        verticalLayout_4 = new QVBoxLayout(widget_6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widget_3 = new QWidget(widget_6);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(widget_3);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout->addWidget(label);


        verticalLayout_4->addWidget(widget_3);

        widget_4 = new QWidget(widget_6);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        gridLayout = new QGridLayout(widget_4);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        txt_account = new QLineEdit(widget_4);
        txt_account->setObjectName(QStringLiteral("txt_account"));

        gridLayout->addWidget(txt_account, 0, 1, 1, 1);

        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        txt_pwd = new QLineEdit(widget_4);
        txt_pwd->setObjectName(QStringLiteral("txt_pwd"));
        txt_pwd->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(txt_pwd, 1, 1, 1, 1);


        verticalLayout_4->addWidget(widget_4);

        widget_5 = new QWidget(widget_6);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        verticalLayout = new QVBoxLayout(widget_5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        btn_login = new QPushButton(widget_5);
        btn_login->setObjectName(QStringLiteral("btn_login"));

        verticalLayout->addWidget(btn_login);


        verticalLayout_4->addWidget(widget_5);


        horizontalLayout_2->addWidget(widget_6);

        horizontalSpacer = new QSpacerItem(139, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(widget_2);

        verticalSpacer_2 = new QSpacerItem(20, 83, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        stackedWidget->addWidget(page_login);
        page_set = new QWidget();
        page_set->setObjectName(QStringLiteral("page_set"));
        horizontalLayout_3 = new QHBoxLayout(page_set);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        widget = new QWidget(page_set);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        widget_8 = new QWidget(widget);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        verticalLayout_5 = new QVBoxLayout(widget_8);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalSpacer_3 = new QSpacerItem(20, 139, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        widget_7 = new QWidget(widget_8);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        gridLayout_2 = new QGridLayout(widget_7);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_4 = new QLabel(widget_7);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        txt_ip = new QLineEdit(widget_7);
        txt_ip->setObjectName(QStringLiteral("txt_ip"));

        gridLayout_2->addWidget(txt_ip, 0, 2, 1, 2);

        label_5 = new QLabel(widget_7);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        txt_port = new QLineEdit(widget_7);
        txt_port->setObjectName(QStringLiteral("txt_port"));

        gridLayout_2->addWidget(txt_port, 1, 2, 1, 2);

        btn_net_ok = new QPushButton(widget_7);
        btn_net_ok->setObjectName(QStringLiteral("btn_net_ok"));

        gridLayout_2->addWidget(btn_net_ok, 2, 0, 1, 2);

        horizontalSpacer_7 = new QSpacerItem(97, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 2, 2, 1, 1);

        btn_net_cancel = new QPushButton(widget_7);
        btn_net_cancel->setObjectName(QStringLiteral("btn_net_cancel"));

        gridLayout_2->addWidget(btn_net_cancel, 2, 3, 1, 1);


        verticalLayout_5->addWidget(widget_7);

        verticalSpacer_4 = new QSpacerItem(20, 139, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_4);


        horizontalLayout_4->addWidget(widget_8);

        horizontalSpacer_4 = new QSpacerItem(117, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        horizontalLayout_3->addWidget(widget);

        stackedWidget->addWidget(page_set);

        verticalLayout_3->addWidget(stackedWidget);


        retranslateUi(LoginDial);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LoginDial);
    } // setupUi

    void retranslateUi(QDialog *LoginDial)
    {
        LoginDial->setWindowTitle(QApplication::translate("LoginDial", "OldDriver", Q_NULLPTR));
        label->setText(QApplication::translate("LoginDial", "<html><head/><body><p align=\"center\">\347\224\250\346\210\267\347\231\273\345\275\225</p></body></html>", Q_NULLPTR));
        label_2->setText(QApplication::translate("LoginDial", "\347\224\250\346\210\267\345\220\215:", Q_NULLPTR));
        txt_account->setText(QApplication::translate("LoginDial", "sanji", Q_NULLPTR));
        label_3->setText(QApplication::translate("LoginDial", "\345\257\206\347\240\201:", Q_NULLPTR));
        txt_pwd->setText(QApplication::translate("LoginDial", "111", Q_NULLPTR));
        btn_login->setText(QApplication::translate("LoginDial", "\347\231\273\345\275\225", Q_NULLPTR));
        label_4->setText(QApplication::translate("LoginDial", "IP:", Q_NULLPTR));
        label_5->setText(QApplication::translate("LoginDial", "\347\253\257\345\217\243:", Q_NULLPTR));
        btn_net_ok->setText(QApplication::translate("LoginDial", "\347\241\256\345\256\232", Q_NULLPTR));
        btn_net_cancel->setText(QApplication::translate("LoginDial", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LoginDial: public Ui_LoginDial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIAL_H
