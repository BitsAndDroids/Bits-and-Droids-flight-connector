//
// Created by dave- on 30-6-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOGWINDOW_H
#define BITSANDDROIDSCONNECTOR_LOGWINDOW_H


#include <QWidget>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "models/Log.h"
#include "Logger.h"
#include "enums/LogLevelEnum.h"

class LogWindow: public QWidget {
    Q_OBJECT
private:
    std::vector<Log> *logList = new std::vector<Log>();
    QTableWidget *logTable = new QTableWidget();
    void loadLogs();
public:
    LogWindow();
    ~LogWindow();
    void openWindow();
public slots:
    void addLogRow(const Log& log);

};


#endif //BITSANDDROIDSCONNECTOR_LOGWINDOW_H
