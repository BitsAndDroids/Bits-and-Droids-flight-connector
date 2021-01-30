#ifndef CONNECTTHREAD_H
#define CONNECTTHREAD_H

#include <cstdio>
#include <iomanip>
#include <sstream>

#include "stdio.h"
#include <QMutex>
#include <QObject>
#include <QWaitCondition>
#include <SerialPort.hpp>
#include <qthread.h>
#include <string>
#include "SimConnect.h"
#include "connectthread.cpp"
using namespace std;


class connectthread: public QThread
{
    Q_OBJECT

public:
    connectthread();
    ~connectthread();

    void setPortName(const char* name);

    bool cbAPAltitudeLock = false;

    bool cbAPHeadingLock = false;
    bool cbAPVerticalLock = false;
    bool cbActiveCom1 = false;
    bool cbActiveCom2 = false;
    bool cbStandbyCom1 = false;
    bool cbStandbyCom2 = false;
    bool cbIndicatedAirspeed = false;
    bool cbIndicatedAltitude = false;
    bool cbIndicatedHeading = false;
    bool cbIndicatedVerticalSpeed = false;
    bool cbKohlman = false;
    bool cbBarometerPressure = false;
    bool cbGPSGroundspeed = false;

public slots:



signals:
    void sendToArduino(float received, string prefix);


protected:
    void run() override;

private:

    QMutex mutex;
    QWaitCondition condition;

    float prevSpeed = 0.0f;
    float currentSpeed;
    int eps = 1;

    Struct1 struct1;
    int quit = 0;


    void testDataRequest();
    bool restart = false;
    bool abort = false;


};



#endif // CONNECTTHREAD_H
