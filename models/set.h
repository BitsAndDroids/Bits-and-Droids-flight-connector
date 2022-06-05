#ifndef SET_H
#define SET_H

#include <qstring.h>

#include <QSettings>
#include <QStandardPaths>

class Set {
 public:
  int id;
  Set(int index, bool newSetReceived, QString nameReceived) {
    id = index;
    newSet = newSetReceived;
    name = nameReceived;
  };
  void setAmntSets(int newAmntSets) { amntSets = newAmntSets; };
  QString setName;
  void createSet();
  void updateSet();
  void addCheckBox(QString cb);
 public slots:

 private:
  QString name;
  QStringList checkboxes;
  bool newSet;
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings *settings =
      new QSettings(path + "/" + "settings.ini", QSettings::IniFormat);
  int amntSets;
};

#endif  // SET_H
