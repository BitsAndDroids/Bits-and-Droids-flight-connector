//
// Created by DaveRiedel on 07-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_AXIS_H
#define BITSANDDROIDSCONNECTOR_AXIS_H


class Axis {
private:
    int currentValue = 0;
    int oldValue = 0;
    int min = 0;
    int max = 0;

public:
    Axis();
    Axis(int min, int max);

    int getCurrentValue() const;

    void setCurrentValue(int currentValue);

    int getOldValue() const;

    void setOldValue(int oldValue);

    int getMin() const;

    void setMin(int min);

    void setMax(int max);

//setters

    int getMax() const;
};


#endif //BITSANDDROIDSCONNECTOR_AXIS_H
