//
// Created by Dave Riedel on 05-Sep-21.
//

#include "coordinates.h"

coordinates::coordinates(float xToSet, float yToSet) {
  x = xToSet;
  y = yToSet;
}

float coordinates::getX() const { return x; }
float coordinates::getY() const { return y; }
void coordinates::setX(float newX) { x = newX; }
void coordinates::setY(float newY) { y = newY; }
