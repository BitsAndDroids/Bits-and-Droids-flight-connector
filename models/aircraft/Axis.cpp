//
// Created by DaveRiedel on 07-Jun-22.
//

#include "Axis.h"

Axis::Axis() {}

Axis::Axis(int min, int max) {
    this->min = min;
    this->max = max;
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

