//
// Created by dave- on 20-11-2023.
//

#ifndef PRESETWIDGETCONTROLLER_H
#define PRESETWIDGETCONTROLLER_H
#include <QMainWindow>

#include "dashboard/models/Preset.h"


class PresetWidgetController {
public:
    explicit PresetWidgetController(QMainWindow *parent);

    static std::vector<Preset> loadPresets();
private:
    QMainWindow * parent = nullptr;

};



#endif //PRESETWIDGETCONTROLLER_H
