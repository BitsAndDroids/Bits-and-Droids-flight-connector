//
// Created by DaveRiedel on 21-9-2022.
//

#include <QDir>
#include <QCoreApplication>
#include <exception>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include "WASMHandler.h"
#include "logging/Logger.h"
#include "settings/ServiceSettingsHandler.h"
#include "handlers/pathhandler.h"
#include "logging/MessageCaster.h"

void WASMHandler::installWasm() {
    //TODO FIX THIS PART
    PathHandler pathHandler;
    try {

        bool customPathFound = pathHandler.getCommunityFolderPath() != nullptr;
        QString pathfound = "";
        QString sourceString =
                QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule";
        if (customPathFound) {
            pathfound = pathHandler.getCommunityFolderPath();
        } else {
            Logger::getInstance()->logDebug("Could not find community folder");

        }
        Logger::getInstance()->logDebug("Installing WASM in: " + pathfound.toStdString());
        QString destinationString = pathfound + "/BitsAndDroidsModule";
        copyFolder(sourceString, destinationString);
    }
    catch (const std::exception &e) {
        Logger::getInstance()->logError("Couldn't install WASM, reason: " + std::string(e.what()));
    }
}

bool WASMHandler::isWASMModuleInstalled() {
    try{
      ServiceSettingsHandler serviceSettingsHandler = ServiceSettingsHandler();
      if(serviceSettingsHandler.retrieveSetting("Settings","communityFolderPathLabel")->toString() == "" && !serviceSettingsHandler.searchDefaultCommunityLocation()){
          return false;
      }
      QDir dir(serviceSettingsHandler.getCommunityFolderPath() + "/BitsAndDroidsModule");
      if(!dir.exists()){
          installWasm();
      }
      if(dir.exists()){
          WASMModulePath = dir.absolutePath();
      }
      versionCheck();
      return dir.exists();
    }
    catch (...){
        return false;
    }
}

WASMHandler::WASMHandler() {
        ServiceSettingsHandler serviceSettingsHandler = ServiceSettingsHandler();
        QDir dir = QDir(serviceSettingsHandler.getCommunityFolderPath() + "/BitsAndDroidsModule");
        if (dir.exists()) {
            WASMModulePath = dir.absolutePath();
        }
}

bool WASMHandler::versionCheck(){
    try {
        //return true if the version found in the community folder is the same as the version in the application folder
        QString communityVersion = "";
        QString localVersion = getLocalVersion();
        if (WASMModulePath.size() > 0) {

            QFile file(WASMModulePath + "/manifest.JSON");
            file.open(QIODevice::ReadOnly);
            QByteArray data = file.readAll();
            file.close();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

            QJsonObject jsonObject = jsonDoc.object();
            communityVersion = jsonObject["package_version"].toString();
        }
        QStringList communityVersionList = communityVersion.split(".");
        QStringList localVersionList = localVersion.split(".");
        std::cout<<communityVersionList.size()<<std::endl;
        std::cout<<localVersionList.size()<<std::endl;
        if (communityVersionList.size() == localVersionList.size()) {
            for (int i = 0; i < localVersionList.size(); i++) {
                if (localVersionList[i].toInt() > communityVersionList[i].toInt()) {
                    updateModule();
                    return false;
                }
            }
        }
        return true;
    }
    catch (...){
        return false;
    }

}

void WASMHandler::updateModule(){

    QFile localManifest(QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule/manifest.JSON");
    QFile localModule(QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule/modules/WASM_Module_BitsAndDroids.WASM");

    QFile remoteManifest(WASMModulePath + "/manifest.JSON");
    QFile remoteModule(WASMModulePath + "/modules/WASM_Module_BitsAndDroids.WASM");

    remoteManifest.copy(remoteManifest.fileName() + ".old");
    remoteModule.copy(remoteModule.fileName() + ".old");

    remoteManifest.remove();
    remoteModule.remove();

    localManifest.copy(WASMModulePath + "/manifest.JSON");
    localModule.copy(WASMModulePath + "/modules/WASM_Module_BitsAndDroids.WASM");

    if(remoteManifest.exists() && remoteModule.exists()){
        QFile(WASMModulePath + "/manifest.JSON.old").remove();
        QFile(WASMModulePath + "/modules/WASM_Module_BitsAndDroids.WASM.old").remove();
    }
}

QString WASMHandler::getLocalVersion(){
    QFile file(QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule/manifest.JSON");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = jsonDoc.object();
    return jsonObject["package_version"].toString();
}

void WASMHandler::copyFolder(const QString &sourceFolder, const QString &destinationFolder) {

    QDir sourceDir(sourceFolder);

    QDir destinationDir(destinationFolder);

    if (!sourceDir.exists()) {
        return;
    }

    if (!destinationDir.exists()) {
        destinationDir.mkdir(destinationFolder);

    }
    QStringList files = sourceDir.entryList(QDir::Files);

    for (int i = 0; i < files.count(); i++) {
        QString sourceName = sourceFolder + "/" + files[i];
        QString destName = destinationFolder + "/" + files[i];
        if (!(files[i] == ("events.txt"))) {
            QFile::copy(sourceName, destName);
        }
    }
    QStringList dirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (auto &dir: dirs) {
        QString sourceName = sourceFolder + "/" + dir;
        QString destinationName = destinationFolder + "/" + dir;
        copyFolder(sourceName, destinationName);
    }
}

void WASMHandler::updateEventFile(){
    PathHandler pathHandler = PathHandler();
    try {
        QFile::remove(pathHandler.getCommunityFolderPath() +
                      "/BitsAndDroidsModule/modules/events.txt");
        QFile::copy(pathHandler.getWritableEventPath(),
                    pathHandler.getCommunityFolderPath() +
                    "/BitsAndDroidsModule/modules/events.txt");


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