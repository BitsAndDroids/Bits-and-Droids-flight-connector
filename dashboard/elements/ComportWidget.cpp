//
// Created by DaveRiedel on 19-9-2022.
//

#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include "ComPortButtonRow.h"
#include "ComPortWidget.h"
#include "ComPortRow.h"
#include "dashboard/controller/ComPortWidgetController.h"

ComPortWidget::ComPortWidget(QMainWindow *parent, ComPortWidgetController *controller) {
    this->controller = controller;
    this->parent = parent;

}

QWidget* ComPortWidget::generateElement() {
    auto *widget = new QWidget(parent);
    auto layout = new QVBoxLayout(widget);

    auto *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(2, 2);
    widget->setGraphicsEffect(shadow);

    layout->setAlignment(Qt::AlignTop);

    //COM CONTROLS
    auto *comPortButtonRow = new ComPortButtonRow(controller, 0);
    layout->addWidget(comPortButtonRow->generateElement());

    //COM PORTS
    auto comRowLayout = new QVBoxLayout();
    comRowLayout->setAlignment(Qt::AlignTop);
    layout->addLayout(comRowLayout);
    layout->setObjectName("comRowsContainer");

    //This box lets us display error messages to the user
    //i.e. no ports selected, no ports found, etc.
    auto warningBox = new QVBoxLayout();
    warningBox->setObjectName("warningBox");
    layout->addLayout(warningBox);

    auto settingsHandler = new SettingsHandler();
    auto foundComports = settingsHandler->retrieveKeys("comPorts");
    for (int i = 0; i < foundComports->size(); ++i) {
        auto comPortRow = new ComPortRow(controller, i);
        comRowLayout->addWidget(comPortRow->generateElement());
        widget->adjustSize();
    }



    //widget sizing
    widget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    widget->adjustSize();
    widget->updateGeometry();
    return widget;
}
