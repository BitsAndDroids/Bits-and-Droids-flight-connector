//
// Created by DaveRiedel on 18-10-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_INSTALLATIONSERVICE_H
#define BITSANDDROIDSCONNECTOR_INSTALLATIONSERVICE_H


class InstallationService {
public:
    InstallationService();

    void installWASMModule();

    bool getUpdatesAvailable();

    static QString getCurrentVersion();

    void writeToExeXMLMFS2020();

};


#endif //BITSANDDROIDSCONNECTOR_INSTALLATIONSERVICE_H
