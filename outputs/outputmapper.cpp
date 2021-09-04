#include "outputmapper.h"

#include <windows.h>

#include <iostream>

outputMapper::outputMapper() {}
void outputMapper::mapOutputs(QList<Output*> outputToMap,
                              HANDLE outputConnect) {
  HRESULT hr;
  for (auto& i : outputToMap) {
    hr = SimConnect_AddToDataDefinition(
        outputConnect, 0, i->getOutputName().c_str(), i->getMetric().c_str(),
        SIMCONNECT_DATATYPE_FLOAT32, i->getUpdateEvery(), i->getId());

    std::cout << i->getId() << std::endl;
  }
}
