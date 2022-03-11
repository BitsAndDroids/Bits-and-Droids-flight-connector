#include "pathhandler.h"

#include <qmessagebox.h>

#include "QApplication"
QString PathHandler::writableEventPath = "";
QString PathHandler::communityFolderPath = "";
QString PathHandler::maintenanceToolPath = "";

QString PathHandler::getWritableEventPath() {
  return PathHandler::writableEventPath;
}

QString PathHandler::getCommunityFolderPath() {
  return PathHandler::communityFolderPath;
}

QString PathHandler::getMaintenanceToolPath() {
  return PathHandler::maintenanceToolPath;
}

void PathHandler::setCommunityFolderPath(const QString &newPath) {
  PathHandler::communityFolderPath = newPath;
}

void PathHandler::setWritableEventPath(const QString &newWritableEventPath) {
  PathHandler::writableEventPath = newWritableEventPath;
}

void PathHandler::setMaintenanceToolPath(
    const QString &newMaintenanceToolPath) {
  PathHandler::maintenanceToolPath = newMaintenanceToolPath;
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

  maintenanceToolPath =
      "\"" + qApp->applicationDirPath() + "/ConnectorMaintenanceTool" + "\"";
}
