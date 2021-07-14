#ifndef SETHANDLER_H
#define SETHANDLER_H

#include <settings/settingshandler.h>

#include <QJsonArray>
#include <QList>
#include <QSettings>
#include <QStandardPaths>

#include "set.h"

class SetHandler {
 public:
  SetHandler();

  QList<set> *loadSets();

  set fromJson(QJsonDocument *docToConvert);
  void removeSet(QString id);
  QList<set> *getSets() { return setList; };
  set getSetById(QString id);

  set *saveSet(set *setToSave);

 private:
  SettingsHandler settingsHandler;

  // QList<set*> *savedSets = new QList<set*>();
  QList<set> *setList;
};

#endif  // SETHANDLER_H
