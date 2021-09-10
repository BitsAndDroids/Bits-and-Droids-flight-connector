#ifndef SETTINGSRANGES_H
#define SETTINGSRANGES_H

#include <qboxlayout.h>

#include <QStringList>

class SettingsRanges {
 public:
  SettingsRanges();
  SettingsRanges(int amntOfRanges, QStringList labels, QString header);
  QVBoxLayout* CreateRangeRow();
  QVBoxLayout* createRudderRow();

 private:
  QStringList labels;
  QString headerText;
  int amntOfRanges;
};

#endif  // SETTINGSRANGES_H
