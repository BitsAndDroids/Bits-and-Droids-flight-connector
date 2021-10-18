#include <outputs/set.h>

#include <QApplication>

#include "headers/mainwindow.h"

const char* portName;

int main(int argc, char* argv[])

{
  QApplication a(argc, argv);

  a.setApplicationName("BitsAndDroidsFlightConnector");
  a.setOrganizationName("Bits and Droids");
  a.setOrganizationDomain("www.bitsanddroids.com");
  MainWindow w;

  w.show();

  return a.exec();
}
