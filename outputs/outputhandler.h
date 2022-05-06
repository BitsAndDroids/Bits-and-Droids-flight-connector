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
  void addCategoryString(const QString& category);
  QList<QList<Output>> getOutputsCategorized() { return outputsCategorized; };
  QMap<int, Output*> getAvailableOutputs() { return availableOutputs; };
  Output* findOutputById(int id);
  void readOutputs();

 private:
  static bool updateOutputsRequired;
  static QMap<int, Output*> availableOutputs;
  static QList<QList<Output>> outputsCategorized;
  static QStringList categoryStrings;
  // const QJsonObject &json

  PathHandler pathHandler;
  QString applicationEventsPath = pathHandler.getWritableEventPath();

    void addCategoryString(const QString &category);
};

#endif  // OUTPUTHANDLER_H
