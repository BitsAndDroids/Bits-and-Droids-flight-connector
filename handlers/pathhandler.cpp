#include "pathhandler.h"

QString PathHandler::writableEventPath = "";
QString PathHandler::communityFolderPath = "";

QString PathHandler::getWritableEventPath() {
  return PathHandler::writableEventPath;
}

QString PathHandler::getCommunityFolderPath() {
  return PathHandler::communityFolderPath;
}

void PathHandler::setCommunityFolderPath(const QString &newPath) {
  PathHandler::communityFolderPath = newPath;
}

void PathHandler::setWritableEventPath(const QString &newWritableEventPath) {
  PathHandler::writableEventPath = newWritableEventPath;
}

PathHandler::PathHandler() {
  setWritableEventPath(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      "/events.txt");
  if (!settingsHandler.retrieveSetting("Settings", "communityFolderPathLabel")
           ->isNull()) {
    QString pathFound =
        settingsHandler.retrieveSetting("Settings", "communityFolderPathLabel")
            ->toString();
    setCommunityFolderPath(pathFound);
  }
}
