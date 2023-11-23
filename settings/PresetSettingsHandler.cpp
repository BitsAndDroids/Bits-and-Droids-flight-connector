//
// Created by dave- on 20-11-2023.
//

#include "PresetSettingsHandler.h"
#include <QDebug>
#include <qopenglcontext_platform.h>


PresetSettingsHandler::PresetSettingsHandler() = default;

std::vector<Preset> PresetSettingsHandler::loadPresets() {
    QVariantList* presetList = getGroup("presets");
    qDebug() << "PresetSettingsHandler::loadPresets()";
    qDebug() << presetList->length();
    std::vector<Preset> presets;
    for (const auto& preset: *presetList) {
        const auto deserializedPreset = deserializePreset(preset.toMap());
        qDebug() << deserializedPreset.getName().c_str();
        presets.push_back(deserializedPreset);
    }
    return presets;
}

void PresetSettingsHandler::savePreset(Preset preset) {
    storeSettingValue("presets", QString::fromStdString(preset.getName()), serializePreset(preset));
}

void PresetSettingsHandler::setDefaultPreset(Preset preset) {
    storeSettingValue("PresetParameters", "default", QString::fromStdString(preset.getName()));
}

QString PresetSettingsHandler::getDefaulPreset() {
    return getSettingValue("PresetParameters", "default")->toString();
}

QVariantMap PresetSettingsHandler::serializePreset(const Preset& preset) {
    QVariantMap map;
    map["name"] = QString::fromStdString(preset.getName());

    QVariantMap rowsMap;
    for (const auto& pair : preset.getRows()) {
        rowsMap[QString::fromStdString(pair.first)] = pair.second;
    }
    map["rows"] = rowsMap;

    return map;
}

Preset PresetSettingsHandler::deserializePreset(QVariantMap map) {
    std::string name = map["name"].toString().toStdString();

    std::map<std::string, int> rows;
    QVariantMap rowsMap = map["rows"].toMap();
    for (auto iter = rowsMap.begin(); iter != rowsMap.end(); ++iter) {
        rows[iter.key().toStdString()] = iter.value().toInt();
    }

    return {name, rows};
}
