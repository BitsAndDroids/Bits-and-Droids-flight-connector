#include <QApplication>

#include "headers/mainwindow.h"


int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QApplication::setApplicationName("BitsAndDroidsFlightConnector");
  QApplication::setOrganizationName("Bits and Droids");
  QApplication::setOrganizationDomain("www.bitsanddroids.com");

  MainWindow w;
  w.show();

  return QApplication::exec();
}
