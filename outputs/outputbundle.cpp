#include "outputbundle.h"

outputBundle::outputBundle() {}

bool outputBundle::isOutputInBundle(int id) {
  return this->outputsInSet.contains(id);
}
