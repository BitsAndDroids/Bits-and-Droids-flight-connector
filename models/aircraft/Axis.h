//
// Created by DaveRiedel on 07-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_AXIS_H
#define BITSANDDROIDSCONNECTOR_AXIS_H



#include "enums/inputenum.h"
#include "enums/InputTypeEnum.h"

class Axis {
private:
    int currentValue = 0;
    int oldValue = 0;
    InputEnum::DATA_DEFINE_ID_INPUT event;
    InputTypeEnum type = AXIS;
    int mappedValue = 0;
    int oldMappedValue = 0;
    float min = 0;
    float max = 0;

public:
    int getMappedValue() const;

    void setMappedValue(int mappedValueToSet);

    int getOldMappedValue() const;

    void setOldMappedValue(int oldMappedValue);

    Axis();
    Axis(InputEnum::DATA_DEFINE_ID_INPUT event);
    Axis(float min, float max, InputEnum::DATA_DEFINE_ID_INPUT event);
    Axis(float min, float max, InputTypeEnum type, InputEnum::DATA_DEFINE_ID_INPUT event);
    int getCurrentValue() const;

    void setCurrentValue(int currentValue);

    int getOldValue() const;

    void setOldValue(int oldValue);

    float getMin() const;

    void setMin(float min);

    void setMax(float max);

    void setEvent(InputEnum::DATA_DEFINE_ID_INPUT);

    float getMax() const;

    InputTypeEnum getType() const;

    InputEnum::DATA_DEFINE_ID_INPUT getEvent() const;

};


#endif //BITSANDDROIDSCONNECTOR_AXIS_H
