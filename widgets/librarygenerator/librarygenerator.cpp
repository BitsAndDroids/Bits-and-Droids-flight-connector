#include "librarygenerator.h"

#include "models/events/Event.h"

#include <QFile>
#include <sstream>
libraryGenerator::libraryGenerator() = default;
void libraryGenerator::writeFile() {
  //  const QFile libraryTextPrefix("libraryTextPrefix.txt");
  //  const QFile libraryTextPostfix("libraryTextPostfix.txt");
  //  auto *events = new QList<Event>;
  //  std::string customLibraryStr;
  //  for (auto &event : *events) {
  //    std::string customEventStr;
  //    customEventStr = "case:" + std::to_string(event.getPrefix()) + "{";
  //    std::string commentStripped = event.getComment();
  //    commentStripped.erase(remove_if(event.getComment().begin(),
  //                                    event.getComment().end(), isspace),
  //                          event.getComment().end());
  //    customEventStr += commentStripped;
  //    customEventStr += "cutValue.";
  //    std::string libraryParseStr = EventDatatype::getTypeLibraryParse()
  //                                      .at(event.getType().getTypeIndex())
  //                                      .toStdString();
  //  }
}
