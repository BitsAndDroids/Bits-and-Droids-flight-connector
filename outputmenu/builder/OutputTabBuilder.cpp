//
// Created by DaveRiedel on 18-8-2022.
//

#include "OutputTabBuilder.h"
#include "outputmenu/elements/OutputTabs.h"

QWidget *OutputTabBuilder::buildOutputTabContainer() {
    auto outputTabContainer = new OutputTabs(this);
    return outputTabContainer;
}

OutputTabBuilder::OutputTabBuilder(QWidget *parent): QWidget(parent) {

}
