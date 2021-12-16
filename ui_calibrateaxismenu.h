/********************************************************************************
** Form generated from reading UI file 'calibrateaxismenu.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATEAXISMENU_H
#define UI_CALIBRATEAXISMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalibrateAxisMenu
{
public:

    void setupUi(QWidget *CalibrateAxisMenu)
    {
        if (CalibrateAxisMenu->objectName().isEmpty())
            CalibrateAxisMenu->setObjectName(QString::fromUtf8("CalibrateAxisMenu"));
        CalibrateAxisMenu->resize(400, 300);

        retranslateUi(CalibrateAxisMenu);

        QMetaObject::connectSlotsByName(CalibrateAxisMenu);
    } // setupUi

    void retranslateUi(QWidget *CalibrateAxisMenu)
    {
        CalibrateAxisMenu->setWindowTitle(QCoreApplication::translate("CalibrateAxisMenu", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibrateAxisMenu: public Ui_CalibrateAxisMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATEAXISMENU_H
