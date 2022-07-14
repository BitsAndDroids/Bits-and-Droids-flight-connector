//
// Created by dave- on 13-7-2022.
//

#include "LoggerStore.h"
std::vector<Log>* LoggerStore::logs = new std::vector<Log>();

LoggerStore::LoggerStore() {

}

void LoggerStore::storeLog(const Log& log) {
    LoggerStore::logs->push_back(log);
}

std::vector<Log> *LoggerStore::getLogs() {
    return LoggerStore::logs;
}
