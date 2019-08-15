/********************************************************************************
** Form generated from reading UI file 'mainheader.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINHEADER_H
#define UI_MAINHEADER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainHeader
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_7;
    QToolButton *tbtn_user;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_6;
    QToolButton *tbtn_filelist;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QToolButton *tbtn_upload;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *tbtn_myshare;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *tbtn_share_list;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *tbtn_trans;
    QWidget *tbtn_exit_wg;
    QHBoxLayout *horizontalLayout_5;
    QToolButton *tbtn_exit;

    void setupUi(QWidget *MainHeader)
    {
        if (MainHeader->objectName().isEmpty())
            MainHeader->setObjectName(QStringLiteral("MainHeader"));
        MainHeader->resize(105, 743);
        verticalLayout_2 = new QVBoxLayout(MainHeader);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(MainHeader);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_8 = new QWidget(widget);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        horizontalLayout_7 = new QHBoxLayout(widget_8);
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        tbtn_user = new QToolButton(widget_8);
        tbtn_user->setObjectName(QStringLiteral("tbtn_user"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/resource/header_user.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_user->setIcon(icon);
        tbtn_user->setIconSize(QSize(60, 60));
        tbtn_user->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_user->setAutoRaise(true);

        horizontalLayout_7->addWidget(tbtn_user);


        verticalLayout->addWidget(widget_8);

        widget_7 = new QWidget(widget);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        horizontalLayout_6 = new QHBoxLayout(widget_7);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        tbtn_filelist = new QToolButton(widget_7);
        tbtn_filelist->setObjectName(QStringLiteral("tbtn_filelist"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resource/header_file.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_filelist->setIcon(icon1);
        tbtn_filelist->setIconSize(QSize(60, 60));
        tbtn_filelist->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_filelist->setAutoRaise(true);

        horizontalLayout_6->addWidget(tbtn_filelist);


        verticalLayout->addWidget(widget_7);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tbtn_upload = new QToolButton(widget_3);
        tbtn_upload->setObjectName(QStringLiteral("tbtn_upload"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/resource/header_upload.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_upload->setIcon(icon2);
        tbtn_upload->setIconSize(QSize(60, 60));
        tbtn_upload->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_upload->setAutoRaise(true);

        horizontalLayout->addWidget(tbtn_upload);


        verticalLayout->addWidget(widget_3);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_2 = new QHBoxLayout(widget_4);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        tbtn_myshare = new QToolButton(widget_4);
        tbtn_myshare->setObjectName(QStringLiteral("tbtn_myshare"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/resource/header_share.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_myshare->setIcon(icon3);
        tbtn_myshare->setIconSize(QSize(60, 60));
        tbtn_myshare->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_myshare->setAutoRaise(true);

        horizontalLayout_2->addWidget(tbtn_myshare);


        verticalLayout->addWidget(widget_4);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        tbtn_share_list = new QToolButton(widget_2);
        tbtn_share_list->setObjectName(QStringLiteral("tbtn_share_list"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/resource/header_hot.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_share_list->setIcon(icon4);
        tbtn_share_list->setIconSize(QSize(60, 60));
        tbtn_share_list->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_share_list->setAutoRaise(true);

        horizontalLayout_3->addWidget(tbtn_share_list);


        verticalLayout->addWidget(widget_2);

        widget_5 = new QWidget(widget);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        horizontalLayout_4 = new QHBoxLayout(widget_5);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        tbtn_trans = new QToolButton(widget_5);
        tbtn_trans->setObjectName(QStringLiteral("tbtn_trans"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/resource/title_data.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_trans->setIcon(icon5);
        tbtn_trans->setIconSize(QSize(60, 60));
        tbtn_trans->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_trans->setAutoRaise(true);

        horizontalLayout_4->addWidget(tbtn_trans);


        verticalLayout->addWidget(widget_5);

        tbtn_exit_wg = new QWidget(widget);
        tbtn_exit_wg->setObjectName(QStringLiteral("tbtn_exit_wg"));
        horizontalLayout_5 = new QHBoxLayout(tbtn_exit_wg);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        tbtn_exit = new QToolButton(tbtn_exit_wg);
        tbtn_exit->setObjectName(QStringLiteral("tbtn_exit"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/resource/title_change.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_exit->setIcon(icon6);
        tbtn_exit->setIconSize(QSize(60, 60));
        tbtn_exit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tbtn_exit->setAutoRaise(true);

        horizontalLayout_5->addWidget(tbtn_exit);


        verticalLayout->addWidget(tbtn_exit_wg);


        verticalLayout_2->addWidget(widget);


        retranslateUi(MainHeader);

        QMetaObject::connectSlotsByName(MainHeader);
    } // setupUi

    void retranslateUi(QWidget *MainHeader)
    {
        MainHeader->setWindowTitle(QApplication::translate("MainHeader", "clouddisk", Q_NULLPTR));
        tbtn_user->setText(QApplication::translate("MainHeader", "\347\224\250\346\210\267", Q_NULLPTR));
        tbtn_filelist->setText(QApplication::translate("MainHeader", "\346\226\207\344\273\266\345\210\227\350\241\250", Q_NULLPTR));
        tbtn_upload->setText(QApplication::translate("MainHeader", "\344\270\212\344\274\240", Q_NULLPTR));
        tbtn_myshare->setText(QApplication::translate("MainHeader", "\346\210\221\347\232\204\345\210\206\344\272\253", Q_NULLPTR));
        tbtn_share_list->setText(QApplication::translate("MainHeader", "\345\210\206\344\272\253\345\210\227\350\241\250", Q_NULLPTR));
        tbtn_trans->setText(QApplication::translate("MainHeader", "\344\274\240\350\276\223\345\210\227\350\241\250", Q_NULLPTR));
        tbtn_exit->setText(QApplication::translate("MainHeader", "\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainHeader: public Ui_MainHeader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINHEADER_H
