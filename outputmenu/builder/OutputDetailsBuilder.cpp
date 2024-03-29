//
// Created by DaveRiedel on 20-8-2022.
//

#include "OutputDetailsBuilder.h"

#include <utility>
#include "outputmenu/elements/SetDetails.h"
#include "outputmenu/handlers/sethandler.h"


OutputDetailsBuilder::OutputDetailsBuilder(QWidget *parent): QWidget(parent) {

}

QWidget* OutputDetailsBuilder::buildOutputDetailsContainer(QString id) {
    auto setDetails = new SetDetails(this);
    SetHandler setHandler = SetHandler();
    setToShow = setHandler.getSetById(std::move(id));
    auto setDetailsContainer = setDetails->generateSetDetails(this->setToShow);

    return setDetailsContainer;
}
QWidget* OutputDetailsBuilder::buildOutputDetailsContainer() {
    auto setDetails = new SetDetails(this);
    auto setPlaceholder = new Set();
    setPlaceholder->setSetId(-1);
    auto setDetailsContainer = setDetails->generateSetDetails(*setPlaceholder);
    return setDetailsContainer;
}

QString OutputDetailsBuilder::getCurrentSetID() {
    return QString::number(setToShow.getID());
}


