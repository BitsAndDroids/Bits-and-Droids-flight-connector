//
// Created by Dave Riedel on 31-Mar-22.
//

#ifndef BITSANDDROIDSGUI_EVENTTYPE_H
#define BITSANDDROIDSGUI_EVENTTYPE_H

#include <QStringList>

class EventType {
 private:
  static QStringList availableTypes;
  static QList<int> eventCodes;
  std::string typeString;
  int typeIndex;

 public:
  EventType(){};
  explicit EventType(int index);

  const std::string &getTypeString() const { return typeString; }

  void setTypeString(const std::string &typeString) {
    EventType::typeString = typeString;
  }

  int getTypeIndex() const { return typeIndex; }

  void setTypeIndex(int typeIndex) { EventType::typeIndex = typeIndex; }

  static QStringList getAvailableTypes() { return availableTypes; }
  static QList<int> getEventCodes() { return eventCodes; }
};

#endif  // BITSANDDROIDSGUI_EVENTTYPE_H
