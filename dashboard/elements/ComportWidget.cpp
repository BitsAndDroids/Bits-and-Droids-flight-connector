//
// Created by DaveRiedel on 19-9-2022.
//

#include "ComPortWidget.h"

ComPortWidget::ComPortWidget(QMainWindow *parent, ComPortWidgetController *controller) {
    this->controller = controller;
    this->parent = parent;
}
