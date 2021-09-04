#ifndef SET_H
#define SET_H

#include <qjsonobject.h>

#include <QList>
#include <QMap>
#include <utility>

#include "output.h"

class set {
public:
    set();

    explicit set(QString setName);

    // void addOutput(Output outputToAdd);

    QMap<int, Output *> getOutputs() { return outputs; };

    int getID() const { return setId; };

    QString getSetName() const { return setName; };

    void setOutputs(QMap<int, Output *> newOutputs);

    void setSetName(QString name) { this->setName = std::move(name); };

    void setSetId(int id) { this->setId = id; };

    void deleteSet();

    void saveSet();

    set(QString setName, int id);

    static set fromJson(QJsonDocument *docToConvert);

    set(QString setName, int id, QMap<int, Output *>);

    void addOutput(Output *outputToAdd);

    void clearOutputs();

private:
    QString setName;
    int setId = 0;
    QMap<int, Output *> outputs;
};

#endif  // SET_H
