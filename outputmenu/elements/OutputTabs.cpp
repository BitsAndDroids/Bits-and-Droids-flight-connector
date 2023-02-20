//
// Created by DaveRiedel on 18-8-2022.
//

#include <QGridLayout>
#include <QCheckBox>
#include <iostream>
#include "OutputTabs.h"
#include "handlers/outputhandler.h"
#include "elements/MPushButton.h"
#include "outputmenu/handlers/sethandler.h"

OutputTabs::OutputTabs() : QWidget() {


    auto tabLayout = new QVBoxLayout();
    this->setLayout(tabLayout);
    auto tabHLayout = new QHBoxLayout();
    tabLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    tabLayout->addLayout(tabHLayout);
    outputTabsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    auto saveButton = new MPushButton(tabHLayout);
    saveButton->setObjectName("saveButton");
    saveButton->setIconWithPath(":/resources/images/save.svg");
    saveButton->setToolTip("Save set");
    auto buttonVLayout = new QVBoxLayout();
    buttonVLayout->setObjectName("buttonVLayout");
    buttonVLayout->addItem(new QSpacerItem(40, 30, QSizePolicy::Fixed, QSizePolicy::Fixed));
    buttonVLayout->
            addWidget(saveButton);
    buttonVLayout->setAlignment({
                                        Qt::AlignTop, Qt::AlignLeft});

//TODO add restore to last state button
    tabHLayout->
            addLayout(buttonVLayout);
    tabHLayout->
            addWidget(outputTabsWidget);

//TODO create a stylesheet file for this widget
    outputTabsWidget->setStyleSheet("QTabWidget::pane{"
                              "border-radius:5px;"
                              "background-color:#fff;"
                              "}"
                              "QTabWidget::tab-bar{"
                              "border-top-left-radius:5px;"
                              "border-top-right-radius:5px;"
                              "left:5px;"
                              "padding-left:10px;"
                              "}"
                              "QTabBar::tab:!selected {"
                              "color:#000;"
                              "background-color:#e8e8e8;"
                              "margin-top:0px #fff;"
                              "}"
                              "QTabBar::tab:selected {"
                              "font-weight:bold;"
                              "background-color:#fff;"
                              "color:#000;"
                              "margin-left: -4px;"
                              "margin-right: -4px;"
                              "}"
                              "QTabBar::tab {"
                              "padding:5px;"
                              "border-top-left-radius: 5px;"
                              "border-top-right-radius: 5px;"
                              "border-top: 1px solid #b8b8b8;"
                              "border-right: 1px solid #b8b8b8;"
                              "}"
                              "QTabBar::tab:first:selected {"
                              "    margin-left: 0;"
                              "}"

                              "QTabBar::tab:last:selected {"
                              "    margin-right: 0;"
                              "}"
                              "QTabBar::scroller{"
                              "width:50px;"
                              "background-color:#fff;"
                              "}"
                              "QTabBar QToolButton{"
                              "width:50px;"
                              "background-color:#fff;"
                              "border:none;"
                              "bottom:-4px;"
                              "border-top-left-radius: 4px;"
                              "border-top-right-radius: 4px;"
                              "}");
    outputTabsWidget->setMinimumWidth(600);

    tabLayout->setAlignment({
                                    Qt::AlignTop, Qt::AlignLeft});
    tabHLayout->setAlignment({
                                     Qt::AlignTop, Qt::AlignLeft});

    auto *outputHandler = new class OutputHandler();
    std::vector<Category> categorizedOutputs = outputHandler->getOutputsCategorized();
    std::cout << "CategoryOutputs list size: " << categorizedOutputs.size() << std::endl;
    //categorizedOutputs is a 2d List of outputs, each output is in a category
    //each category receives its own tab
    uint8_t amountOfCheckboxesPerColumn = 10;
    for (auto & categorizedOutput : categorizedOutputs) {
        auto *newTab = new QWidget();
        auto *cbGridLayout = new QGridLayout(newTab);

        cbGridLayout->setAlignment({Qt::AlignTop, Qt::AlignLeft});
        newTab->setStyleSheet("border-radius:5px;"
                              "border-top-right-radius:0px;"
                              "background-color:#fff;");
        uint8_t row = 0;
        uint8_t column = 0;

        for(auto & output : categorizedOutput.getOutputs()){
            std::cout << "output: " << output.getCbText().toStdString() << std::endl;
            auto *checkbox = new QCheckBox();
            checkbox->setMinimumHeight(20);

            QString cbText = output.getCbText();
            checkbox->setText(QString(cbText));
            checkbox->setObjectName(
                    "cb" + QString::number(output.getId()));
            cbGridLayout->addWidget(checkbox, row, column);
            row++;
            if(row >= amountOfCheckboxesPerColumn){
                row = 0;
                column++;
            }
        }

        std::cout << "end of category" << std::endl;
        std::cout << "end of tab" << std::endl;

        outputTabsWidget->addTab( newTab, QString::fromStdString(categorizedOutput.getCategoryName()));
        std::cout << "end of addtab" << std::endl;
    }
}

OutputTabs::~OutputTabs() {
}
