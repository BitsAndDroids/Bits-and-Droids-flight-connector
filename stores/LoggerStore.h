//
// Created by dave- on 13-7-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOGGERSTORE_H
#define BITSANDDROIDSCONNECTOR_LOGGERSTORE_H


#include <vector>
#include "models/Log.h"

class LoggerStore {
public:
    LoggerStore();
    void storeLog(const Log& log);
    std::vector<Log>* getLogs();
private:
    static std::vector<Log> *logs;

};


#endif //BITSANDDROIDSCONNECTOR_LOGGERSTORE_H
