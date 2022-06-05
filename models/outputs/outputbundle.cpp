#include "outputbundle.h"

outputBundle::outputBundle() {}
// test
bool outputBundle::isOutputInBundle(int id) {
  return this->outputsInSet.contains(id);
}
