#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include <qstring.h>

#include <QHBoxLayout>

using namespace std;

class FormBuilder {
 public:
  FormBuilder();
  //~FormBuilder();

  QVBoxLayout *RangeBuilder();

  void loadComPortData();
  QVBoxLayout *generateComColumn(int index);
  QHBoxLayout *generateComBlock();
  QVBoxLayout *generateRange(QString header);

 private:
  QStringList rangeHeaders = {"Flaps", "Mixture 1", "Mixture 2", "Propeller 1",
                              "Proppeller 2"};
  QStringList comHeaders = {"Inputs", "Outputs", "Both"};
  QList<QString> availableComPorts;
};

#endif  // FORMBUILDER_H
