#include <QFile>
#include <sstream>
#include "librarygenerator.h"

libraryGenerator::libraryGenerator()
{

}
void libraryGenerator::writeFile(){
    const QFile libraryTextPrefix("libraryTextPrefix.txt");
    const QFile libraryTextPostfix("libraryTextPostfix.txt");
    QList<QString> *events = new QList<QString>;
    std::stringstream customLibrarySS;
    for(auto & event : *events){
        customLibrarySS<<"case:"<<event.prefix<<"{";
        ss<<remove(event.comment.begin(), event.comment.end(), " ").uppercase<<"=";
        ss<<"cutValue.";
        switch(events.type){
            case 0: ss<<"toInt(); break;}";break;
            case 1: ss<<"toFloat(); break;}";break;
            case 2: ss<<"convBool(); break;}";break;
            case 3: ;break;
            case 4: ;break;
            case 5: ;break;
            case 6: ;break;
        }
    }
}