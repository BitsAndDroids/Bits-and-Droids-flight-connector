#include "models/aircraft/Engine.h"


Engine::Engine(
        float minRange,
        float idleIndex,
        float maxRange,
        InputEnum::DATA_DEFINE_ID_INPUT event) : Axis(minRange,maxRange, event) {
    this->idleIndex = idleIndex;
    this->engNmbr = engNmbr;
}
Engine::Engine(
        float minRange,
        float idleIndex,
        float maxRange,
        InputTypeEnum type,
        InputEnum::DATA_DEFINE_ID_INPUT event) : Axis(minRange,maxRange,type, event) {
    this->idleIndex = idleIndex;
    this->engNmbr = engNmbr;
}

Engine::Engine() : Axis(){
  this->idleIndex = 0;
  this->engNmbr = 0;
}
