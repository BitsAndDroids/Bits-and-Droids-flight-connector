/********************************************************************************
** Form generated from reading UI file 'outputmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
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
        widget = new QWidget(OutputMenu);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 721, 1500));
        containerLayout = new QVBoxLayout(widget);
        containerLayout->setObjectName(QString::fromUtf8("containerLayout"));

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
