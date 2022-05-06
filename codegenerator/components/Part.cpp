//
// Created by Dave Riedel on 01-May-22.
//

#include "Part.h"
#include <utility>

Part::Part(std::string name) {
    this->name = std::move(name);
}

std::string Part::getName() {
    return name;
}
