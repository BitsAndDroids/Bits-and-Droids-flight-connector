//
// Created by dave- on 20-11-2023.
//

#ifndef PRESETWIDGETCONTROLLER_H
#define PRESETWIDGETCONTROLLER_H
#include <QMainWindow>
#include <QObject>

#include "dashboard/elements/PresetRow.h"
#include "dashboard/models/Preset.h"


class PresetWidgetController final : public QObject{
    Q_OBJECT
public:
    explicit PresetWidgetController(QWidget *parent);
    static std::vector<Preset> loadPresets();

public slots:
        void saveDefaultPreset(Preset preset);

private:
    QWidget* parent = nullptr;
    void setActiveRowStyle(PresetRow* row, bool active);
};



#endif //PRESETWIDGETCONTROLLER_H
