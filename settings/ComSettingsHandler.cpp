//
// Created by DaveRiedel on 11-10-2022.
//

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "ComSettingsHandler.h"

ComSettingsHandler::ComSettingsHandler() {

}

void ComSettingsHandler::saveComs(QList<QPair<QString,int>> coms) {
    QJsonObject obj = comListToJsonObject(coms);
    storeValue("com","bundles", obj);
    for (int i = 0; i < coms.size(); ++i) {
        storeValue("comPorts",QString::number(i),coms[i].first);
        storeValue("comSets",QString::number(i),coms[i].second);
    }
}

void ComSettingsHandler::adjustIndexes(){
    auto comPorts = retrieveKeys("comPorts");
    auto comSets = retrieveKeys("comSets");
    auto autoRunStates = retrieveKeys("autoRunStates");
    if(comPorts != nullptr) {
        for (int i = 0; i < comPorts->size(); i++) {
            auto comValue = retrieveSetting("comPorts", comPorts->at(i))->toString();
            removeSetting("comPorts", comPorts->at(i));
            storeValue("comPorts", QString::number(i), comValue);
        }
    }
    if(comSets != nullptr) {
        for (int i = 0; i < comSets->size(); i++) {
            auto setValue = retrieveSetting("comSets", comSets->at(i))->toInt();
            removeSetting("comSets", comSets->at(i));
            storeValue("comSets", QString::number(i), setValue);
        }
    }
    if(autoRunStates != nullptr) {
        for (int i = 0; i < autoRunStates->size(); i++) {
            auto autoRunValue = retrieveSetting("autoRunStates", autoRunStates->at(i))->toBool();
            removeSetting("autoRunStates", autoRunStates->at(i));
            storeValue("autoRunStates", QString::number(i), autoRunValue);
        }
    }
}

void ComSettingsHandler::removeComRowSettings(QString index) {
    removeSetting("comPorts", index);
    removeSetting("comSets", index);
    removeSetting("autoRunStates", index);
}

QList<QPair<QString, int>> ComSettingsHandler::loadComs() {
    auto bundleJsonArray = retrieveSetting("com","bundles")->toJsonObject();
    return jsonObjectToComList(bundleJsonArray);
}

QList<QPair<QString, int>> ComSettingsHandler::jsonObjectToComList(QJsonObject obj) {
    QList<QPair<QString, int>> coms = QList<QPair<QString, int>>();
    for(const auto& key : obj.keys()){
        auto pair = comPairFromJson(obj[key].toObject());
        coms.append(pair);
    }
    return coms;
}

QJsonObject ComSettingsHandler::comListToJsonObject(QList<QPair<QString,int>> coms) {
    QJsonObject obj;
    for (int i = 0; i < coms.size(); i++) {
        obj[QString::number(i)] =  comPairToJson(coms[i]);
    }
    return obj;
}

QJsonObject ComSettingsHandler::comPairToJson(const QPair<QString, int>& pair) {
    QJsonObject object = QJsonObject();
    object.insert("comPort", pair.first);
    object.insert("comSet", pair.second);
    return object;
}

QPair<QString, int> ComSettingsHandler::comPairFromJson(const QJsonObject& object) {
    QPair<QString, int> pair;
    pair.first = object.value("comPort").toString();
    pair.second = object.value("comSet").toInt();
    return pair;
}

