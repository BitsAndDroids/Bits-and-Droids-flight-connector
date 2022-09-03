#include "Set.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <utility>

#include "output.h"

Set::Set() {}

Set::Set(QString setName) { this->setName = std::move(setName); }

Set::Set(QString setName, int id) {
  this->setName = std::move(setName);
  this->setId = id;
}

Set::Set(QString setName, int id, QMap<int, Output *> outputs) {
  this->setName = std::move(setName);
  this->setId = id;
  this->outputs = std::move(outputs);
}

void Set::addOutput(Output *outputToAdd) {
  outputs.insert(outputToAdd->getId(), outputToAdd);
  qDebug() << outputs;
}
void Set::removeOutput(int id) { outputs.remove(id); }
void Set::setOutputs(QMap<int, Output *> newOutputs) {
  this->outputs = newOutputs;
}

void Set::clearOutputs() { this->outputs.clear(); }

void Set::deleteSet() {}
