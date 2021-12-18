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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventWindow
{
public:
    QPushButton *testButton;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *EventWindow)
    {
        if (EventWindow->objectName().isEmpty())
            EventWindow->setObjectName(QString::fromUtf8("EventWindow"));
        EventWindow->resize(640, 480);
        testButton = new QPushButton(EventWindow);
        testButton->setObjectName(QString::fromUtf8("testButton"));
        testButton->setGeometry(QRect(130, 80, 75, 24));
        label = new QLabel(EventWindow);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 220, 49, 16));
        label_2 = new QLabel(EventWindow);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(310, 210, 49, 16));

        retranslateUi(EventWindow);

        QMetaObject::connectSlotsByName(EventWindow);
    } // setupUi

    void retranslateUi(QWidget *EventWindow)
    {
        EventWindow->setWindowTitle(QCoreApplication::translate("EventWindow", "Form", nullptr));
        testButton->setText(QCoreApplication::translate("EventWindow", "PushButton", nullptr));
        label->setText(QCoreApplication::translate("EventWindow", "Test1", nullptr));
        label_2->setText(QCoreApplication::translate("EventWindow", "Test2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EventWindow: public Ui_EventWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTWINDOW_H
