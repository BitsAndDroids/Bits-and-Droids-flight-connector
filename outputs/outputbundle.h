#ifndef OUTPUTBUNDLE_H
#define OUTPUTBUNDLE_H

#include "set.h"

#include <headers/SerialPort.hpp>



class outputBundle
{
public:
    outputBundle();
    bool isOutputInBundle(int id);
private:
    set set;
    SerialPort *arduino;
    QList<Output*> outputsInSet;
};

#endif // OUTPUTBUNDLE_H
