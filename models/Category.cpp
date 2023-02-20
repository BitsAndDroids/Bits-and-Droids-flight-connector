//
// Created by dave- on 20-2-2023.
//

#include "Category.h"

#include <utility>

Category::Category(std::string categoryName, std::vector<Output> outputs) {
    this->categoryName = std::move(categoryName);
    this->outputs = std::move(outputs);
}

std::string Category::getCategoryName() const {
    return categoryName;
}

void Category::setCategoryName(std::string categoryName) {
    Category::categoryName = categoryName;
}

std::vector<Output> Category::getOutputs() const {
    return outputs;
}

void Category::setOutputs(std::vector<Output> outputs) {
    Category::outputs = outputs;
}


