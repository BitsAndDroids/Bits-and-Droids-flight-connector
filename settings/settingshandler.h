#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QObject>
#include <QSettings>
#include <qstandardpaths.h>

class SettingsHandler
{

public:
    SettingsHandler();
    void storeQStringToGroup(QString group, QString key, QString value);
private:
    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings *settings =
        new QSettings(path +"/"+ "settings.ini", QSettings::IniFormat);
};

#endif // SETTINGSHANDLER_H
