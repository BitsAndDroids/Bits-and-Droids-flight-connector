#ifndef OUTPUT_H
#define OUTPUT_H

#include <QJsonObject>
#include <qstring.h>



class Output

{

public:
    Output(int id,
    std::string outputName,
    std::string metric,
    float updateEvery,
    int dataType,
           QString cbText);

    std::string getOutputName(){return outputName;};
    int getId(){return this->id;};
    QString getCbText(){return cbText;};
    QJsonObject toJson() const;

private:
    int id;
    std::string outputName;
    std::string metric;
    float updateEvery;
    int dataType;
    QString cbText;
};


#endif // OUTPUT_H
