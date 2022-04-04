#include "outputhandler.h"

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonParseError>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <logging/MessageCaster.h>
bool outputHandler::updateOutputsRequired = true;
outputHandler::outputHandler() {
    if(updateOutputsRequired){
    readOutputs();
    updateOutputsRequired = false;
    }
}

void outputHandler::addCategoryString(const QString& category) {
    categoryStrings.append(category);
}
// const QJsonObject &json

void outputHandler::readOutputs() {
    availableOutputs.clear();
    outputsCategorized.clear();
    categoryStrings.clear();
    QFile file_obj("outputs.json");
    file_obj.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_bytes = file_obj.readAll();
    file_obj.close();

    QJsonParseError error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug().noquote() << json_bytes[0];
        qDebug() << "Parse error: " << error.errorString();
        for (int i = error.offset - 10; i < error.offset + 30; i++) {
            qDebug().noquote() << json_bytes[i];
        }
        return;
    }

    if (!json_doc.isObject()) {
        qDebug() << "Document is not an object";
        return;
    }

    QJsonObject outputJSON = json_doc.object();
    categoryStrings = outputJSON.keys();
    categoryStrings.append("Custom");
    std::cout << "SIZE WUT " << categoryStrings.size() << std::endl;
    for (auto &cat: categoryStrings) {
        std::cout << "STRING F" << cat.toStdString() << std::endl;
    }
    for (int i = 0; i < categoryStrings.size() - 1; i++) {
        auto *outputCategory = new QList<Output>;
        QJsonArray array = outputJSON.value(categoryStrings[i]).toArray();
                foreach (const QJsonValue &v, array) {
                QJsonObject obj = v.toObject();

                int JSONid = obj.value("id").toInt();
                std::string JSONoutputName =
                        obj.value("outputName").toString().toStdString();
                std::string JSONmetric = obj.value("metric").toString().toStdString();
                float JSONupdateEvery = (float)obj.value("updateEvery").toDouble();
                int JSONdataType = obj.value("dataType").toInt();
                QString JSONcbText = obj.value("cbText").toString();
                int prefix = obj.value("prefix").toInt();
                int type = obj.value("type").toInt();

                // TODO I want to migrate to a dynamic ID system
                // Challenge is to preserve old sets

                //      std::string idFormat =
                //          std::to_string(i * 10) +
                //          std::to_string(outputCategory->size());
                //      std::cout << "ID = " << stoi(idFormat) << std::endl;
                auto *foundOutput =
                        new Output(JSONid, JSONoutputName, JSONmetric, JSONupdateEvery,
                                   JSONdataType, JSONcbText, prefix, type);
                outputCategory->append(*foundOutput);
                availableOutputs.insert(foundOutput->getId(), foundOutput);

                QJsonValue score = obj.value("updateEvery");
            }
        outputsCategorized.append(*outputCategory);
    }
    auto *outputCategory = new QList<Output>();

    std::ifstream file(applicationEventsPath.toStdString());
    std::string row;
    int offsetCounter = 0;
    int index = 0;
    while (std::getline(file, row)) {
        try {
            index++;
            if (row.size() > 25 && row.at(0) != '/') {
                int modeDelimiter = (int)row.find('^');
                int prefixDelimiter = (int)row.find('#');

                int updateEveryDelimiter = (int)row.find('$');
                int commentDelimiter =(int) row.find("//");
                if (row.front() == ' ') {
                    row.erase(0, 1);
                }

                std::string type = row.substr(modeDelimiter + 1, 1);
                if (type == "3") {
                    int prefixId = stoi(row.substr(prefixDelimiter + 1, 4));
                    auto *newRow = new Output();
                    // To accomodate old event system
                    newRow->setId(prefixId);
                    newRow->setPrefix(prefixId);
                    std::string datatype = row.substr(modeDelimiter + 2, 1);
                    if (datatype == "f") {
                        newRow->setType(97);
                    } else if (datatype == "i") {
                        newRow->setType(98);
                    } else if (datatype == "b") {
                        newRow->setType(99);
                    }

                    newRow->setOutputName(row.substr(0, modeDelimiter));
                    newRow->setUpdateEvery(
                            stof(row.substr(updateEveryDelimiter + 1,
                                            commentDelimiter - updateEveryDelimiter - 1)));
                    newRow->setOffset((int)sizeof(float) * offsetCounter);

                    newRow->setCbText(
                            QString::fromStdString(row.substr(commentDelimiter + 2)));

                    availableOutputs.insert(newRow->getId(), newRow);
                    offsetCounter++;
                    outputCategory->append(*newRow);
                }
            }
        }
        catch (const std::exception& e){
            //#TODO write to logger
            auto errorString = "Event file error in line " + QString::number(index) + "\n" + QString::fromStdString(row) +"\nReason(" + e.what() + " failed)";
            MessageCaster::showWarningMessage(errorString);
        }
    }
    outputsCategorized.append(*outputCategory);
    file.close();
    qDebug() << availableOutputs.size()
             << " outputs saved in sets:" << outputsCategorized.size()
             << " AVAILABLE " << availableOutputs.size();
}

Output *outputHandler::findOutputById(int idToFind) {
    qDebug() << "SEARCHING FOR " << idToFind;
    // qDebug() << availableOutputs[idToFind] << "FOUND";
    if (!availableOutputs.contains(idToFind)) {
        auto emptyOutput =
                new Output(-1, "empty", "none", 0.0, -1, "empty", -1, -1);
        return emptyOutput;
    } else {
        return availableOutputs[idToFind];
    }
}
