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

//stylesheet
// QPushbutton#btn_plane_style_inactive{
//     background-color: #605f5f;
//     border: 4px solid #59493b;
//     color: #e1ae6b;
//     font-weight: 900;
//     line-height: 15px;
//
//     padding: 10px 20px;
//     text-align: center;
//     text-decoration: none;
//     display: inline-block;
//     font-size: 12px;
//     margin: 0px 0px;
//     cursor: pointer;
//     border-radius: 3px;
//     -webkit-transition-duration: 0.4s; /* Safari */
//     transition-duration: 0.4s;
//     height: fit-content;
//     font-family: "Roboto Bold", sans-serif;
//     box-shadow: #e1ae6b 0px 0px 5px 0px;
//     width: 200px;
// }
// QPushbutton#btn_plane_style_active{
//     background-color: #605f5f;
//     border: 4px solid #3f593b;
//     color: #96d327;
//     font-weight: 900;
//     line-height: 15px;
//
//     padding: 10px 20px;
//     text-align: center;
//     text-decoration: none;
//     display: inline-block;
//     font-size: 12px;
//     margin: 0px 0px;
//     cursor: pointer;
//     border-radius: 3px;
//     -webkit-transition-duration: 0.4s; /* Safari */
//     transition-duration: 0.4s;
//     height: fit-content;
//     font-family: "Roboto Bold", sans-serif;
//     box-shadow: #8ce16b 0px 0px 5px 0px;
//     width: 200px;
// }
// QPushbutton#btn_plane_style_active:hover{
//     background-color: #3f3f3f;
//   }
//
// QPushbutton#btn_plane_style_inactive:hover{
//     background-color: #3f3f3f;
//   }
QWidget* PresetRow::generateElement() {
    this->setParent(this->parent);
    const auto styleWidget = new QWidget(this);
    styleWidget->setObjectName("presetRowWidget");

    auto* layout = new QHBoxLayout();
    auto* styleLayout = new QHBoxLayout();
    styleLayout->setAlignment(Qt::AlignLeft);
    //Set size to minimum
    auto* preset_annunciator_button = new MPushButton(QString::fromStdString(this->preset.getName()), styleLayout, ButtonStyleEnum::DEFAULT);
    preset_annunciator_button->setObjectName("btn_plane_style_inactive");
    connect(preset_annunciator_button, &QPushButton::clicked, this, &PresetRow::setActive);

    auto* deletePreset = new QPushButton("Delete preset");
    deletePreset->setObjectName("BtnDeletePreset");
    //set stylesheet as seen in comment above this function
    this->setStyleSheet(
        " #btn_plane_style_inactive{"
        "background-color: #605f5f;"
        "border: 4px solid #59493b;"
        "color: #e1ae6b;"
        "font-weight: 900;"
        "line-height: 15px;"
        "padding: 10px 20px;"
        "text-align: center;"
        "text-decoration: none;"
        "display: inline-block;"
        "font-size: 12px;"
        "margin: 0px 0px;"
        "cursor: pointer;"
        "border-radius: 3px;"
        "-webkit-transition-duration: 0.4s; /* Safari */"
        "transition-duration: 0.4s;"
        "height: fit-content;"
        //"box-shadow: #e1ae6b 0px 0px 5px 0px;"
        "width: 200px;"
        "max-width: 200px;"
        "}"
        "#btn_plane_style_active{"
        "background-color: #605f5f;"
        "border: 4px solid #3f593b;"
        "color: #96d327;"
        "font-weight: 900;"
        "line-height: 15px;"
        "padding: 10px 20px;"
        "text-align: center;"
        "text-decoration: none;"
        "display: inline-block;"
        "font-size: 12px;"
        "margin: 0px 0px;"
        "cursor: pointer;"
        "border-radius: 3px;"
        "-webkit-transition-duration: 0.4s; /* Safari */"
        "transition-duration: 0.4s;"
        "height: fit-content;"
        //"box-shadow: #8ce16b 0px 0px 5px 0px;"
        "width: 200px;"
        "max-width: 200px;"
        "}"
        " #btn_plane_style_active:hover{"
        "background-color: #3f3f3f;"
        "}"
        " #btn_plane_style_inactive:hover{"
        "background-color: #3f3f3f;"
        "}"
    );
    auto* delete_preset_icon_button = new MPushButton(styleLayout, ButtonStyleEnum::DEFAULT);
    delete_preset_icon_button->setIconWithPath(":/resources/images/trashcan.svg");
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
        const auto btn = this->findChild<QPushButton*>();
        if (active) {
            btn->setObjectName("btn_plane_style_active");
            btn->repaint();
            btn->update();
            btn->show();
            qDebug() << "PresetRow::setActiveStyle(true)";
        }
        else {
            btn->setObjectName("btn_plane_style_inactive");
            btn->repaint();
            btn->update();
            btn->show();
            qDebug() << "PresetRow::setActiveStyle(false)";
        }
    }
}
