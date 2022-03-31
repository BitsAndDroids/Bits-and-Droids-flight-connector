/********************************************************************************
** Form generated from reading UI file 'librarygeneratorwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIBRARYGENERATORWINDOW_H
#define UI_LIBRARYGENERATORWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LibraryGeneratowWindow
{
public:

    void setupUi(QWidget *LibraryGeneratowWindow)
    {
        if (LibraryGeneratowWindow->objectName().isEmpty())
            LibraryGeneratowWindow->setObjectName(QString::fromUtf8("LibraryGeneratowWindow"));
        LibraryGeneratowWindow->resize(400, 300);

        retranslateUi(LibraryGeneratowWindow);

        QMetaObject::connectSlotsByName(LibraryGeneratowWindow);
    } // setupUi

    void retranslateUi(QWidget *LibraryGeneratowWindow)
    {
        LibraryGeneratowWindow->setWindowTitle(QCoreApplication::translate("LibraryGeneratowWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LibraryGeneratowWindow: public Ui_LibraryGeneratowWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIBRARYGENERATORWINDOW_H
