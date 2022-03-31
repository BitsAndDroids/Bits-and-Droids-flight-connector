//
// Created by dave- on 3/30/2022.
//

#ifndef BITSANDDROIDSGUI_EVENT_H
#define BITSANDDROIDSGUI_EVENT_H
class Event{

public:
    Event();
    int getPrefix() const;

    void setPrefix(int prefix);

    const std::string &getEvent() const;

    void setEvent(const std::string &event);

    int getType() const;

    void setType(int type);

    std::string getDatatype() const;

    void setDatatype(std::string datatype);

    float getUpdateEvery() const;

    void setUpdateEvery(float updateEvery);

    const std::string &getComment() const;

    void setComment(const std::string &comment);

private:
    int prefix;
    std::string event;
    int type;
    std::string datatype;
    float updateEvery;
    std::string comment;
};

Event::Event() {

}

int Event::getPrefix() const {
    return prefix;
}

void Event::setPrefix(int prefix) {
    Event::prefix = prefix;
}

const std::string &Event::getEvent() const {
    return event;
}

void Event::setEvent(const std::string &event) {
    Event::event = event;
}

int Event::getType() const {
    return type;
}

void Event::setType(int type) {
    Event::type = type;
}

std::string Event::getDatatype() const {
    return datatype;
}

void Event::setDatatype(std::string datatype) {
    Event::datatype = datatype;
}

float Event::getUpdateEvery() const {
    return updateEvery;
}

void Event::setUpdateEvery(float updateEvery) {
    Event::updateEvery = updateEvery;
}

const std::string &Event::getComment() const {
    return comment;
}

void Event::setComment(const std::string &comment) {
    Event::comment = comment;
}

#endif //BITSANDDROIDSGUI_EVENT_H
