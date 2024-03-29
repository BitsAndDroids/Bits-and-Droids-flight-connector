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


    QFont Triforce("Roboto Black", 11, 900);
    logTable->horizontalHeader()->setFont(Triforce);

    logTable->setColumnCount(3);
    logTable->setHorizontalHeaderLabels(QStringList() << "Level" << "Message" << "Time");

    this->logTable->setColumnWidth(0, 100);
    this->logTable->setColumnWidth(1, 300);
    this->logTable->setColumnWidth(2, 200);
    this->logTable->setSortingEnabled(true);

    auto widgetLayout = new QVBoxLayout();
    this->setLayout(widgetLayout);
    this->resize(600, 500);
    auto logRowWidget = new QWidget();
    auto logVBoxLayout = new QVBoxLayout();
    logRowWidget->setLayout(logVBoxLayout);
    widgetLayout->addWidget(logTable);
    loadLogs();

    this->setWindowTitle("Logs");
}

void LogWindow::addLogRow(const Log& log) {
    this->logList->push_back(log);
    this->logTable->setRowCount((int)logList->size());
    this->logTable->setItem((int)logList->size() - 1, 0, new QTableWidgetItem(QString::fromStdString(log.getLevelString())));
    this->logTable->setItem((int)logList->size()-1, 1, new QTableWidgetItem(QString::fromStdString(log.getMessage())));
    this->logTable->setItem((int)logList->size()-1, 2, new QTableWidgetItem(QString::fromStdString(log.getTimeString())));
    this->logTable->scrollToBottom();
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
}


