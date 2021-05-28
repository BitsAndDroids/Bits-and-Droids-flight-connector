#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include <qstring.h>

#include <QHBoxLayout>

using namespace std;

class FormBuilder {
 public:
  FormBuilder();
  //~FormBuilder();

  QVBoxLayout* RangeBuilder();

  void loadComPortData();
  QVBoxLayout *generateComColumn(int index);
  QHBoxLayout *generateComBlock();
private:
  QStringList comHeaders = {"Inputs","Outputs","Both"};
  QList<QString> availableComPorts;
};

#endif  // FORMBUILDER_H
