#include "set.h"
#include "output.h"


#include <QJsonArray>
#include <QJsonDocument>
#include <utility>

set::set() {

}

set::set(QString setName) {
    this->setName = std::move(setName);

}

set::set(QString setName, int id) {
    this->setName = std::move(setName);
    this->setId = id;
}

set::set(QString setName, int id, QMap<int, Output *> outputs) {
    this->setName = std::move(setName);
    this->setId = id;
    this->outputs = std::move(outputs);
}


void set::addOutput(Output *outputToAdd) {

    outputs.insert(outputToAdd->getId(), outputToAdd);
    qDebug() << outputs;
}

void set::setOutputs(QMap<int, Output *> newOutputs) {
    this->outputs = std::move(newOutputs);
}

void set::clearOutputs() {
    this->outputs.clear();
}

void set::deleteSet() {

}
