//
// Created by DaveRiedel on 18-Jun-22.
//

#include "Input.h"
#include "enums/inputenum.h"


Input::Input() {
    //iterate over inputenum  and add to list

}

Input::Input(int type, std::string event, int prefix) {
    this->type = type;
    this->event = event;
    this->prefix = prefix;
}

int Input::getType() const {
    return type;
}

const std::string &Input::getEvent() const {
    return event;
}

int Input::getPrefix() const {
    return prefix;
}

