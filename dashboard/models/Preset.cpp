//
// Created by dave- on 20-11-2023.
//

#include "Preset.h"

#include <utility>

Preset::Preset(std::string name, std::map<std::string, int> connection): active(false) {
    this->name = std::move(name);
    this->rows = std::move(connection);
}

void Preset::setActive(const bool active) {
    this->active = active;
}

bool Preset::isActive() const {
    return this->active;
}

std::string  Preset::getName() const{
    return this->name;
}

std::map<std::string, int> Preset::getRows() const {
    return this->rows;
}

