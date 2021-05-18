#include "headers/formbuilder.h"

#include <headers/settingsranges.h>

#include <iostream>
using namespace std;
FormBuilder::FormBuilder() {
  //    lights[make_pair(<QString, QString,bool>(("Test","Test"),true));)
}

QVBoxLayout* FormBuilder::RangeBuilder() {
  QVBoxLayout* rangeHLayout = new QVBoxLayout();

  QVBoxLayout* engineRanges = new QVBoxLayout();
  engineRanges->setObjectName("Engine ranges");
  QStringList engineLabels = {"Reverse", "Idle cutoff", "Max"};
  QStringList headers = {"Engine 1", "Engine 2", "Engine 3", "Engine 4"};
  for (int i = 0; i < headers.size(); i++) {
    cout << "test " << engineLabels.size() << endl;
    SettingsRanges* engineRange =
        new SettingsRanges(engineLabels.size(), engineLabels, headers[i]);
    QVBoxLayout* layout = new QVBoxLayout();
    layout = engineRange->CreateRangeRow();
    engineRanges->addLayout(layout);
  }
  rangeHLayout->addLayout(engineRanges);
  rangeHLayout->setContentsMargins(0, 0, 0, 0);
  return rangeHLayout;
}
