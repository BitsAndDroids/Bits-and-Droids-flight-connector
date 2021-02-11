#include "headers/Set.h"

#include <cstdio>
#include <future>
#include <iostream>
#include <string>
void Set::createSet() {
  settings->beginGroup("Sets");
  settings->setValue("amntOfSets", amntSets);
  QStringList sets = settings->value("setNameList").toStringList();
  sets.append(name);
  settings->setValue("setNameList", sets);
  settings->beginGroup(QString::number(id));
  if (newSet) {
    if (name.length() > 0) {
      QString setId = "set" + QString::number(amntSets);
      settings->setValue(setId, name);
    }
    settings->setValue("checkboxes", this->checkboxes);
  }
  settings->endGroup();
  settings->endGroup();
  settings->sync();
}
void Set::updateSet() {}
void Set::addCheckBox(QString cb) {
  checkboxes.append(cb);
  std::cout << cb.toStdString() << std::endl;
}
