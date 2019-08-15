/********************************************************************************
** Form generated from reading UI file 'loginupper.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINUPPER_H
#define UI_LOGINUPPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginUpper
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lab_logo;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lab_title;
    QSpacerItem *horizontalSpacer;
    QToolButton *tbtn_setting;
    QToolButton *tbtn_min;
    QToolButton *tbtn_close;

    void setupUi(QWidget *LoginUpper)
    {
        if (LoginUpper->objectName().isEmpty())
            LoginUpper->setObjectName(QStringLiteral("LoginUpper"));
        LoginUpper->resize(941, 58);
        horizontalLayout = new QHBoxLayout(LoginUpper);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(LoginUpper);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        lab_logo = new QLabel(widget);
        lab_logo->setObjectName(QStringLiteral("lab_logo"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_logo->sizePolicy().hasHeightForWidth());
        lab_logo->setSizePolicy(sizePolicy);
        lab_logo->setStyleSheet(QStringLiteral("border:0 0 0 0;"));
        lab_logo->setPixmap(QPixmap(QString::fromUtf8(":/resource/logo.ico")));

        horizontalLayout_2->addWidget(lab_logo);

        horizontalSpacer_3 = new QSpacerItem(19, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        lab_title = new QLabel(widget);
        lab_title->setObjectName(QStringLiteral("lab_title"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        lab_title->setFont(font);

        horizontalLayout_2->addWidget(lab_title);

        horizontalSpacer = new QSpacerItem(660, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        tbtn_setting = new QToolButton(widget);
        tbtn_setting->setObjectName(QStringLiteral("tbtn_setting"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/resource/login_setting.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_setting->setIcon(icon);
        tbtn_setting->setIconSize(QSize(30, 30));
        tbtn_setting->setAutoRaise(true);

        horizontalLayout_2->addWidget(tbtn_setting);

        tbtn_min = new QToolButton(widget);
        tbtn_min->setObjectName(QStringLiteral("tbtn_min"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resource/login_min.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_min->setIcon(icon1);
        tbtn_min->setIconSize(QSize(30, 30));
        tbtn_min->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tbtn_min->setAutoRaise(true);

        horizontalLayout_2->addWidget(tbtn_min);

        tbtn_close = new QToolButton(widget);
        tbtn_close->setObjectName(QStringLiteral("tbtn_close"));
        tbtn_close->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/resource/login_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_close->setIcon(icon2);
        tbtn_close->setIconSize(QSize(30, 30));
        tbtn_close->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tbtn_close->setAutoRaise(true);

        horizontalLayout_2->addWidget(tbtn_close);


        horizontalLayout->addWidget(widget);


        retranslateUi(LoginUpper);

        QMetaObject::connectSlotsByName(LoginUpper);
    } // setupUi

    void retranslateUi(QWidget *LoginUpper)
    {
        LoginUpper->setWindowTitle(QApplication::translate("LoginUpper", "LoginUpper", Q_NULLPTR));
        lab_logo->setText(QString());
        lab_title->setText(QApplication::translate("LoginUpper", "\345\217\270\346\234\272\344\272\221\347\233\230", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        tbtn_setting->setToolTip(QApplication::translate("LoginUpper", "\350\256\276\347\275\256", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        tbtn_setting->setText(QString());
#ifndef QT_NO_TOOLTIP
        tbtn_min->setToolTip(QApplication::translate("LoginUpper", "\346\234\200\345\260\217\345\214\226", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        tbtn_min->setText(QString());
#ifndef QT_NO_TOOLTIP
        tbtn_close->setToolTip(QApplication::translate("LoginUpper", "\345\205\263\351\227\255", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        tbtn_close->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginUpper: public Ui_LoginUpper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINUPPER_H
