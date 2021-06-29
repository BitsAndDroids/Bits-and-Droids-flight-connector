#ifndef SET_H
#define SET_H

#include "output.h"



class set
{
public:
    set(QString setName);
    const QList<Output> &getOutputs() const;
    void setOutputs(const QList<Output> &newOutputs);
    void saveSet();
    void deleteSet();



private:
    QString setName;
    int setId;
    QList<Output>* outputs;
};

#endif // SET_H
