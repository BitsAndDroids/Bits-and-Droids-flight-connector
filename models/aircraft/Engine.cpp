#include "models/aircraft/Engine.h"
Engine::Engine(int minRange, int idleIndex, int maxRange, int engNmbr) {
  this->minRange = minRange;
  this->idleIndex = idleIndex;
  this->maxRange = maxRange;
  this->engNmbr = engNmbr;
}
Engine::Engine() {
  this->minRange = 0;
  this->idleIndex = 0;
  this->maxRange = 0;
  this->engNmbr = 0;
}
