//
// Created by DaveRiedel on 07-Jun-22.
//

#include "Axis.h"

Axis::Axis() {}

Axis::Axis(float min, float max) {
    this->min = min;
    this->max = max;
}
Axis::Axis(float min, float max, InputEnum::DATA_DEFINE_ID_INPUT event) {
    this->min = min;
    this->max = max;
    this->event = event;
}
Axis::Axis(float min, float max, InputTypeEnum type, InputEnum::DATA_DEFINE_ID_INPUT event) {
    this->min = min;
    this->max = max;
    this->type = type;
    this->event = event;
}
int Axis::getCurrentValue() const {
    return currentValue;
}

void Axis::setCurrentValue(int currentValue) {
    this->currentValue = currentValue;
}

int Axis::getOldValue() const {
    return oldValue;
}

void Axis::setOldValue(int oldValue) {
    this->oldValue = oldValue;
}

float Axis::getMin() const {
    return min;
}

void Axis::setMin(float min) {
    this->min = min;
}

float Axis::getMax() const {
    return max;
}

void Axis::setMax(float max) {
    Axis::max = max;
}

int Axis::getMappedValue() const {
    return mappedValue;
}

void Axis::setMappedValue(int mappedValueToSet) {
    this->mappedValue = mappedValueToSet;
}

int Axis::getOldMappedValue() const {
    return oldMappedValue;
}

void Axis::setOldMappedValue(int oldMappedValue) {
    Axis::oldMappedValue = oldMappedValue;
}

void Axis::setEvent(InputEnum::DATA_DEFINE_ID_INPUT event) {
    this->event = event;
}

InputTypeEnum Axis::getType() const {
    return type;
}

InputEnum::DATA_DEFINE_ID_INPUT Axis::getEvent() const {
    return event;
}

Axis::Axis(InputEnum::DATA_DEFINE_ID_INPUT event) {

}

