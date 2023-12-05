//
// Created by dave- on 20-11-2023.
//

#include "PresetRow.h"

#include <QHBoxLayout>
#include <QRadioButton>

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

    auto* layout = new QHBoxLayout();
    auto* styleLayout = new QHBoxLayout();
    //Set size to minimum
    this->setMaximumHeight(60);

    const auto default_radio_button = new QRadioButton();
    styleLayout->addWidget(default_radio_button);

    auto* name = new MStyleLabels(QString::fromStdString(this->preset.getName()), LABEL);
    auto* setActive = new QPushButton("Set default");
    setActive->setObjectName("BtnSetActive");
    connect(setActive, &QPushButton::clicked, this, &PresetRow::setActive);
    auto* deletePreset = new QPushButton("Delete preset");
    deletePreset->setObjectName("BtnDeletePreset");
    this->setStyleSheet(
        "QWidget #presetRowWidget{"
        "background-color:#fff;"
        "margin: 0px;"
        "}"
        "QPushButton {"
        "width: 100px;"
        "min-width: 100px;"
        "max-width: 100px;"
        "}");

    styleLayout->addWidget(name);
    styleLayout->addWidget(setActive);
    if (preset.getName() != "Custom") {
        styleLayout->addWidget(deletePreset);
    }

    styleWidget->setLayout(styleLayout);
    layout->addWidget(styleWidget);
    this->setLayout(layout);

    return this;
}

void PresetRow::setActive() {
    emit(saveDefaultPresetSignal(this->preset));
}

void PresetRow::setActiveStyle(bool active) {
    if (this) {
        if (active) {
            this->setStyleSheet(
                "QPushButton {"
                "width: 100px;"
                "min-width: 100px;"
                "max-width: 100px;"
                "}"
                "QWidget#presetRowWidget {border-color: #9fd980; border-radius: 5px; border-width: 2px; border-style: solid;}");
            qDebug() << "PresetRow::setActiveStyle(true)";
        }
        else {
            this->setStyleSheet(
                "QPushButton {"
                "width: 100px;"
                "min-width: 100px;"
                "max-width: 100px;"
                "}"
                "QWidget#presetRowWidget {border-color: #9fd980; border-radius: 5px; border-width: 0px; border-style: none;}");
            qDebug() << "PresetRow::setActiveStyle(false)";
        }
    }
}
