#include "../headers/range.h"

Range::Range(int minRange, int maxRange) {
  this->minRange = minRange;
  this->maxRange = maxRange;
}
Range::Range() {
  this->minRange = 0;
  this->maxRange = 1023;
}
