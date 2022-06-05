#ifndef ENGINE_H
#define ENGINE_H
class Engine {
 public:
  Engine(int minRange, int idleIndex, int maxRange, int engNmbr);
  Engine();
  int getMinRange() const { return minRange; };
  int getMaxRange() const { return maxRange; };
  int getIdleIndex() const { return idleIndex; };
  int getEngNmbr() const { return engNmbr; };

 private:
  int minRange;
  int idleIndex;
  int maxRange;
  int engNmbr;
};

#endif  // ENGINE_H
