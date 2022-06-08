//
// Created by DaveRiedel on 07-Jun-22.
//

#include "Axis.h"

Axis::Axis() {}

Axis::Axis(int min, int max, InputEnum event) {
    this->min = min;
    this->max = max;
    this->event = event;
}

int Axis::getCurrentValue() const {
    return currentValue;
}

void Axis::setCurrentValue(int currentValue) {
    Axis::currentValue = currentValue;
}

int Axis::getOldValue() const {
    return oldValue;
}

void Axis::setOldValue(int oldValue) {
    Axis::oldValue = oldValue;
}

int Axis::getMin() const {
    return min;
}

void Axis::setMin(int min) {
    Axis::min = min;
}

int Axis::getMax() const {
    return max;
}

void Axis::setMax(int max) {
    Axis::max = max;
}

void Axis::setType(AxisTypes type) {
    this->axisType = type;
}

