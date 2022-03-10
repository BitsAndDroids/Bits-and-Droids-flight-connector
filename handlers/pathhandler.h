#ifndef PATHHANDLER_H
#define PATHHANDLER_H

#include <qstandardpaths.h>
#include <qstring.h>
#include <settings/settingshandler.h>

class PathHandler {
 public:
  PathHandler();
  QString getWritableEventPath();
  QString getCommunityFolderPath();

  static void setWritableEventPath(const QString &newWritableEventPath);
  static void setCommunityFolderPath(const QString &newPath);

 private:
  SettingsHandler settingsHandler;
  static QString writableEventPath;

  static QString communityFolderPath;
};

#endif  // PATHHANDLER_H
