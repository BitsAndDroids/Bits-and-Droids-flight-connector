#include "output.h"
#include "outputmapper.h"

#include <tchar.h>
#include <windows.h>
#include <headers/SimConnect.h>
outputMapper::outputMapper()
{

}
void outputMapper::mapOutputs(QList<Output> outputToMap,HANDLE outputConnect){


    if (SUCCEEDED(
            SimConnect_Open(&outputConnect, "Tagged Data", NULL, 0, nullptr, 0))) {
        for(int i = 0; i < outputToMap.size(); i++){
            SimConnect_AddToDataDefinition(
                      outputConnect, 1, outputToMap[i].getOutputName().c_str(), outputToMap[i].getMetric().c_str(),
                      SIMCONNECT_DATATYPE_FLOAT32, outputToMap[i].getUpdateEvery(), outputToMap[i].getId());
                }
        }

   }

