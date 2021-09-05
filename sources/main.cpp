#include <QApplication>
#include <outputs/set.h>

#include "headers/mainwindow.h"
Q_DECLARE_METATYPE( QList<coordinates>*)
const char* portName;
#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &stream, const QList<coordinates>*  &object)
{
    for (coordinates i: *object){
        stream<<"y"<<i.getY()<<"x"<<i.getX();
    }
    return stream;
}
QDataStream &operator>>(QDataStream &stream, QList<coordinates>*  &object)
{
    return stream;
}
#endif
int main(int argc, char* argv[])

{
  QApplication a(argc, argv);
  a.setApplicationName("BitsAndDroidsFlightConnector");
  a.setOrganizationName("Bits and Droids");
  a.setOrganizationDomain("www.bitsanddroids.com");
  MainWindow w;
  qRegisterMetaType<QList<coordinates>*>("QList<coordinates>*");
  qRegisterMetaTypeStreamOperators<QList<coordinates>*>("QList<coordinates>*");


  w.show();

  return a.exec();
}
