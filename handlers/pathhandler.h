#ifndef PATHHANDLER_H
#define PATHHANDLER_H

#include <qstandardpaths.h>
#include <qstring.h>
#include "settingshandler.h"

class PathHandler {
 public:
  PathHandler();
  QString getWritableEventPath();
  QString getCommunityFolderPath();
  QString getMaintenanceToolPath();

  static void setWritableEventPath(const QString &newWritableEventPath);
  static void setCommunityFolderPath(const QString &newPath);
  static void setMaintenanceToolPath(const QString &maintenanceToolPath);

 private:
  SettingsHandler settingsHandler;
  static QString writableEventPath;
  static QString maintenanceToolPath;
  static QString communityFolderPath;
};

#endif  // PATHHANDLER_H
