#include "models/aircraft/Engine.h"

Engine::Engine(int minRange, int idleIndex, int maxRange, int engNmbr, InputEnum::DATA_DEFINE_ID_INPUT event) : Axis(minRange,maxRange,event) {
  this->idleIndex = idleIndex;
  this->engNmbr = engNmbr;
  this->setType(THROTTLE);
}

Engine::Engine() : Axis(){
  this->idleIndex = 0;
  this->engNmbr = 0;
  this->setType(THROTTLE);
}
