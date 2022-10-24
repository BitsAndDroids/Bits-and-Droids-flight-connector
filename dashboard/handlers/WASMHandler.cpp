//
// Created by DaveRiedel on 21-9-2022.
//

#include <QDir>
#include <QCoreApplication>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include "WASMHandler.h"
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
        std::cout << sourceString.toStdString() << std::endl;
        if (customPathFound) {
            pathfound = pathHandler.getCommunityFolderPath();
        } else {
          std::cout<<"Could not find the community folder"<<std::endl;

        }

        QString destinationString = pathfound + "/BitsAndDroidsModule";
        copyFolder(sourceString, destinationString);
    }
    catch (...) {
        std::cout << "error" << std::endl;
    }
}

bool WASMHandler::isWASMModuleInstalled() {
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

WASMHandler::WASMHandler() {
    ServiceSettingsHandler serviceSettingsHandler = ServiceSettingsHandler();
    QDir dir = QDir(serviceSettingsHandler.getCommunityFolderPath() + "/BitsAndDroidsModule");
    if(dir.exists()){
        WASMModulePath = dir.absolutePath();
    }
}

bool WASMHandler::versionCheck(){
    //return true if the version found in the community folder is the same as the version in the application folder
    QString communityVersion = "";
    QString localVersion = getLocalVersion();
    if(WASMModulePath.size() > 0){

        QFile file(WASMModulePath + "/manifest.JSON");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        QJsonObject jsonObject = jsonDoc.object();
        communityVersion = jsonObject["package_version"].toString();
    }
    QStringList communityVersionList = communityVersion.split(".");
    QStringList localVersionList = localVersion.split(".");
    for(int i = 0; i < localVersionList.size(); i++){
        std::cout<<localVersionList[i].toInt()<<std::endl;
        std::cout<<communityVersionList[i].toInt()<<std::endl;
        if(localVersionList[i].toInt() > communityVersionList[i].toInt()){
            std::cout<<"Local version is higher than remote version"<<std::endl;
            updateModule();
            i = localVersionList.size();
        }
    }
}

void WASMHandler::updateModule(){
    QFile localManifest(QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule/manifest.JSON");
    QFile localModule(QCoreApplication::applicationDirPath() + "/BitsAndDroidsModule/modules/WASM_Module_BitsAndDroids.WASM");

    QFile remoteManifest(WASMModulePath + "/manifest.JSON");
    QFile remoteModule(WASMModulePath + "/modules/WASM_Module_BitsAndDroids.WASM");

    remoteManifest.copy(localManifest.fileName() + ".old");
    remoteModule.copy(localModule.fileName() + ".old");

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
    file.open(QIODevice::ReadOnly | QIODevice::Text);
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