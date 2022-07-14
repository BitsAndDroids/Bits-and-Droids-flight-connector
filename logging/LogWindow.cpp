//
// Created by dave- on 30-6-2022.
//

#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include "LogWindow.h"
#include <QHeaderView>
LogWindow::LogWindow() {
    connect(this, &LogWindow::logReceived, this, &LogWindow::addLogRow);
    connect(loggerService, &LoggerService::logReceived, this, &LogWindow::logReceived);

    QFont Triforce("Roboto Black", 11, 900);
    logTable->horizontalHeader()->setFont(Triforce);

    logTable->setColumnCount(3);
    logTable->setHorizontalHeaderLabels(QStringList() << "Level" << "Message" << "Type");

    logTable->setColumnWidth(0, 100);
    logTable->setColumnWidth(1, 300);
    logTable->setColumnWidth(2, 200);
    logTable->setSortingEnabled(true);

    this->setWindowTitle("Logs");
}

void LogWindow::addLogRow(const Log& log) {
    this->logList.push_back(log);

    logTable->setRowCount((int)logList.size());
    logTable->setItem((int)logList.size() - 1, 0, new QTableWidgetItem(QString::fromStdString(log.getLevelString())));
    logTable->setItem((int)logList.size()-1, 1, new QTableWidgetItem(QString::fromStdString(log.getMessage())));
    logTable->setItem((int)logList.size()-1, 2, new QTableWidgetItem(QString::fromStdString(log.getTimeString())));
    std::cout<<log.getMessage()<<std::endl;
}

void LogWindow::openWindow() {

    loadLogs();
    auto widgetLayout = new QVBoxLayout();
    this->setLayout(widgetLayout);
    //this->setFixedWidth(750);
    this->resize(600, 500);
    auto logRowWidget = new QWidget();
    auto logVBoxLayout = new QVBoxLayout();
    logRowWidget->setLayout(logVBoxLayout);



    widgetLayout->addWidget(logTable);

    this->show();



}

LogWindow::~LogWindow() {

}

void LogWindow::loadLogs() {
    LoggerStore loggerStore = LoggerStore();
    std::vector<Log> *logs = loggerStore.getLogs();
    for(auto & log : *logs) {
        this->addLogRow(log);
    }
}


