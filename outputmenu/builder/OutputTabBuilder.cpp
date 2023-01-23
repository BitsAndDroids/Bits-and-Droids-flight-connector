//
// Created by DaveRiedel on 18-8-2022.
//

#include <QHBoxLayout>
#include <QCheckBox>
#include <iostream>
#include <QPushButton>
#include "OutputTabBuilder.h"
#include "outputmenu/elements/OutputTabs.h"
#include "outputmenu/handlers/sethandler.h"
#include "logging/MessageCaster.h"

QWidget *OutputTabBuilder::buildOutputTabContainer() {
    auto outputTabContainer = new OutputTabs(this);
    outputTabContainer->setObjectName("outputTabWidget");
    auto saveButton = this->findChild<QPushButton *>("saveButton");
    QObject::connect(saveButton, &QPushButton::clicked, this, &OutputTabBuilder::saveEditedSet);
    return outputTabContainer;
}

OutputTabBuilder::OutputTabBuilder(QWidget *parent) : QWidget(parent) {
}

void OutputTabBuilder::setCheckboxes(const QString& id) {
    activeSetId = id;
    auto checkBoxes = parent()->findChildren<QCheckBox *>();
    for(auto checkBox : checkBoxes) {
            checkBox->setChecked(false);
    }
    auto setHandler = SetHandler();
    auto setFound = setHandler.getSetById(id);
    QMap<int, Output *> outputsToToggle = setFound.getOutputs();
    QMap<int, Output *>::iterator i;

    for (i = outputsToToggle.begin(); i != outputsToToggle.end(); i++) {
        if (parent()->findChild<QCheckBox *>("cb" +
                                         QString::number(i.value()->getId()))) {

            parent()->findChild<QCheckBox *>("cb" + QString::number(i.value()->getId()))->setChecked(true);
        }
    }
}

void OutputTabBuilder::saveEditedSet(){
    auto setHandler = SetHandler();
    auto outputHandler = new class outputHandler();
    auto setToEdit = setHandler.getSetById(activeSetId);
    setToEdit.clearOutputs();
    auto checkBoxes = parent()->findChildren<QCheckBox *>();
    for (auto &i: checkBoxes) {
        if (i->isChecked()) {
            QString cbName = i->objectName();
            if (outputHandler->findOutputById(cbName.mid(2).toInt())) {
                Output *outputSelected =
                        outputHandler->findOutputById(cbName.mid(2).toInt());
                setToEdit.addOutput(outputSelected);
            }
        }
    }
    setHandler.saveSet(&setToEdit);
    emit setEdited(activeSetId);
    MessageCaster::showCompleteMessage("Set saved");
}
