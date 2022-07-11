//
// Created by dave- on 30-6-2022.
//

#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include "LogWindow.h"
LogWindow::LogWindow(QWidget *parent): QWidget(parent) {
    logList.append("Test");


    auto* widget = new QWidget();
    auto widgetLayout = new QVBoxLayout();
    widget->setLayout(widgetLayout);

    auto logWindowHeader = new QLabel("LOGS");
    widgetLayout->addWidget(logWindowHeader);


    auto scrollWindow = new QScrollArea();
    auto logRowWidget = new QWidget();
    auto logVBoxLayout = new QVBoxLayout();
    logRowWidget->setLayout(logVBoxLayout);

    widgetLayout->addWidget(scrollWindow);


    scrollWindow->setMinimumHeight(250);
    scrollWindow->setMaximumHeight(250);

    widget->adjustSize();
    widget->show();
}

void LogWindow::addLogRow(QString) {

}

LogWindow::~LogWindow() {

}


