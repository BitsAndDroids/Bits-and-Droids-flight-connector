//
// Created by DaveRiedel on 07-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_AXIS_H
#define BITSANDDROIDSCONNECTOR_AXIS_H


#include <enums/AxisTypeEnum.h>
#include <enums/inputenum.h>

class Axis {
private:
    int currentValue = 0;
    int oldValue = 0;
    int min = 0;
    int max = 0;
    InputEnum event;
    AxisTypes axisType = AXIS;

public:
    Axis();
    Axis(int min, int max, InputEnum event);

    int getCurrentValue() const;

    void setCurrentValue(int currentValue);

    int getOldValue() const;

    void setOldValue(int oldValue);

    int getMin() const;

    void setMin(int min);

    void setMax(int max);

    int getMax() const;

    void setType(AxisTypes type);
};


#endif //BITSANDDROIDSCONNECTOR_AXIS_H
