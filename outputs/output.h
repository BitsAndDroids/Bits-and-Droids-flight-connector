#ifndef OUTPUT_H
#define OUTPUT_H

#include <qstring.h>



class Output
{
public:
    Output(QString cbName,
           QString cbText);
private:
    QString cbName;
    QString cbText;
};

#endif // OUTPUT_H
