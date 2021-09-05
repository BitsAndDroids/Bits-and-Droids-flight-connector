#include <QApplication>
#include <outputs/set.h>

#include "headers/mainwindow.h"
Q_DECLARE_METATYPE( QAbstractSeries *)
const char* portName;
#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &stream, const QAbstractSeries * &object)
{
    return stream<<object;
}
QDataStream &operator>>(QDataStream &stream, QAbstractSeries * &object)
{
    stream >> object;
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
  qRegisterMetaType<QList<QAbstractSeries *>>("QAbstractSeries *");
  qRegisterMetaTypeStreamOperators<QList<QAbstractSeries *>>("QAbstractSeries *");


  w.show();

  return a.exec();
}
