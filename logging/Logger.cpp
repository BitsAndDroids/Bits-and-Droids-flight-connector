//
// Created by dave- on 13-7-2022.
//

#include "Logger.h"
#include <utility>
#include "LogWindow.h"

Logger *Logger::getInstance()
{
    if(instance==nullptr){
        instance = new Logger();
    }
    return instance;
}

void Logger::logError(std::string message) {
    Log log = Log(std::move(message), LogLevel::ERRORLOG, std::time(nullptr));
    writeToLogFile(log);
}

void Logger::logDebug(std::string message) {
    Log log = Log(std::move(message), LogLevel::DEBUGLOG, std::time(nullptr));
    writeToLogFile(log);
}

void Logger::logWarning(std::string message) {
    Log log = Log(std::move(message), LogLevel::WARNINGLOG, std::time(nullptr));
    writeToLogFile(log);
}

void Logger::writeToLogFile(const Log& log) {
    loggerStore.storeLog(log);

    emit logReceived(log);
}

