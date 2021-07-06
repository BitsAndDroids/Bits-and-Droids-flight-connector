#ifndef OUTPUT_H
#define OUTPUT_H

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
    QString getCbText(){return cbText;};
private:
    int id;
    std::string outputName;
    std::string metric;
    float updateEvery;
    int dataType;
    QString cbText;
};

#endif // OUTPUT_H
