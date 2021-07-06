#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include "output.h"

#include <QJsonObject>



class outputHandler
{
public:
    outputHandler();
    QStringList getCategoryStrings(){return categoryStrings;};
    QList<QList<Output>> getOutputsCategorized(){return outputsCategorized;};
    QList<Output> getAvailableOutputs(){return availableOutputs;};
private:
    //const QJsonObject &json
    QStringList categoryStrings;
    QList<QList<Output>> outputsCategorized;
    QList<Output> availableOutputs;
    void readOutputs();
};

#endif // OUTPUTHANDLER_H
