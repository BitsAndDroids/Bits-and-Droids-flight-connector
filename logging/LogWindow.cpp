//
// Created by dave- on 30-6-2022.
//

#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include "LogWindow.h"
#include <QHeaderView>
#include <QTimer>
LogWindow::LogWindow() {
    //connect(this, &LogWindow::logReceived, this, &LogWindow::addLogRow);
    connect(loggerService, &LoggerService::logReceived, this, &LogWindow::addLogRow);

    QFont Triforce("Roboto Black", 11, 900);
    logTable->horizontalHeader()->setFont(Triforce);

    logTable->setColumnCount(3);
    logTable->setHorizontalHeaderLabels(QStringList() << "Level" << "Message" << "Time");

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

    logTable->scrollToBottom();
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
    const clock_t begin_time = clock();
    logTable->setRowCount((int)logs->size());
    for(int i = 0; i < logs->size(); i++) {
        logTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(logs->at(i).getLevelString())));
        logTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(logs->at(i).getMessage())));
        logTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(logs->at(i).getTimeString())));
    }
    const clock_t totalTime = clock() - begin_time;
    logTable->scrollToBottom();
    std::cout<<"LOADING ROWS TOOK "<< totalTime<<std::endl;
}


