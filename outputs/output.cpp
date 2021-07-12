#include "output.h"

Output::Output(int id, std::string outputName, std::string metric, float updateEvery, int dataType, QString cbText, int prefix, int type)
{
    this->id = id;
    this->outputName = outputName;
    this->metric = metric;
    this->updateEvery = updateEvery;
    this->dataType = dataType;
    this->cbText = cbText;
    this->type = type;
    this->prefix = prefix;
}

QJsonObject Output::toJson() const
{
    QJsonObject jsonOutput{
        {"id", this->id},
        {"outputName", this->outputName.c_str()},
        {"metric", this->metric.c_str()},
        {"updateEvery", this->updateEvery},
        {"dataType", this->dataType},
        {"cbText", this->cbText.toStdString().c_str()},
        {"prefix", this->prefix},
        {"type", this->type}
    };
    return jsonOutput;
}
