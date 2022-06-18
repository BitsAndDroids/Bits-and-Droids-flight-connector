#ifndef ENGINE_H
#define ENGINE_H

#include "Axis.h"

class Engine : public Axis{
 public:


  Engine();

    Engine(float minRange, float idleIndex, float maxRange, InputEnum::DATA_DEFINE_ID_INPUT event);

    Engine(float minRange, float idleIndex, float maxRange, InputTypeEnum type, InputEnum::DATA_DEFINE_ID_INPUT event);

    float getIdleIndex() const { return idleIndex; };


 private:

    float idleIndex;
  int engNmbr;
};

#endif  // ENGINE_H
