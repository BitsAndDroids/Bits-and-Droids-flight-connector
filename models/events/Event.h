//
// Created by dave- on 3/30/2022.
//
#include "EventDatatype.h"
#include "EventType.h"
#ifndef BITSANDDROIDSGUI_EVENT_H
#define BITSANDDROIDSGUI_EVENT_H
class Event {
 private:
  int prefix;
  std::string event;
  EventType type;
  EventDatatype datatype;
  float updateEvery;
  std::string comment;

 public:
  Event(){};
  int getPrefix() const { return prefix; }

  void setPrefix(int prefix) { Event::prefix = prefix; }

  const std::string getEvent() const { return event; }

  void setEvent(const std::string &event) { Event::event = event; }

  EventType getType() const { return type; }

  void setType(EventType type) { Event::type = type; }

  EventDatatype getDatatype() const { return datatype; }

  void setDatatype(EventDatatype datatype) { Event::datatype = datatype; }

  float getUpdateEvery() const { return updateEvery; }

  void setUpdateEvery(float updateEvery) { this->updateEvery = updateEvery; }

  const std::string getComment() const { return comment; }

  void setComment(const std::string &comment) { Event::comment = comment; }
};

#endif  // BITSANDDROIDSGUI_EVENT_H
