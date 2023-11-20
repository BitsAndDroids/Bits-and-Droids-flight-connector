//
// Created by dave- on 20-11-2023.
//

#include "PresetRow.h"

#include <QHBoxLayout>

#include "elements/MPushButton.h"
#include "elements/MStyleLabels.h"

PresetRow::PresetRow(QMainWindow* parent, Preset preset) {
    this->parent = parent;
    this->preset = preset;
}

QWidget* PresetRow::generateElement() {
    widget = new QWidget(parent);

    auto *layout = new QHBoxLayout();

    auto *name = new MStyleLabels(QString::fromStdString(this->preset.getName()), LABEL);
    auto *setActive = new QPushButton("Set active");
    setActive->connect(setActive, &QPushButton::clicked, this, &PresetRow::setActive);
    auto *deletePreset = new QPushButton("Delete preset");

    layout->addWidget(name);
    layout->addWidget(setActive);
    layout->addWidget(deletePreset);

    widget->setLayout(layout);
    widget->setObjectName("presetRowWidget");
    return widget;
}

void PresetRow::setActive() {
    this->preset.setActive(!this->preset.isActive());
    //set background of this widget to green
    if(widget) {
        if(this->preset.isActive()) {
            widget->setStyleSheet("#presetRowWidget {border-color: #9fd980; border-radius: 5px; border-width: 2px; border-style: solid;}");
        } else {
            widget->setStyleSheet("{background-color: #fff}");
        }
    }



}
