#ifndef SETHANDLER_H
#define SETHANDLER_H

#include "handlers/settingshandler.h"

#include <QJsonArray>
#include <QList>
#include <QSettings>
#include <QStandardPaths>

#include "handlers/outputhandler.h"
#include "models/commands/Set.h"

class SetHandler {
 public:
  SetHandler();
  bool outputsInitialized = false;
  QList<Set> *loadSets();

  Set fromJson(QJsonDocument *docToConvert);

  void removeSet(QString id);

  QList<Set> *getSets() { return setList; };

  Set getSetById(QString id);

  Set *saveSet(Set *setToSave);

  void updateSets();

  void exportSet(QString id);

 private:
  SettingsHandler settingsHandler;
  outputHandler *outputHandler = new class outputHandler();
  // QList<Set*> *savedSets = new QList<Set*>();
  QList<Set> *setList;

  void removeOutputFromSet(int setId, int outputId);
};

#endif  // SETHANDLER_H
