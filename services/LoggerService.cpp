//
// Created by dave- on 13-7-2022.
//

#include "LoggerService.h"
#include <utility>
#include <logging/LogWindow.h>

LoggerService::LoggerService() {
    logError("ERROR TEST");
    logDebug("DEBUG TEST");
    logWarning("WARNING TEST");
}

LoggerService::~LoggerService() {

}

void LoggerService::logError(std::string message) {
    Log log = Log(std::move(message), LogLevel::ERRORLOG, std::time(nullptr));
    writeToLogFile(log);
}

void LoggerService::logDebug(std::string message) {
    Log log = Log(std::move(message), LogLevel::DEBUGLOG, std::time(nullptr));
    writeToLogFile(log);
}

void LoggerService::logWarning(std::string message) {
    Log log = Log(std::move(message), LogLevel::WARNINGLOG, std::time(nullptr));
    writeToLogFile(log);
}

void LoggerService::writeToLogFile(const Log& log) {
    loggerStore.storeLog(log);

    emit logReceived(log);
}

