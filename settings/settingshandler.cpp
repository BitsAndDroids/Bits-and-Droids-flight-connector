#include "settingshandler.h"
#include "settings/migrations/Migration.h"
#include "settings/migrations/2022092601.h"
#include <qdir.h>
#include <qfile.h>
#include <QApplication>
#include <utility>

SettingsHandler::SettingsHandler() {

}

void SettingsHandler::storeSettingValue(QString group, QString key, QVariant value) {
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
    storeSettingValue(std::move(subGroup), std::move(key), std::move(value));
    settings->endGroup();
}

QStringList *SettingsHandler::retrieveSubKeys(QString group, QString subGroup) {
    settings->beginGroup(group);
    QStringList *keys = retrieveKeys(subGroup);
    settings->endGroup();
    return keys;
}

QVariant *SettingsHandler::getSettingValue(const QString &group, const QString &key) {
    auto *value = new QVariant();
    settings->beginGroup(group);
    *value = settings->value(key);
    settings->endGroup();
    return value;
}

QVariantList *SettingsHandler::getGroup(QString group) {
    settings->beginGroup(group);
    QStringList keys = settings->childKeys();
    auto *list = new QVariantList();
    for (const auto & key : keys) {
        list->append(settings->value(key));
    }
    return list;
}

QVariant *SettingsHandler::retrieveSubSetting(QString group, QString subGroup,
                                              QString key) {
    settings->beginGroup(group);
    QVariant *value = getSettingValue(std::move(subGroup), std::move(key));
    settings->endGroup();
    return value;
}

void SettingsHandler::removeSetting(QString group, QString key) {
    settings->beginGroup(group);
    settings->remove(key);
    settings->endGroup();
    settings->sync();
}

QStringList *SettingsHandler::retrieveKeys(QString group) {
    QStringList *keys = new QStringList();
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

/*!
\fn void SettingsHandler::migrate()
\brief Migrates the settings to the latest version
 When we make changes to the connector there is a chance that the settings have to be altered as well.
 In order to reduce the risk of breaking the settings we migrate the settings to the latest version.
 This function will migrate the settings to the latest version. It will do this by
 checking the version of the settings and then running the migrations that are
 required to get to the latest version.

 The ID of the migration is the creation date of the migration( in the format
 YYYYMMDDVV where YYYY is the year, MM is the month, DD is the day and VV is the version
*/
void SettingsHandler::migrate() {
    auto migrations = new QList<Migration *>();

    //LIST OF AVAILABLE MIGRATIONS
    Migration *migration2022092601 = new Migration2022092601(2022092601);
    migrations->append(migration2022092601);

    for (auto &migration: *migrations) {
        if (migration->getMigrationId() > getSettingValue("migrations", "id")->toInt()) {
            migration->migrate();
            storeSettingValue("migrations", "id", QString::number(migration->getMigrationId()));
            storeSettingValue("migrations", "date", QDateTime::currentDateTime().toString());
        }
    }
}
