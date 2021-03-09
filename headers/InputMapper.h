#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H
#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qwaitcondition.h>
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <string>

#include "SimConnect.h"
#include "inputenum.h"
#include "stdio.h"
class InputMapper {
 public:
  HANDLE connect;
  InputMapper();
  void mapEvents(HANDLE mapConnect);
};

#endif  // INPUTMAPPER_H
