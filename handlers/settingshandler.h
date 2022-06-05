#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <qstandardpaths.h>

#include <QObject>
#include <QSettings>

class SettingsHandler {
public:
    SettingsHandler();

    void storeValue(QString group, QString key, QVariant value);

    QVariant *retrieveSetting(QString group, QString key);

    QStringList *retrieveKeys(QString group);

    void clearKeys(QString group);

    void removeSetting(QString group, QString key);

    QString getPath() { return path; };

    void storeSubGroup(QString group, QString subGroup, QString key, QVariant value);

    QStringList *retrieveSubKeys(QString group, QString subGroup);

    QVariant *retrieveSubSetting(QString group, QString subGroup, QString key);

    void checkEventFilePresent();
private:
    QString path =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings *settings =
            new QSettings(QSettings::IniFormat, QSettings::UserScope,
                          "Bits and Droids", "settings");



};

#endif  // SETTINGSHANDLER_H
