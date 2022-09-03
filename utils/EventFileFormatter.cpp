//
// Created by DaveRiedel on 27-8-2022.
//

#include "EventFileFormatter.h"
#include "models/events/EventDatatype.h"

QString EventFileFormatter::outputToQString(Output output) {
    QString string = "";
    QString typeString = "i";
    QString formattedEvent = QString::fromStdString(output.getOutputName());
    string= "\n" + formattedEvent + "^3" + typeString
        + "#" + QString::number(output.getPrefix()) + "$" + QString::number(output.getUpdateEvery())
        + "//" + output.getCbText() ;
    return string;
}

EventFileFormatter::EventFileFormatter() {

}
