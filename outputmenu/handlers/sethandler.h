#ifndef SETHANDLER_H
#define SETHANDLER_H



#include <QJsonArray>
#include <QList>
#include <QSettings>
#include <QStandardPaths>
#include "settings/settingshandler.h"
#include <handlers/outputhandler.h>
#include <models/commands/Set.h>

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

  void exportSet(const QString& id, const QString& fileName);

  int importSet(const QString& filePath, const QString& name);

 private:

  SettingsHandler settingsHandler;
  outputHandler *outputHandler = new class outputHandler();
  // QList<Set*> *savedSets = new QList<Set*>();
  QList<Set> *setList;

  void removeOutputFromSet(int setId, int outputId);

    QJsonDocument* setToJSON(Set *setToConvert);
};

#endif  // SETHANDLER_H
