#include "outputhandler.h"

#include "logging/MessageCaster.h"
#include "utils/EventFileFormatter.h"
#include "models/Category.h"
#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <utility>

bool OutputHandler::updateOutputsRequired = true;
QMap<int, Output *> OutputHandler::availableOutputs = QMap<int, Output *>();
std::vector<Category> OutputHandler::outputsCategorized = std::vector<Category>();
std::vector<QString> OutputHandler::categoryStrings = std::vector<QString>();

OutputHandler::OutputHandler() {
    if (updateOutputsRequired) {
        readOutputs();
        updateOutputsRequired = false;
    }
}

void OutputHandler::addCategoryString(const QString &category) {
    categoryStrings.emplace_back(category);
}

void OutputHandler::readOutputs() {
    OutputHandler::availableOutputs.clear();
    OutputHandler::outputsCategorized.clear();
    QFile file_obj(qApp->applicationDirPath() + "/outputs.json");
    file_obj.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_bytes = file_obj.readAll();
    file_obj.close();

    QJsonParseError error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes, &error);

    if (error.error != QJsonParseError::NoError) {
        for (int i = error.offset - 10; i < error.offset + 30; i++) {

        }
        return;
    }

    if (!json_doc.isObject()) {
        qDebug() << "Document is not an object";
        return;
    }

    QJsonObject outputJSON = json_doc.object();
    for(auto &cat: outputJSON.keys()) {
        categoryStrings.emplace_back(cat);
    }
    categoryStrings.emplace_back("Custom");
    std::cout << "SIZE WUT " << categoryStrings.size() << std::endl;
    for (auto &cat: categoryStrings) {
        std::cout << "STRING F" << cat.toStdString() << std::endl;
    }
    for (int i = 0; i < categoryStrings.size() - 1; i++) {
        auto *outputCategory = new std::vector<Output>;
        QJsonArray array = outputJSON.value(categoryStrings[i]).toArray();
                foreach (const QJsonValue &v, array) {
                QJsonObject obj = v.toObject();

                int JSONid = obj.value("id").toInt();
                std::string JSONoutputName =
                        obj.value("outputName").toString().toStdString();
                std::string JSONmetric = obj.value("metric").toString().toStdString();
                float JSONupdateEvery = (float) obj.value("updateEvery").toDouble();
                int JSONdataType = obj.value("dataType").toInt();
                QString JSONcbText = obj.value("cbText").toString();
                int prefix = obj.value("prefix").toInt();
                int type = obj.value("type").toInt();

                // TODO I want to migrate to a dynamic ID system challenge is to preserve old sets

                auto *foundOutput =
                        new Output(JSONid, JSONoutputName, JSONmetric, JSONupdateEvery,
                                   JSONdataType, JSONcbText, prefix, type);

                //Swapping variables triggers commands too fast,
                //so we need to use a delay to accomodate our microcontrollers
                if(foundOutput->getOutputName().find("SWAP") != std::string::npos) {
                    foundOutput->setDelay(1);
                }

                outputCategory->emplace_back(*foundOutput);
                availableOutputs.insert(foundOutput->getId(), foundOutput);

                QJsonValue score = obj.value("updateEvery");
            }
            Category category(categoryStrings[i].toStdString(), *outputCategory);
            outputsCategorized.emplace_back(category);
    }
    auto *eventOutputs = new std::vector<Output>();

    std::ifstream file(applicationEventsPath.toStdString());
    std::string row;
    int offsetCounter = 0;
    int index = 0;
    while (std::getline(file, row)) {
        try {
            index++;
            if (row.size() > 25 && row.at(0) != '/') {
                int modeDelimiter = (int) row.find('^');
                int prefixDelimiter = (int) row.find('#');

                int updateEveryDelimiter = (int) row.find('$');
                int commentDelimiter = (int) row.find("//");
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
                    newRow->setOffset((int) sizeof(float) * offsetCounter);

                    newRow->setCbText(
                            QString::fromStdString(row.substr(commentDelimiter + 2)));

                    availableOutputs.insert(newRow->getId(), newRow);
                    offsetCounter++;
                    Category category("Custom", *eventOutputs);
                    eventOutputs->emplace_back(*newRow);
                }
            }
        } catch (const std::exception &e) {
            //#TODO write to logger
            auto errorString = "Event file error in line " + QString::number(index) +
                               "\n" + QString::fromStdString(row) + "\nReason(" +
                               e.what() + " failed)";
            MessageCaster::showWarningMessage(errorString);
        }
    }
    Category eventCategory("Custom", *eventOutputs);
    outputsCategorized.emplace_back(eventCategory);
    file.close();
}

Output *OutputHandler::findOutputById(int idToFind) {
    if (!availableOutputs.contains(idToFind)) {
        auto emptyOutput =
                new Output(-1, "empty", "none", 0.0, -1, "empty", -1, -1);
        return emptyOutput;
    } else {
        if(availableOutputs.find(idToFind).value() == nullptr) {
            std::string delayFilters[] = {"COM", "NAV"};
            for (auto &filter: delayFilters) {
                if (availableOutputs[idToFind]->getOutputName().find(filter) != std::string::npos) {
                    availableOutputs[idToFind]->setDelay(10);
                }
            }
            return availableOutputs.find(idToFind).value();

        }
        return availableOutputs[idToFind];
    }
}

void OutputHandler::addToEventFileDialog(Output output) {
    QFile newEventsFile(applicationEventsPath);
    newEventsFile.open(QIODevice::Append);
    EventFileFormatter formatter = EventFileFormatter();
    QString row =formatter.outputToQString(std::move(output));
    newEventsFile.write(row.toStdString().c_str(), row.size());
    newEventsFile.close();
    availableOutputs.insert(output.getId(), &output);
}
