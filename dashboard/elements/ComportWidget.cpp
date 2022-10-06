//
// Created by DaveRiedel on 19-9-2022.
//

#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include "ComPortButtonRow.h"
#include "ComPortWidget.h"
#include "dashboard/controller/ComPortWidgetController.h"

ComPortWidget::ComPortWidget(QMainWindow *parent, ComPortWidgetController *controller) {
    this->controller = controller;
    this->parent = parent;
}

QWidget* ComPortWidget::generateElement() {
    auto *widget = new QWidget(parent);
    widget->setObjectName("comPortWidget");
    widget->setStyleSheet("QWidget#comPortWidget{ border-radius:4px;}");
    auto containerLayout = new QVBoxLayout(widget);
    auto layout = new QVBoxLayout();
    containerLayout->addLayout(layout);
    containerLayout->insertStretch( -1, 1 );

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
    layout->setAlignment(Qt::AlignTop);

    //This box lets us display error messages to the user
    //i.e. no ports selected, no ports found, etc.
    auto warningBox = new QVBoxLayout();
    warningBox->setObjectName("warningBox");

    containerLayout->addLayout(warningBox);

    controller->refresh();

    comRowLayout->setAlignment(Qt::AlignTop);
    comRowLayout->setSpacing(0);
    widget->adjustSize();
    return widget;
}
