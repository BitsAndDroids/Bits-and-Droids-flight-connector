#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include <handlers/pathhandler.h>

#include <QJsonObject>
#include <QMap>

#include "output.h"

class outputHandler {
 public:
  outputHandler();
  QStringList getCategoryStrings() { return categoryStrings; };
  void addCategoryString(QString category);
  QList<QList<Output>> getOutputsCategorized() { return outputsCategorized; };
  QMap<int, Output*> getAvailableOutputs() { return this->availableOutputs; };
  Output* findOutputById(int id);
  void readOutputs();

 private:
  // const QJsonObject &json
  QStringList categoryStrings;
  QList<QList<Output>> outputsCategorized;
  QMap<int, Output*> availableOutputs = QMap<int, Output*>();
  PathHandler pathHandler;
  QString applicationEventsPath = pathHandler.getWritableEventPath();

    void addCategoryString(const QString &category);
};

#endif  // OUTPUTHANDLER_H
