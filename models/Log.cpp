//
// Created by dave- on 13-7-2022.
//

#include "Log.h"

std::string Log::getMessage() const {
    return this->logMessage;
}

Log::Log(std::string logMessage, LogLevel logLevel, std::time_t logTime) {
    this->logMessage = logMessage;
    this->logLevel = logLevel;
    this->logTime = logTime;
}

std::string Log::getLevelString() const {
    switch (logLevel) {
        case DEBUGLOG:
            return "DEBUG";
        case ERRORLOG:
            return "ERROR";
        case WARNINGLOG:
            return "WARNING";
    }
}

std::string Log::getTimeString() const {
    //convert time_t to string
    std::string timeString = std::ctime(&logTime);
    timeString.erase(timeString.end() - 1);
    return timeString;
}
