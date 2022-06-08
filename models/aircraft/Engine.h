#ifndef ENGINE_H
#define ENGINE_H

#include "Axis.h"

class Engine : public Axis{
 public:

    Engine(int minRange, int maxRange, int idleIndex, int engNmbr, InputEnum::DATA_DEFINE_ID_INPUT event);
  Engine();

  int getIdleIndex() const { return idleIndex; };
  int getEngNmbr() const { return engNmbr; };

 private:

  int idleIndex;
  int engNmbr;
};

#endif  // ENGINE_H
