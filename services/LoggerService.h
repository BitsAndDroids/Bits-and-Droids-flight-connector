//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOGGERSERVICE_H
#define BITSANDDROIDSCONNECTOR_LOGGERSERVICE_H

#include <QWidget>
#include <QString>
#include "models/Log.h"
#include "stores/LoggerStore.h"

class LoggerService: public QObject {
Q_OBJECT
private:
    void writeToLogFile(const Log& log);
    LoggerStore loggerStore = LoggerStore();

public:

    ~LoggerService();

    void logError(std::string message);

    void logDebug(std::string message);

    void logWarning(std::string message);

    LoggerService(LoggerService &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const LoggerService &) = delete;

    static LoggerService *getInstance();

    signals:
    void logReceived(const Log& log);

protected:
    LoggerService(){};
    static LoggerService* instance;

};
#endif //BITSANDDROIDSCONNECTOR_LOGGERSERVICE_H
