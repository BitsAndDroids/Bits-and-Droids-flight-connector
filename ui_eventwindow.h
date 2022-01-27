/********************************************************************************
** Form generated from reading UI file 'eventwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTWINDOW_H
#define UI_EVENTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventWindow
{
public:

    void setupUi(QWidget *EventWindow)
    {
        if (EventWindow->objectName().isEmpty())
            EventWindow->setObjectName(QString::fromUtf8("EventWindow"));
        EventWindow->resize(640, 480);

        retranslateUi(EventWindow);

        QMetaObject::connectSlotsByName(EventWindow);
    } // setupUi

    void retranslateUi(QWidget *EventWindow)
    {
        EventWindow->setWindowTitle(QCoreApplication::translate("EventWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EventWindow: public Ui_EventWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTWINDOW_H
