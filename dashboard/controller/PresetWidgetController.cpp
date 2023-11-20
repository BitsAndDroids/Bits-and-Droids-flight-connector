//
// Created by dave- on 20-11-2023.
//

#include "PresetWidgetController.h"
#include <QDebug>
#include "settings/PresetSettingsHandler.h"
#include "settings/settingshandler.h"

PresetWidgetController::PresetWidgetController(QMainWindow *parent) {
    this->parent = parent;
    const Preset preset = Preset("Custom", std::map<std::string, int>({{"com1", 1}}));
    auto* presetSettingsHandler = new PresetSettingsHandler();
    presetSettingsHandler->savePreset(preset);
}

std::vector<Preset> PresetWidgetController::loadPresets() {
    const auto presetSettingsHandler = new PresetSettingsHandler();
    qDebug() << "PresetWidgetController::loadPresets()";
    return presetSettingsHandler->loadPresets();
}
