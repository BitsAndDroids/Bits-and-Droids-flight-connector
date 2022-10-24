//
// Created by DaveRiedel on 24-10-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_SERVICESETTINGSHANDLER_H
#define BITSANDDROIDSCONNECTOR_SERVICESETTINGSHANDLER_H


#include "settingshandler.h"

class ServiceSettingsHandler: public SettingsHandler {
public:
    ServiceSettingsHandler();

    QList<QPair<QString, int>> loadComs();

    void saveComs(QList<QPair<QString, int>> coms);

    void adjustIndexes();

    void removeComRowSettings(QString index);

    QString getCommunityFolderPath();

    bool searchDefaultCommunityLocation();

    bool getWASMModuleInstalled();

private:

};



#endif //BITSANDDROIDSCONNECTOR_SERVICESETTINGSHANDLER_H
