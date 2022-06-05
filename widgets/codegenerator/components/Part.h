//
// Created by Dave Riedel on 01-May-22.
//

#ifndef BITSANDDROIDSGUI_PART_H
#define BITSANDDROIDSGUI_PART_H


#include <string>

class Part {
public:

    explicit Part(std::string name);

private:
    std::string name;
    std::string getName();

};


#endif //BITSANDDROIDSGUI_PART_H
