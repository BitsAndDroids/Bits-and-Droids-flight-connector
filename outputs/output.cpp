#include "output.h"

Output::Output(int id, std::string outputName, std::string metric, float updateEvery, int dataType, QString cbText)
{
    this->id = id;
    this->outputName = outputName;
    this->metric = metric;
    this->updateEvery = updateEvery;
    this->dataType = dataType;
    this->cbText = cbText;
}
