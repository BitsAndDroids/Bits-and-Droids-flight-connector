//
// Created by dave- on 20-11-2023.
//

#include "PresetWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QMainWindow>

#include "PresetRow.h"
#include "dashboard/controller/PresetWidgetController.h"
#include "dashboard/models/Preset.h"
#include "elements/MStyleLabels.h"

PresetWidget::PresetWidget(QMainWindow* parent, const PresetWidgetController* controller) {
    this->parent = parent;
    this->controller = controller;
}

QWidget* PresetWidget::generateElement() {
    auto* widget = new QWidget(parent);
    widget->setObjectName("presetWidget");
    widget->setStyleSheet("#presetWidget { "
        "background-color: #fff; "
        "border-radius: 5px; "
        "}");

    widget->setGeometry(0, 0, 100, 100);
    const auto layout = new QVBoxLayout();

    layout->setAlignment(Qt::AlignTop);
    generatePresetRows(layout);
    qDebug()<< "PresetWidget::generateElement()";
    widget->setLayout(layout);
    widget->show();
    return widget;
}

std::vector<Preset> PresetWidget::loadPresets() {
    qDebug() << "PresetWidget::loadPresets()";
    return PresetWidgetController::loadPresets();
}

void PresetWidget::generatePresetRows(QVBoxLayout* layout) {
    if(layout == nullptr) {
        qDebug() << "Layout is null";
        return;
    }
    const std::vector<Preset> presets = loadPresets();
    if(presets.empty()) {
        qDebug() << "No presets found";
        return;
    }
    layout->addWidget(new MStyleLabels("PRESETS", H2));
    for (const Preset& preset : presets) {
        auto *row = new PresetRow(parent, preset);
        layout->addWidget(row->generateElement());
        row->setActive();
    }
}
