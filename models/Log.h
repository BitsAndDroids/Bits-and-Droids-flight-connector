//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOG_H
#define BITSANDDROIDSCONNECTOR_LOG_H


#include <string>
#include <ctime>
#include <iostream>
#include <enums/LogLevelEnum.h>

class Log {
private:
    std::string logMessage;
    LogLevel logLevel;
    std::time_t logTime;

public:
    Log(std::string logMessage, LogLevel logLevel, std::time_t logTime);
    std::string getMessage() const;
    std::string getLevelString() const;
    std::string getTimeString() const;
};


#endif //BITSANDDROIDSCONNECTOR_LOG_H
