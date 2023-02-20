#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include "pathhandler.h"
#include "models/Category.h"

#include <QJsonObject>
#include <QMap>

#include <models/commands/output.h>

class OutputHandler {
 public:
  OutputHandler();
  std::vector<QString> getCategoryStrings() { return categoryStrings; };
  void addCategoryString(const QString& category);
  std::vector<Category> getOutputsCategorized() { return outputsCategorized; };
  QMap<int, Output*> getAvailableOutputs() { return availableOutputs; };
  Output* findOutputById(int id);
  void readOutputs();
  void addToEventFileDialog(Output output);

 private:
  static bool updateOutputsRequired;
  static QMap<int, Output*> availableOutputs;
  static std::vector<Category> outputsCategorized;
  static std::vector<QString> categoryStrings;
  // const QJsonObject &json

  PathHandler pathHandler;
  QString applicationEventsPath = pathHandler.getWritableEventPath();
};

#endif  // OUTPUTHANDLER_H
