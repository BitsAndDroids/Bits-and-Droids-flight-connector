//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortRow.h"
#include "elements/ModeIndexCombobox.h"
#include "elements/MPushButton.h"
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSerialPort>

QWidget *ComPortRow::generateElement() {
    auto *comSelector = new QWidget();
    comSelector->setObjectName("comRowWidget");
    //TODO move stylesheets to seperate files
    comSelector->setStyleSheet("QComboBox{"
                               "border-radius: 4px;"
                               "height: 30px;"
                               "margin-right:4px;"
                               "padding-left:4px;"
                               "}"
                               "QComboBox:editable {\n"
                               "    background: white;\n"
                               "}"
                               "\n"

                               "QComboBox:on { /* shift the text when the popup opens */\n"
                               "    padding-top: 3px;\n"
                               "    padding-left: 4px;\n"
                               "}\n"
                               "\n"
                               "QComboBox::drop-down {\n"
                               "    subcontrol-origin: padding;\n"
                               "    subcontrol-position: top right;\n"
                               "    width: 15px;\n"
                               "\n"
                               "    border-left-width: 1px;\n"
                               "    border-left-color: darkgray;\n"
                               "    border-left-style: solid; /* just a single line */\n"
                               "    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
                               "    border-bottom-right-radius: 3px;\n"
                               "}\n"
                               "\n"
                               "QComboBox::down-arrow {\n"
                               "    image: url(/usr/share/icons/crystalsvg/16x16/actions/1downarrow.png);\n"
                               "}\n"
                               "\n"
                               "QComboBox::down-arrow:on { /* shift the arrow when popup is open */\n"
                               "    top: 1px;\n"
                               "    left: 1px;\n"
                               "}");
    auto *comRow = new QHBoxLayout();
    auto *dropdownContainer = new QWidget();
    auto *dropdownLayout = new QHBoxLayout(dropdownContainer);
    dropdownContainer->setObjectName("dropdownContainer");

    comSelector->setLayout(comRow);
    auto *comPortComboBox = new ModeIndexCombobox("comBox",index);

    ComPortWidgetModel model = ComPortWidgetModel();
    auto availableComPorts = model.loadAvailableComPorts();
    auto availableSets = model.getAvailableSets();

    for (auto &availableComPort: availableComPorts) {
        comPortComboBox->addItem(availableComPort);
    }
    dropdownLayout->addWidget(comPortComboBox);
    comPortComboBox->setMinimumWidth(150);
    if (setsNeeded) {
        auto *setComboBox = new ModeIndexCombobox("setBox",index);
        setComboBox->addItem("No outputs");
        for (const auto &availableSet: *availableSets) {
            setComboBox->addItem(availableSet.getSetName());
        }
        setComboBox->setMinimumWidth(150);
        dropdownLayout->addWidget(setComboBox);
        qDebug() << setComboBox->objectName();
    }

    comRow->addWidget(dropdownContainer);

    auto *removeButton = new MPushButton(comRow);
    removeButton->setIconWithPath(":/resources/images/trashcan.svg");

    removeButton->setObjectName("del" + QString::number(index));

    QObject::connect(removeButton, &QAbstractButton::clicked, controller,
            &ComPortWidgetController::removeComPortRow);
    comRow->addWidget(removeButton);

//    auto autoRunCB = new ModeIndexCheckbox("auto",mode,index);
//    connect(autoRunCB, &QCheckBox::stateChanged, this, &FormBuilder::autoRunChanged);
//    comRow->addWidget(autoRunCB);
    comSelector->setMinimumWidth(400);
    comSelector->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    comRow->setSpacing(0);
    dropdownLayout->setContentsMargins(0,0,0,0);
    dropdownLayout->setSpacing(0);
    return comSelector;
}

ComPortRow::ComPortRow(ComPortWidgetController *controller, int index) {
    this->index = index;
    this->controller = controller;
}
