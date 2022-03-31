//
// Created by Dave Riedel on 31-Mar-22.
//

#include "EventDataType.h"
QStringList EventDatatype::typeLibrary={"Int", "Float", "Bool"};
QStringList EventDatatype::typeLibraryParse={"Int", "Float", "Bool"};
QStringList EventDatatype::typeLibraryParseShorthand={"i", "f", "b"};
EventDatatype::EventDatatype(int index) {
    this->datatypeIndex = index;
}
