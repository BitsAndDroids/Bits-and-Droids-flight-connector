#include "settingshandler.h"

SettingsHandler::SettingsHandler() {}

void SettingsHandler::storeValue(QString group, QString key, QVariant value) {
  settings->beginGroup(group);
  settings->setValue(key, value);
  settings->endGroup();
  settings->sync();
}

QVariant* SettingsHandler::retrieveSetting(QString group, QString key) {
  QVariant* value = new QVariant();
  settings->beginGroup(group);
  *value = settings->value(key);
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
