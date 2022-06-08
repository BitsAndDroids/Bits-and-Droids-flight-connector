#include "models/aircraft/Engine.h"

Engine::Engine(int minRange, int idleIndex, int maxRange, int engNmbr) : Axis(minRange,maxRange) {
  this->idleIndex = idleIndex;
  this->engNmbr = engNmbr;
}

Engine::Engine() : Axis(){
  this->idleIndex = 0;
  this->engNmbr = 0;
}
