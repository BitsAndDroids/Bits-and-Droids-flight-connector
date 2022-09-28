//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortRow.h"
#include "elements/ModeIndexCombobox.h"
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSerialPort>

QWidget *ComPortRow::generateElement() {
    auto *comSelector = new QWidget();
    comSelector->setObjectName("comRowWidget");
    //TODO move stylesheets to seperate files
    comSelector->setStyleSheet("QWidget#comRowWidget {"
                               "background-color: #fff;"
                               "border-radius: 5px;"
                               "margin: 5px;"
                               "}");
    auto *comRow = new QHBoxLayout();
    comSelector->setLayout(comRow);
    auto *comPortComboBox = new ModeIndexCombobox("comBox",index);
    ComPortWidgetModel model = ComPortWidgetModel();
    auto availableComPorts = model.loadAvailableComPorts();
    auto availableSets = model.getAvailableSets();

    for (auto &availableComPort: availableComPorts) {
        comPortComboBox->addItem(availableComPort);
    }
    comRow->addWidget(comPortComboBox);
    comPortComboBox->setMinimumWidth(150);
    if (setsNeeded) {
        auto *setComboBox = new ModeIndexCombobox("setBox",index);
        setComboBox->addItem("No outputs");
        for (const auto &availableSet: *availableSets) {
            setComboBox->addItem(availableSet.getSetName());
        }
        setComboBox->setMinimumWidth(150);
        comRow->addWidget(setComboBox);
        qDebug() << setComboBox->objectName();
    }

    auto *removeButton = new QPushButton("-");
    removeButton->setObjectName("del" + QString::number(index));
    removeButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    removeButton->setMinimumSize(20, 20);
    removeButton->setMaximumSize(20, 20);
    QObject::connect(removeButton, &QAbstractButton::clicked, controller,
            &ComPortWidgetController::removeComPortRow);
    comRow->addWidget(removeButton);

//    auto autoRunCB = new ModeIndexCheckbox("auto",mode,index);
//    connect(autoRunCB, &QCheckBox::stateChanged, this, &FormBuilder::autoRunChanged);
//    comRow->addWidget(autoRunCB);
    comSelector->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    comSelector->setMinimumSize(400, 50);
    return comSelector;
}

ComPortRow::ComPortRow(ComPortWidgetController *controller, int index) {
    this->index = index;
    this->controller = controller;
}
