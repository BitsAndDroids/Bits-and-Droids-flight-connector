//
// Created by dave- on 20-2-2023.
//

#ifndef BITSANDDROIDSCONNECTOR_CATEGORY_H
#define BITSANDDROIDSCONNECTOR_CATEGORY_H


#include <string>
#include <vector>
#include "models/commands/output.h"

class Category {
public:
    Category(std::string categoryName, std::vector<Output> outputs);
    std::string getCategoryName() const;
    void setCategoryName(std::string categoryName);
    std::vector<Output> getOutputs() const;
    void setOutputs(std::vector<Output> outputs);

private:
    std::string categoryName;
    std::vector<Output> outputs;

};


#endif //BITSANDDROIDSCONNECTOR_CATEGORY_H
