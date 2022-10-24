//
// Created by DaveRiedel on 24-10-2022.
//

#include <QDir>
#include "ServiceSettingsHandler.h"
ServiceSettingsHandler::ServiceSettingsHandler() {

}
QString ServiceSettingsHandler::getCommunityFolderPath() {
    return retrieveSetting("Settings","communityFolderPathLabel")->toString();
}

bool ServiceSettingsHandler::searchDefaultCommunityLocation() {
    QString test = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir dir = QDir(test + R"(\Packages\Microsoft.FlightSimulator_8wekyb3d8bbwe\LocalCache\Packages\Community)");
    if(dir.exists()){
        storeValue("Settings","communityFolderPathLabel",dir.absolutePath());
    }
    return dir.exists();
}
