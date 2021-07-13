#ifndef ACTIVEOUTPUTS_H
#define ACTIVEOUTPUTS_H

#include <QMap>



class ActiveOutputs
{
public:
    ActiveOutputs();
    void changeOutputState(int index);
private:
    QMap<int, bool> activeOutputsMap;
};

#endif // ACTIVEOUTPUTS_H
