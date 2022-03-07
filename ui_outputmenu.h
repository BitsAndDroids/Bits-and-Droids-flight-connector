/********************************************************************************
** Form generated from reading UI file 'outputmenu.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUTMENU_H
#define UI_OUTPUTMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OutputMenu
{
public:
    QWidget *widget;
    QVBoxLayout *containerLayout;

    void setupUi(QWidget *OutputMenu)
    {
        if (OutputMenu->objectName().isEmpty())
            OutputMenu->setObjectName(QString::fromUtf8("OutputMenu"));
        OutputMenu->resize(746, 509);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OutputMenu->sizePolicy().hasHeightForWidth());
        OutputMenu->setSizePolicy(sizePolicy);
        widget = new QWidget(OutputMenu);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 721, 1500));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        containerLayout = new QVBoxLayout(widget);
        containerLayout->setSpacing(10);
        containerLayout->setObjectName(QString::fromUtf8("containerLayout"));
        containerLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

        retranslateUi(OutputMenu);

        QMetaObject::connectSlotsByName(OutputMenu);
    } // setupUi

    void retranslateUi(QWidget *OutputMenu)
    {
        OutputMenu->setWindowTitle(QCoreApplication::translate("OutputMenu", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OutputMenu: public Ui_OutputMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTMENU_H
