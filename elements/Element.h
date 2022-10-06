//
// Created by DaveRiedel on 19-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_ELEMENT_H
#define BITSANDDROIDSCONNECTOR_ELEMENT_H


#include <QWidget>

class Element {
public:
    virtual QWidget* generateElement() = 0;
};


#endif //BITSANDDROIDSCONNECTOR_ELEMENT_H
