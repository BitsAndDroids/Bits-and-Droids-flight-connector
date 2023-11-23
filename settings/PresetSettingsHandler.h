//
// Created by dave- on 20-11-2023.
//

#ifndef PRESETSETTINGSHANDLER_H
#define PRESETSETTINGSHANDLER_H
#include "settingshandler.h"
#include "dashboard/models/Preset.h"


class PresetSettingsHandler final : public SettingsHandler {
public:
    PresetSettingsHandler();
    std::vector<Preset> loadPresets();
    void savePreset(Preset preset);

    void setDefaultPreset(Preset preset);

    QString getDefaulPreset();

private:
    static QVariantMap serializePreset(const Preset& preset);
    static Preset deserializePreset(QVariantMap map);
};



#endif //PRESETSETTINGSHANDLER_H
