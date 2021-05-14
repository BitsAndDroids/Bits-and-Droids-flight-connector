#ifndef FORMBUILDER_H
#define FORMBUILDER_H

#include <qstring.h>

#include <iterator>
#include <map>

using namespace std;

class FormBuilder {
 public:
  FormBuilder();
  ~FormBuilder();
  map<pair<QString, QString>, bool> lights;

 private:
};

#endif  // FORMBUILDER_H
