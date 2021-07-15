#include "outputmapper.h"

#include <headers/SimConnect.h>
#include <tchar.h>
#include <windows.h>

#include <iostream>

#include "output.h"

outputMapper::outputMapper() {}
void outputMapper::mapOutputs(QList<Output*> outputToMap,
                              HANDLE outputConnect) {
  HRESULT hr;
  for (int i = 0; i < outputToMap.size(); i++) {
    hr = SimConnect_AddToDataDefinition(
        outputConnect, 0, outputToMap[i]->getOutputName().c_str(),
        outputToMap[i]->getMetric().c_str(), SIMCONNECT_DATATYPE_FLOAT32,
        outputToMap[i]->getUpdateEvery(), outputToMap[i]->getId());
    std::cout << outputToMap[i]->getId() << std::endl;
  }
}
