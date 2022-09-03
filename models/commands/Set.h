#ifndef SET_H
#define SET_H

#include <qjsonobject.h>

#include <QList>
#include <QMap>
#include <utility>

#include "output.h"

class Set {
 public:
  Set();

  explicit Set(QString setName);

  // void addOutput(Output outputToAdd);

  QMap<int, Output *> getOutputs() { return this->outputs; };

  int getID() const { return setId; };

  QString getSetName() const { return setName; };

  void setOutputs(QMap<int, Output *> newOutputs);

  void setSetName(QString name) { this->setName = std::move(name); };

  void setSetId(int id) { this->setId = id; };

  void deleteSet();

  void saveSet();

  Set(QString setName, int id);

  static Set fromJson(QJsonDocument *docToConvert);

  Set(QString setName, int id, QMap<int, Output *>);

  void addOutput(Output *outputToAdd);

  void clearOutputs();

  void removeOutput(int id);

 private:
  QString setName;
  int setId = 0;
  QMap<int, Output *> outputs = QMap<int, Output *>();
};

#endif  // SET_H
