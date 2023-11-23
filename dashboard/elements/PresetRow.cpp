//
// Created by dave- on 20-11-2023.
//

#include "PresetRow.h"

#include <QHBoxLayout>

#include "elements/MPushButton.h"
#include "elements/MStyleLabels.h"
#include "settings/PresetSettingsHandler.h"

PresetRow::PresetRow(QWidget* parent, Preset preset): QWidget(parent) {
    this->parent = parent;
    this->preset = std::move(preset);
    this->setStyleSheet("");
    this->setObjectName("presetRowWidget");
}

QWidget* PresetRow::generateElement() {
    this->setParent(this->parent);
    const auto styleWidget = new QWidget(this);
    styleWidget->setObjectName("presetRowWidget");

    auto *layout = new QHBoxLayout();
    auto *styleLayout = new QHBoxLayout();

    auto *name = new MStyleLabels(QString::fromStdString(this->preset.getName()), LABEL);
    auto *setActive = new QPushButton("Set default");
    setActive->connect(setActive, &QPushButton::clicked, this, &PresetRow::setActive);
    auto *deletePreset = new QPushButton("Delete preset");

    styleLayout->addWidget(name);
    styleLayout->addWidget(setActive);
    styleLayout->addWidget(deletePreset);

    styleWidget->setLayout(styleLayout);
    layout->addWidget(styleWidget);

   this->setLayout(layout);

    return this;
}

void PresetRow::setActive() {
    emit(saveDefaultPresetSignal(this->preset));
}

void PresetRow::setActiveStyle(bool active) {
    if(this) {
        if(active) {
            this->setStyleSheet("QWidget#presetRowWidget {border-color: #9fd980; border-radius: 5px; border-width: 2px; border-style: solid;}");
            qDebug() << "PresetRow::setActiveStyle(true)";
        } else {
            this->setStyleSheet("QWidget#presetRowWidget {border-color: #9fd980; border-radius: 5px; border-width: 0px; border-style: none;}");
            qDebug() << "PresetRow::setActiveStyle(false)";
        }
    }
}
