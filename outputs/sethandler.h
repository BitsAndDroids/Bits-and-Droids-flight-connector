#ifndef SETHANDLER_H
#define SETHANDLER_H

#include "set.h"

#include <QJsonArray>
#include <QList>
#include <QSettings>
#include <QStandardPaths>



class SetHandler
{
public:
    SetHandler();
    void saveSet(set setToSave);
    QList<set> loadSets();

    set fromJson(QJsonDocument *docToConvert);
    void removeSet(QString id);
    QList<set> getSets(){return setList;};
    set getSetById(QString id);

private:
    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings *settings =
        new QSettings(path + "/Bits and Droids/settings.ini", QSettings::IniFormat);
    //QList<set*> *savedSets = new QList<set*>();
    QList<set> setList;

};

#endif // SETHANDLER_H
