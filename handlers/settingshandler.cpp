#include "settingshandler.h"

#include <qdir.h>
#include <qfile.h>

#include <QApplication>
#include <utility>

SettingsHandler::SettingsHandler() {}

void SettingsHandler::storeValue(QString group, QString key, QVariant value) {
  settings->beginGroup(group);
  settings->setValue(key, value);
  settings->endGroup();
  settings->sync();
}

void SettingsHandler::checkEventFilePresent() {
  if (!QFile::exists(path + "/events.txt")) {
    QString applicationEventsPath =
        qApp->applicationDirPath() + "/BitsAndDroidsModule/modules/events.txt";

    if (!QDir(path).exists()) {
      QDir().mkdir(path);
    }
    QFile::copy(applicationEventsPath, path + "/events.txt");
  }
}

void SettingsHandler::storeSubGroup(QString group, QString subGroup,
                                    QString key, QVariant value) {
  settings->beginGroup(group);
  storeValue(std::move(subGroup), std::move(key), std::move(value));
  settings->endGroup();
}
QStringList* SettingsHandler::retrieveSubKeys(QString group, QString subGroup) {
  settings->beginGroup(group);
  QStringList* keys = retrieveKeys(subGroup);
  settings->endGroup();
  return keys;
}

QVariant* SettingsHandler::retrieveSetting(const QString& group, const QString& key) {
  auto* value = new QVariant();
  settings->beginGroup(group);
  *value = settings->value(key);
  settings->endGroup();
  return value;
}
QVariant* SettingsHandler::retrieveSubSetting(QString group, QString subGroup,
                                              QString key) {
  settings->beginGroup(group);
  QVariant* value = retrieveSetting(std::move(subGroup), std::move(key));
  settings->endGroup();
  return value;
}

void SettingsHandler::removeSetting(QString group, QString key) {
  settings->beginGroup(group);
  settings->remove(key);
  settings->endGroup();
  settings->sync();
}
QStringList* SettingsHandler::retrieveKeys(QString group) {
  QStringList* keys = new QStringList();
  settings->beginGroup(group);
  *keys = settings->childKeys();
  settings->endGroup();
  return keys;
}

void SettingsHandler::clearKeys(QString group) {
  settings->beginGroup(group);
  QStringList keys = settings->childKeys();
  for (int i = 0; i < keys.size(); i++) {
    settings->remove(keys[i]);
  }
  settings->endGroup();
}
