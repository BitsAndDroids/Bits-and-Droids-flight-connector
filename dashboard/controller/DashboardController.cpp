//
// Created by dave- on 19-9-2022.
//

#include <QFile>
#include <QApplication>
#include <QProcess>
#include <QPushButton>
#include "DashboardController.h"
#include "services/InstallationService.h"

void DashboardController::updateEventFile() {
    try {
        QFile::remove(pathHandler.getCommunityFolderPath() +
                      "/BitsAndDroidsModule/modules/events.txt");
        QFile::copy(applicationEventsPath,
                    pathHandler.getCommunityFolderPath() +
                    "/BitsAndDroidsModule/modules/events.txt");
        connect(this, &DashboardController::sendWASMCommand, serviceWorker,
                &ServiceWorker::sendWASMData);

        emit sendWASMCommand("9999");
        MessageCaster::showCompleteMessage("Successfully updated the event file");
    }
        //This catches the specific error thrown when no output is running
        //Since we send a command over SimConnect to the WASM module we need to have a connection that is alive
    catch (const std::invalid_argument &i) {
        MessageCaster::showWarningMessage("Please start the connector before updating the remote event file");
    }
    catch (const std::exception &e) {
        MessageCaster::showWarningMessage("Something went wrong updating the event file");
    }
}

DashboardController::DashboardController(QMainWindow *parent) {
    this->parent = parent;
    //TODO connect logger
    //QObject::connect(&dualWorker, &MFSWorker::logMessage, &serviceWorker, &ServiceWorker::logMessage);
    SettingsHandler settingsHandler = SettingsHandler();
    settingsHandler.checkEventFilePresent();
}

void DashboardController::initController(){
    connect(serviceWorker, &ServiceWorker::gameConnectionMade, this, &DashboardController::gameConnectionMade);
    serviceWorker->start();
    InstallationService installationService = InstallationService();
    if(installationService.getUpdatesAvailable()){
        emit updateAvailable();
    }
}

QList<ModeIndexCheckbox *> DashboardController::getCheckboxesByPattern(const QRegularExpression &pattern){

}