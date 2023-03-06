//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOGGER_H
#define BITSANDDROIDSCONNECTOR_LOGGER_H

#include <QWidget>
#include <QString>
#include "models/Log.h"
#include "stores/LoggerStore.h"

class Logger: public QObject {
Q_OBJECT
private:
    void writeToLogFile(const Log& log);
    LoggerStore loggerStore = LoggerStore();
    Logger(){};
public:

    void logError(std::string message);

    void logDebug(std::string message);

    void logWarning(std::string message);

    Logger(Logger &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const Logger &) = delete;

    static Logger *getInstance();

    signals:
    void logReceived(const Log& log);

protected:

    static Logger* instance;

};
#endif //BITSANDDROIDSCONNECTOR_LOGGER_H
