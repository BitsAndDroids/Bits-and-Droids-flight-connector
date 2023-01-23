//
// Created by dave- on 19-9-2022.
//

#include <QFile>
#include <QApplication>
#include <QProcess>
#include <QPushButton>
#include "DashboardController.h"
#include "services/InstallationService.h"


void DashboardController::initController(){
    connect(serviceWorker, &ServiceWorker::gameConnectionMade, this, &DashboardController::gameConnectionMade);
    connect(serviceWorker, &ServiceWorker::wasmConnectionMade, this, &DashboardController::wasmConnectionMade);

    serviceWorker->start();
    connect(this, &DashboardController::updateEventFile, serviceWorker, &ServiceWorker::updateEventFile);
    SettingsHandler settingsHandler = SettingsHandler();
    settingsHandler.checkEventFilePresent();
    InstallationService installationService = InstallationService();
    if(installationService.getUpdatesAvailable()){
        emit updateAvailable();
    }

}

void DashboardController::setServiceWorker(ServiceWorker *serviceWorkerToSet) {
    this->serviceWorker = serviceWorkerToSet;
}


DashboardController::DashboardController(QMainWindow *parent) {
    this->parent = parent;
}

QList<ModeIndexCheckbox *> DashboardController::getCheckboxesByPattern(const QRegularExpression &pattern){

}