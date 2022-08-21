//
// Created by DaveRiedel on 17-8-2022.
//

#include "SetrowBuilder.h"
#include "models/commands/Set.h"

void editSet();

QWidget* SetrowBuilder::buildSetrowContainer(){
    auto setHandler = SetHandler();
    auto *foundSets = setHandler.getSets();
    auto setContainer = new QWidget();
    setContainer->setObjectName("SetListContainer");
    auto setContainerVLayout = new QVBoxLayout(setContainer);
    setContainerVLayout->addItem(new QSpacerItem(0, 30, QSizePolicy::Fixed, QSizePolicy::Fixed));
    for(auto & i : *foundSets){
        buildSetrow(i, setContainerVLayout);
    }
    for(auto & row : setRows){
        QObject::connect(row, &SetRow::clickedSignal, parent, &OutputMenu::showSetDetails);
        QObject::connect(row, &SetRow::editSetSignal, parent, &OutputMenu::editSet);
    }
    setContainerVLayout->setAlignment(Qt::AlignTop);
    setContainer->setStyleSheet(
                        "border-radius:5px;"
                        "background-color:#487f94;");
    setContainer->setFixedWidth(375);

    return setContainer;
}
void SetrowBuilder::buildSetrow(const Set setForRow, QVBoxLayout* parent) {
    auto setrow = new SetRow(setForRow);
    setRows.push_back(setrow);
    parent->addWidget(setrow);
}



SetrowBuilder::SetrowBuilder(OutputMenu *parent) {
    this->parent = parent;
}
