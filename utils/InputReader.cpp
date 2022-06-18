#include <QMap>
#include <QJsonParseError>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include "models/commands/Input.h"
#include "InputReader.h"

InputReader::InputReader() {}

void InputReader::readInputs() {
    //inputs.clear();
    std::map<int,Input> foundInputs;
    QFile file_obj("inputs.json");
    file_obj.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_bytes = file_obj.readAll();
    file_obj.close();

    QJsonParseError error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes, &error);

    auto outputJSON = json_doc.object();
    QJsonValue inputArrayValue = outputJSON.value("inputs");
    QJsonArray value = inputArrayValue.toArray();
    foreach (const QJsonValue &value, value) {
        int type = value.toObject().value("type").toInt();
        std::string event = value.toObject().value("event").toString().toStdString();
        int prefix = value.toObject().value("prefix").toInt();

        Input input = Input(type, event, prefix);
        if (prefix != 0) {
            inputs.insert(std::pair<int, Input>(prefix, input));
        }
    }

    //InputReader::inputs = foundInputs;

}

std::map<int, Input> InputReader::getInputs() {
    return inputs;
}


#include "InputReader.h"
