/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *menuItemOptions;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *labelLastVal;
    QPushButton *updateButton;
    QWidget *messagesWidget;
    QHBoxLayout *messagesWidgetLayout;
    QLabel *labelLastVal_2;
    QLabel *labelLastStatus;
    QWidget *connectionsVWidget;
    QHBoxLayout *connectionsContainerLayout;
    QWidget *inWidgetContainer;
    QVBoxLayout *inLayoutContainer;
    QWidget *outWidgetContainer;
    QVBoxLayout *outLayoutContainer;
    QWidget *dualWidgetContainer;
    QVBoxLayout *dualLayoutContainer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1058, 810);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("QApp{\n"
"	\n"
"}\n"
"#connectionPage{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QWidget#MainWindow{\n"
"\n"
"}\n"
"QWidget#centralwidget{\n"
"	background-color: #ebebeb;\n"
"}\n"
"QWidget#stackedWidget{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"#page{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"#Input{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"#Output{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QTabWidget::tab-bar {\n"
"\n"
"}\n"
"QFrame{\n"
"   border-color:#000;\n"
"}\n"
"QPushButton{\n"
"	background-color: #0f4c5c;\n"
"	color: white;\n"
"	border-radius: 4px;\n"
"  \n"
"}\n"
"QPushButton#updateButton{\n"
"	\n"
"	padding: 4px;\n"
"	\n"
"}\n"
"QPushButton#switchButton{\n"
"\n"
"padding: 8px;\n"
"\n"
"}\n"
"QPushButton#startButton, QPushButton#startInputButton{\n"
"	background-color: rgb(1, 150, 11);\n"
"}\n"
"QPushButton#startButton:disabled, QPushButton#startInputButton:disabled{\n"
"	background-color: rgb(9, 42, 0);\n"
"}\n"
"\n"
"QPushButton#stopButton, Q"
                        "PushButton#stopInputButton{\n"
"	background-color: rgb(167, 13, 13);\n"
"\n"
"}\n"
"QTabWidget::pane { /* The tab widget frame */\n"
"    border-top: 2px solid #C2C7CB;\n"
"    position: absolute;\n"
"    top: -0.5em;\n"
"}\n"
"/* Style the tab using the tab sub-control. Note that\n"
"    it reads QTabBar _not_ QTabWidget */\n"
"QTabBar::tab {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
"                                stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
"    border: 2px solid #C4C4C3;\n"
"    border-bottom-color: #C2C7CB; /* same as the pane color */\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"    min-width: 8ex;\n"
"    padding: 2px;\n"
"}\n"
"\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #fafafa, stop: 0.4 #f4f4f4,\n"
"                                stop: 0.5 #e7e"
                        "7e7, stop: 1.0 #fafafa);\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    border-color: #9B9B9B;\n"
"    border-bottom-color: #C2C7CB; /* same as pane color */\n"
"}\n"
"QCheckBox{\n"
"	color: #000;\n"
"}\n"
""));
        menuItemOptions = new QAction(MainWindow);
        menuItemOptions->setObjectName(QString::fromUtf8("menuItemOptions"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelLastVal = new QLabel(centralwidget);
        labelLastVal->setObjectName(QString::fromUtf8("labelLastVal"));

        gridLayout->addWidget(labelLastVal, 9, 0, 1, 1);

        updateButton = new QPushButton(centralwidget);
        updateButton->setObjectName(QString::fromUtf8("updateButton"));
        updateButton->setEnabled(true);
        updateButton->setMaximumSize(QSize(250, 16777215));
        updateButton->setStyleSheet(QString::fromUtf8(":hover{\n"
"	background-color:#0f1d33;\n"
"}"));

        gridLayout->addWidget(updateButton, 0, 0, 1, 1);

        messagesWidget = new QWidget(centralwidget);
        messagesWidget->setObjectName(QString::fromUtf8("messagesWidget"));
        messagesWidgetLayout = new QHBoxLayout(messagesWidget);
        messagesWidgetLayout->setObjectName(QString::fromUtf8("messagesWidgetLayout"));
        messagesWidgetLayout->setContentsMargins(10, 10, -1, -1);
        labelLastVal_2 = new QLabel(messagesWidget);
        labelLastVal_2->setObjectName(QString::fromUtf8("labelLastVal_2"));

        messagesWidgetLayout->addWidget(labelLastVal_2);

        labelLastStatus = new QLabel(messagesWidget);
        labelLastStatus->setObjectName(QString::fromUtf8("labelLastStatus"));

        messagesWidgetLayout->addWidget(labelLastStatus);


        gridLayout->addWidget(messagesWidget, 19, 0, 1, 1);

        connectionsVWidget = new QWidget(centralwidget);
        connectionsVWidget->setObjectName(QString::fromUtf8("connectionsVWidget"));
        connectionsVWidget->setStyleSheet(QString::fromUtf8(""));
        connectionsContainerLayout = new QHBoxLayout(connectionsVWidget);
        connectionsContainerLayout->setObjectName(QString::fromUtf8("connectionsContainerLayout"));
        inWidgetContainer = new QWidget(connectionsVWidget);
        inWidgetContainer->setObjectName(QString::fromUtf8("inWidgetContainer"));
        inWidgetContainer->setStyleSheet(QString::fromUtf8("#inWidgetContainer{\n"
"background-color:#fff;\n"
"border-radius:4px;\n"
"}"));
        inLayoutContainer = new QVBoxLayout(inWidgetContainer);
        inLayoutContainer->setObjectName(QString::fromUtf8("inLayoutContainer"));

        connectionsContainerLayout->addWidget(inWidgetContainer);

        outWidgetContainer = new QWidget(connectionsVWidget);
        outWidgetContainer->setObjectName(QString::fromUtf8("outWidgetContainer"));
        outWidgetContainer->setStyleSheet(QString::fromUtf8("#outWidgetContainer{\n"
"background-color:#fff;\n"
"border-radius:4px;\n"
"}"));
        outLayoutContainer = new QVBoxLayout(outWidgetContainer);
        outLayoutContainer->setObjectName(QString::fromUtf8("outLayoutContainer"));

        connectionsContainerLayout->addWidget(outWidgetContainer);

        dualWidgetContainer = new QWidget(connectionsVWidget);
        dualWidgetContainer->setObjectName(QString::fromUtf8("dualWidgetContainer"));
        dualWidgetContainer->setStyleSheet(QString::fromUtf8("#dualWidgetContainer{\n"
"background-color:#fff;\n"
"border-radius:4px;\n"
"}"));
        dualLayoutContainer = new QVBoxLayout(dualWidgetContainer);
        dualLayoutContainer->setObjectName(QString::fromUtf8("dualLayoutContainer"));

        connectionsContainerLayout->addWidget(dualWidgetContainer);


        gridLayout->addWidget(connectionsVWidget, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1058, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Bits and droids connector", nullptr));
        menuItemOptions->setText(QCoreApplication::translate("MainWindow", "Options", nullptr));
        labelLastVal->setText(QString());
        updateButton->setText(QCoreApplication::translate("MainWindow", "There is no update avaialable", nullptr));
        labelLastVal_2->setText(QString());
        labelLastStatus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
