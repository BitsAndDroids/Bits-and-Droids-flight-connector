//
// Created by Dave Riedel on 31-Mar-22.
//

#ifndef BITSANDDROIDSGUI_EVENTDATATYPE_H
#define BITSANDDROIDSGUI_EVENTDATATYPE_H

#include <QStringList>

class EventDatatype {
 private:
  static QStringList typeLibrary;
  static QStringList typeLibraryParse;
  static QStringList typeLibraryParseShorthand;
  int datatypeIndex;

 public:
  EventDatatype(){};
  EventDatatype(int index);
  enum types { INT, FLOAT, BOOL };
  static QStringList getTypeLibraryParse() { return typeLibraryParse; }
  static QStringList getTypeLibraryParseShorthand() {
    return typeLibraryParseShorthand;
  }
  static QStringList getTypeLibrary() { return typeLibrary; }
  int getDatatypeIndex() { return datatypeIndex; }
};

#endif  // BITSANDDROIDSGUI_EVENTDATATYPE_H
