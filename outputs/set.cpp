#include "set.h"
#include "output.h"


#include <QJsonArray>
#include <QJsonDocument>

set::set() {

}

set::set(QString setName) {
    this->setName = setName;

}

set::set(QString setName, int id) {
    this->setName = setName;
    this->setId = id;
}

set::set(QString setName, int id, QMap<int, Output *> outputs) {
    this->setName = setName;
    this->setId = id;
    this->outputs = outputs;
}


void set::addOutput(Output *outputToAdd) {

    outputs.insert(outputToAdd->getId(), outputToAdd);
    qDebug() << outputs;
}

void set::setOutputs(QMap<int, Output *> newOutputs) {
    this->outputs = newOutputs;
}

void set::clearOutputs() {
    this->outputs.clear();
}

void set::deleteSet() {

}
