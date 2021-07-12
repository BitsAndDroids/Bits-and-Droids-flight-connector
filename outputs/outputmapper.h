#ifndef OUTPUTMAPPER_H
#define OUTPUTMAPPER_H

#include "output.h"

#include <qstring.h>

#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>
#include <headers/SimConnect.h>

class outputMapper
{
public:
    outputMapper();
    void mapOutputs(QList<Output> outputToMap, HANDLE outputConnect);
private:
    void mapOutputs();
};

#endif // OUTPUTMAPPER_H
