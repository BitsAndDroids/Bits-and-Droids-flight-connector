#ifndef PLATFORM_H
#define PLATFORM_H
#include<iostream>
#include <QString>

class Platform
{
private:
    int baudRate;
    QString name;
    bool skipHardwareSerial;
public:
    Platform(int baudRate, QString name, bool skipHardwareSerial);

};

#endif // PLATFORM_H
