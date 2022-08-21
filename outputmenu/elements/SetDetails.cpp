//
// Created by DaveRiedel on 18-8-2022.
//

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "SetDetails.h"
#include "models/commands/output.h"
#include "elements/MStyleLabels.h"

SetDetails::SetDetails(QWidget *parent): QWidget(parent) {


}
struct QScrollArea * SetDetails::generateGridLayout() {
    auto setDetailsGridLayout = new QGridLayout();

    auto gridWidget = new QWidget();

    uint8_t columnCounter = 0;
    uint8_t rowCounter = 0;
    uint8_t variablesPerCol = 15;

    QMap < int, Output * > outputsInSet = selectedSet->getOutputs();
    QMap<int, Output *>::Iterator i;


    for (i = outputsInSet.begin(); i != outputsInSet.end(); i++) {
        auto variableLabel = new QLabel(i.value()->getCbText());
        if (rowCounter == variablesPerCol) {
            columnCounter++;
            rowCounter = 0;
        }
        setDetailsGridLayout->addWidget(variableLabel, rowCounter, columnCounter);
        rowCounter++;
    }

    setDetailsGridLayout->setAlignment({Qt::AlignTop, Qt::AlignLeft});

    gridWidget->setLayout(setDetailsGridLayout);


    auto *activeScroll = new QScrollArea();


    activeScroll->setWidget(gridWidget);
    activeScroll->setStyleSheet(
            "border-radius:5px;"
            "background-color:#fff;");
    return activeScroll;
}

SetDetails* SetDetails::generateSetDetails(Set setToShow) {
    this->selectedSet = &setToShow;
    auto setDetailsContainerLayout = new QVBoxLayout(this);
    this->setLayout(setDetailsContainerLayout);
    setDetailsContainerLayout->setAlignment({Qt::AlignTop, Qt::AlignLeft});
    this->layout()->setAlignment({Qt::AlignTop, Qt::AlignLeft});
    this->layout()->addWidget(new MStyleLabels(setToShow.getSetName(), StyleEnum::HEADER));
    this->layout()->addWidget(generateGridLayout());
    this->setFixedWidth(375);
    this->setObjectName("setDetailsContainer");
    return this;
}

