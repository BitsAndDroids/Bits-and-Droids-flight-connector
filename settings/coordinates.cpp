//
// Created by Dave Riedel on 05-Sep-21.
//

#include "coordinates.h"

coordinates::coordinates(float x, float y) {
    this->x = x;
    this->y = y;
}

float coordinates::getX() {
    return this->x;
}
float coordinates::getY() {
    return this->y;
}
void coordinates::setX(float x) {
    this->x = x;
}
void coordinates::setY(float y) {
    this->y = y;
}
