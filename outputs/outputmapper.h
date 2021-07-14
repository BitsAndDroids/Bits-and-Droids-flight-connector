#ifndef OUTPUTMAPPER_H
#define OUTPUTMAPPER_H

#include <headers/SimConnect.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qstring.h>
#include <tchar.h>
#include <windows.h>

#include "output.h"

class outputMapper {
 public:
  outputMapper();

  void mapOutputs(QList<Output *> outputToMap, HANDLE outputConnect);

 private:
  void mapOutputs();
};

#endif  // OUTPUTMAPPER_H
