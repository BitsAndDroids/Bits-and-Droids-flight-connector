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

QString PathHandler::getApplicationExecutablePath() {
  return qApp->applicationDirPath() + "/BitsAndDroidsConnector.exe";
}

PathHandler::PathHandler() {
  setWritableEventPath(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      "/events.txt");

  if (!settingsHandler.getSettingValue("Settings", "communityFolderPathLabel")
           ->isNull()) {
    QString pathFound =
        settingsHandler.getSettingValue("Settings", "communityFolderPathLabel")
            ->toString();
    setCommunityFolderPath(pathFound);
  }
  //By removing /bin from the path we ensure we get the correct path
  // to the maintenance tool in a folder up from the application
  QString pathCut = qApp->applicationDirPath();
  pathCut.remove("/bin");
  maintenanceToolPath =
      "\"" + pathCut + "/ConnectorMaintenanceTool" + "\"";
}
