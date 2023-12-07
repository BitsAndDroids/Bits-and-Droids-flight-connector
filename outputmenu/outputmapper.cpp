#include "outputmapper.h"

#include <windows.h>

#include <iostream>

#include "enums/ModeEnum.h"
#include "enums/OutputModesEnum.h"

outputMapper::outputMapper() {
}

void outputMapper::mapOutputs(QList<Output *> outputToMap,
                              HANDLE outputConnect) {
    int offsetCounter = 0;
    for (auto&i: outputToMap) {
        if (i->getType() == 99 || i->getType() == 98 || i->getType() == 97) {
            SimConnect_AddToClientDataDefinition(outputConnect, i->getPrefix(),
                                                 i->getOffset(), sizeof(float),
                                                 i->getUpdateEvery(), 0);
            SimConnect_RequestClientData(
                outputConnect, 2, i->getPrefix(), i->getPrefix(),
                SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET,
                SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
            offsetCounter++;
        }
        else {
            //String output
            if (i->getType() == ModeEnum::STRINGMODE) {
                SimConnect_AddToDataDefinition(
                    outputConnect, 1, i->getOutputName().c_str(), i->getMetric().c_str(),
                    SIMCONNECT_DATATYPE_STRING256, i->getUpdateEvery(), i->getId());
                continue;
            }
            SimConnect_AddToDataDefinition(
                outputConnect, 0, i->getOutputName().c_str(), i->getMetric().c_str(),
                SIMCONNECT_DATATYPE_FLOAT32, i->getUpdateEvery(), i->getId());
        }
    }
}
