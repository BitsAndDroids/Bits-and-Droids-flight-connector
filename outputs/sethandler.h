#ifndef SETHANDLER_H
#define SETHANDLER_H

#include <settings/settingshandler.h>

#include <QJsonArray>
#include <QList>
#include <QSettings>
#include <QStandardPaths>

#include "outputhandler.h"
#include "set.h"

class SetHandler {
 public:
  SetHandler();
  bool outputsInitialized = false;
  QList<set> *loadSets();

  set fromJson(QJsonDocument *docToConvert);

  void removeSet(QString id);

  QList<set> *getSets() { return setList; };

  set getSetById(QString id);

  set *saveSet(set *setToSave);

  void updateSets();

 private:
  SettingsHandler settingsHandler;
  outputHandler *outputHandler = new class outputHandler();
  // QList<set*> *savedSets = new QList<set*>();
  QList<set> *setList;

  void removeOutputFromSet(int setId, int outputId);
};

#endif  // SETHANDLER_H
