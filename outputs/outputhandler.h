#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include "output.h"

#include <QJsonObject>
#include <QMap>



class outputHandler
{
public:
    outputHandler();
    QStringList getCategoryStrings(){return categoryStrings;};
    QList<QList<Output>> getOutputsCategorized(){return outputsCategorized;};
    QMap<int,Output*> getAvailableOutputs(){return availableOutputs;};
    Output* findOutputById(int id);
private:
    //const QJsonObject &json
    QStringList categoryStrings;
    QList<QList<Output>> outputsCategorized;
    QMap<int,Output*> availableOutputs;
    void readOutputs();
};

#endif // OUTPUTHANDLER_H
