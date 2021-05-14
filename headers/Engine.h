#ifndef ENGINE_H
#define ENGINE_H
class Engine {
 public:
  Engine(int minRange, int idleIndex, int maxRange, int engNmbr);
  Engine();
  int getMinRange() { return minRange; };
  int getMaxRange() { return maxRange; };
  int getIdleIndex() { return idleIndex; };
  int getEngNmbr() { return engNmbr; };

 private:
  int minRange;
  int idleIndex;
  int maxRange;
  int engNmbr;
};

#endif  // ENGINE_H
