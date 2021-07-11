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

QJsonObject Output::toJson() const
{
    QJsonObject jsonOutput{
        {"id", this->id},
        {"outputName", this->outputName.c_str()},
        {"metric", this->metric.c_str()},
        {"updateEvery", this->updateEvery},
        {"dataType", this->dataType},
        {"cbText", this->cbText.toStdString().c_str()}
    };
    return jsonOutput;
}
