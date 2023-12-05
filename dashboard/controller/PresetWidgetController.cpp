//
// Created by dave- on 20-11-2023.
//

#include "PresetWidgetController.h"
#include <QDebug>
#include "settings/PresetSettingsHandler.h"

PresetWidgetController::PresetWidgetController(QWidget *parent) {
    this->parent = parent;
    qDebug() << "PresetWidgetController::PresetWidgetController()";
    qDebug() << "Parent: " << this->parent;
    const Preset preset = Preset("Custom", std::map<std::string, int>({{"com1", 1}}));

    const Preset testPreset = Preset(" Test", std::map<std::string, int>({{"com1", 1}}));

    auto* presetSettingsHandler = new PresetSettingsHandler();
    presetSettingsHandler->savePreset(preset);
    presetSettingsHandler->savePreset(testPreset);
}

void PresetWidgetController::saveDefaultPreset(Preset preset) {
    qDebug() << "PresetWidgetController::saveDefaultPreset()";
    emit updatePreset(preset);
    // caller PresetRow
    const auto callerRow = dynamic_cast<PresetRow*>(sender());
    if(callerRow == nullptr) {
        qDebug() << "Caller row is null";
        return;
    }
    const auto presetSettingsHandler = new PresetSettingsHandler();
    presetSettingsHandler->setDefaultPreset(std::move(preset));
    // set active style to false for all rows
    if(parent != nullptr) {
        try {
            qDebug() << parent->objectName();
            const auto presetRows = parent->findChildren<PresetRow*>();
            for (const auto& row : presetRows) {
                setActiveRowStyle(row, false);
            }
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        setActiveRowStyle(callerRow, true);
    } else {
        // Handle the case when parent is null
        std::cerr << "Parent is not defined. Cannot find children." << std::endl;
    }
}

void PresetWidgetController::setActiveRowStyle(PresetRow* row, bool active) {
    row->setActiveStyle(active);
}

std::vector<Preset> PresetWidgetController::loadPresets() {
    const auto presetSettingsHandler = new PresetSettingsHandler();
    qDebug() << "PresetWidgetController::loadPresets()";
    return presetSettingsHandler->loadPresets();
}
