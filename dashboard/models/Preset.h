//
// Created by dave- on 20-11-2023.
//

#ifndef PRESET_H
#define PRESET_H
#include <map>
#include <string>


class Preset {
public:
    Preset(std::string name, std::map<std::string, int> connection);
    Preset() = default;

    void setActive(bool active);

    [[nodiscard]] bool isActive() const;
    std::string getName() const;
    [[nodiscard]] std::map<std::string, int> getRows() const;
private:
    std::string name;
    std::string comport;
    bool active;
    std::map<std::string, int> rows;
};



#endif //PRESET_H
