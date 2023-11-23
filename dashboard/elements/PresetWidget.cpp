//
// Created by dave- on 20-11-2023.
//

#include "PresetWidget.h"
#include <QDebug>
#include <QHBoxLayout>

#include "PresetRow.h"
#include "dashboard/controller/PresetWidgetController.h"
#include "dashboard/models/Preset.h"
#include "elements/MStyleLabels.h"
#include "settings/PresetSettingsHandler.h"

PresetWidget::PresetWidget(QWidget* parent): QWidget(parent) {
    this->setObjectName("presetWidget");
    this->parent = parent;
}

QWidget* PresetWidget::generateElement() {
    this->setParent(this->parent);
    this->controller = new PresetWidgetController(this);
    const auto styleWidget = new QWidget(this);
    styleWidget->setObjectName("presetWidget");
    qDebug() << "className: " << this->metaObject()->className();
    qDebug() << "type: " << this->metaObject()->metaType().name();

    styleWidget->setStyleSheet("QWidget#presetWidget { "
        "background-color: #fff!important; "
        "border-radius: 5px; "
        "}");

    qDebug() << "Applied Styles:" << this->styleSheet();
    const auto layout = new QVBoxLayout();

    layout->setAlignment(Qt::AlignTop);

    qDebug() << this->styleSheet();
    generatePresetRows(layout);
    qDebug() << "PresetWidget::generateElement()";
    styleWidget->setLayout(layout);
    const auto newLayout = new QVBoxLayout();
    newLayout->addWidget(styleWidget);
    newLayout->setAlignment(Qt::AlignTop);
    newLayout->addStretch(1);
    this->setLayout(newLayout);
    this->show();

    return this;
}

std::vector<Preset> PresetWidget::loadPresets() {
    qDebug() << "PresetWidget::loadPresets()";
    return PresetWidgetController::loadPresets();
}

void PresetWidget::generatePresetRows(QVBoxLayout* layout) {
    if (layout == nullptr) {
        qDebug() << "Layout is null";
        return;
    }
    const std::vector<Preset> presets = loadPresets();
    if (presets.empty()) {
        qDebug() << "No presets found";
        return;
    }
    layout->addWidget(new MStyleLabels("PRESETS", H2));
    const auto presetSettingHandler = new PresetSettingsHandler();
    for (const Preset&preset: presets) {
        auto* row = new PresetRow(this, preset);
        layout->addWidget(row->generateElement(), Qt::AlignTop);
        layout->setAlignment(Qt::AlignTop);
        layout->addStretch(1);
        connect(row, SIGNAL(saveDefaultPresetSignal(Preset)), controller, SLOT(saveDefaultPreset(Preset)));
        if (QString::fromStdString(preset.getName()) == presetSettingHandler->getDefaulPreset()) {
            row->setActiveStyle(true);
        }
    }
}
