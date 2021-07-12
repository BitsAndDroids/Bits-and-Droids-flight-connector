#ifndef SET_H
#define SET_H

#include "output.h"

#include <QList>
#include <QMap>
#include <QMap>
#include <qjsonobject.h>



class set
{

public:
    set();
    set(QString setName);


    //void addOutput(Output outputToAdd);

    QMap<int,Output*> getOutputs(){return outputs;};
    int getID() const{return setId;};
        QString getSetName() const{return setName;};

    void setOutputs(QMap<int, Output *> newOutputs);
    void setSetName(QString name){this->setName = name;};
    void setSetId(int id){this->setId = id;};
    void deleteSet();

     void saveSet();




     set(QString setName, int id);
     set fromJson(QJsonDocument *docToConvert);

     set(QString setName, int id, QMap<int, Output *>);

     void addOutput(Output *outputToAdd);
     void clearOutputs();
private:
    QString setName;
    int setId;
    QMap<int, Output*> outputs;
};


#endif // SET_H
