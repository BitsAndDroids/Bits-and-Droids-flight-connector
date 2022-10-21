//
// Created by DaveRiedel on 18-10-2022.
//

#include <QProcess>
#include <QMessageBox>
#include "InstallationService.h"
#include "handlers/pathhandler.h"
#include "logging/MessageCaster.h"

InstallationService::InstallationService() {

}

void InstallationService::installWASMModule() {

}

bool InstallationService::getUpdatesAvailable() {
    auto *process = new QProcess();
    PathHandler pathHandler = PathHandler();
    process->start(pathHandler.getMaintenanceToolPath() + " ch");
    process->waitForFinished();
    QByteArray data = process->readAll();
    return data.contains("update name");

}
