#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include <qstring.h>

#include <QHBoxLayout>
#include <qtabwidget.h>

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

  QStringList comHeaders = {"Inputs", "Outputs", "Both"};
  QStringList rangeHeaders;
  QStringList engineLabels = {"Reverse", "Idle cutoff", "Max"};
  QStringList engineHeaders;

  int getAmountOfEngines() { return engineHeaders.size(); };

  QTabWidget *generateOutputTabs();
private:
  QList<QString> availableComPorts;
};

#endif  // FORMBUILDER_H
