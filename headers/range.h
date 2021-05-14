#ifndef RANGE_H
#define RANGE_H

class Range {
 public:
  Range(int minRange, int maxRange);
  int getMinRange() { return minRange; };
  int getMaxRange() { return maxRange; };

  Range();

 private:
  int minRange;
  int maxRange;
};

#endif  // RANGE_H
