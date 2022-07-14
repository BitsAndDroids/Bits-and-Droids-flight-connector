//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOGGERSERVICE_H
#define BITSANDDROIDSCONNECTOR_LOGGERSERVICE_H


#include <QString>

class LoggerService {
private:
public:
    LoggerService();

    ~LoggerService();

    void logError(QString message);

    void logDebug(QString message);

    void logWarning(QString message);
};


#endif //BITSANDDROIDSCONNECTOR_LOGGERSERVICE_H
