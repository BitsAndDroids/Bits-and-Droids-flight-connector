#include "sethandler.h"
#include "logging/MessageCaster.h"

#include <QJsonDocument>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>

SetHandler::SetHandler() { setList = loadSets(); }

Set *SetHandler::saveSet(Set *setToSave) {
  int counter;
  QString key;
  if (setToSave->getID() != -1) {
    key = QString::number(setToSave->getID());
  } else {
    if (settingsHandler.getSettingValue("setKeys", "lastId")->isNull()) {
      counter = 0;
    } else {
      counter =
          settingsHandler.getSettingValue("setKeys", "lastId")->toInt() + 1;
    }
    setToSave->setSetId(counter);
    settingsHandler.storeSettingValue("setKeys", "lastId", setToSave->getID());
    key = QString::number(counter);
  }
  QVariant jsonVariant = *setToJSON(setToSave);
  settingsHandler.storeSettingValue("sets", key, jsonVariant);
  setList = loadSets();
  return setToSave;
}

QList<Set> *SetHandler::loadSets() {

  auto *setListFound = new QList<Set>();
  QStringList *keys = settingsHandler.retrieveKeys("sets");
  for (const auto &key : *keys) {
    QVariant *varFound = settingsHandler.getSettingValue("sets", key);
    QJsonDocument foundDoc = varFound->toJsonDocument();
    QJsonObject foundObj = foundDoc.object();
    Set savedSet = fromJson(&foundDoc);
    setListFound->append(savedSet);
  }
  return setListFound;
}

void SetHandler::updateSets() {
    setList->clear();
    setList = loadSets();
    for (auto &setFound : *setList) {
        auto outputMap = QMap<int, Output *>();
        for (auto &output : setFound.getOutputs()) {
            auto outputChecked = outputHandler->findOutputById(output->getId());
            if (outputChecked->getId() != -1) {
                outputMap.insert(outputChecked->getId(), outputChecked);
            }
        }
        setFound.setOutputs(outputMap);
  outputHandler->readOutputs();
    saveSet(&setFound);
  }
}

Set SetHandler::getSetById(QString id) {
  Set setFound;
  QJsonDocument foundSetJson =
      settingsHandler.getSettingValue("sets", id)->toJsonDocument();
  setFound = fromJson(&foundSetJson);
  return setFound;
}

Set SetHandler::fromJson(QJsonDocument *docToConvert) {
  Set convertedSet;
  auto *outputsConverted = new QMap<int, Output *>();
  QJsonObject objToConvert = docToConvert->object();
  convertedSet.setSetId(objToConvert.value("setId").toInt());
  convertedSet.setSetName(objToConvert.value("setName").toString());

  QJsonArray outputJSONArray = objToConvert.value("outputs").toArray();

  for (auto value : outputJSONArray) {
    QJsonObject tempObj = value.toObject();

    auto *foundOutput = new Output(
        tempObj.value("id").toInt(),
        tempObj.value("outputName").toString().toStdString(),
        tempObj.value("metric").toString().toStdString(),
        tempObj.value("updateEvery").toDouble(),
        tempObj.value("dataType").toInt(), tempObj.value("cbText").toString(),
        tempObj.value("prefix").toInt(), tempObj.value("type").toInt());
    foundOutput->setOffset(tempObj.value("offset").toInt());

    if (!outputHandler->getAvailableOutputs().empty()) {
      if (outputHandler->findOutputById(foundOutput->getId())->getId() != -1) {
        outputsConverted->insert(foundOutput->getId(), foundOutput);

      } else {
          //TODO let user choose whether to delete from the set or add the output to the event file
         MessageCaster::showWarningMessage("Could not import set, output " + QString::fromStdString(foundOutput->getOutputName()) + " not found\n" +
         "It will be added to your event file");
         outputHandler->addToEventFileDialog(*foundOutput);
         outputsConverted->insert(foundOutput->getId(), foundOutput);
      }
    }
  }
  //TODO add event to event file
  convertedSet.setOutputs(*outputsConverted);
  return convertedSet;
}

QJsonDocument* SetHandler::setToJSON(Set *setToConvert) {
    auto *document = new QJsonDocument();

    QMap<int, Output *> outputs = setToConvert->getOutputs();
    QJsonArray jsonArray;
    QMap<int, Output *>::iterator i;
    for (i = outputs.begin(); i != outputs.end(); i++) {
        jsonArray.append(i.value()->toJson());
    }
    QJsonObject object{{"setName", setToConvert->getSetName()},
                       {"setId", setToConvert->getID()},
                       {"outputs", jsonArray}};
    qDebug() << "OBJECT TO SAVE" << object;
    document->setObject(object);
    return document;
}

void SetHandler::removeOutputFromSet(int setId, int outputId) {
  Set setFound = getSetById(QString::number(setId));
  setFound.removeOutput(outputId);
  saveSet(&setFound);
}
void SetHandler::removeSet(QString id) {
  settingsHandler.removeSetting("sets", id);
  setList->clear();
  setList = loadSets();
}

void SetHandler::exportSet(const QString& id, const QString &fileName) {
    if(fileName.isEmpty()) {
        MessageCaster::showWarningMessage("Could not export set");
        return;
    }
    auto setToExport = getSetById(id);
    QJsonDocument *jsonDoc = setToJSON(&setToExport);
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(jsonDoc->toJson());
    file.close();

}

int SetHandler::importSet(const QString& path, const QString& name) {
    if(path.isEmpty()) {
        MessageCaster::showWarningMessage("Could not import set");
        return 0;
    }
    QFile importFile = QFile(path);
    importFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString importFileContent = importFile.readAll();

    auto importedJSON = QJsonDocument::fromJson(importFileContent.toUtf8());
    auto importedSet = fromJson(&importedJSON);
    //We create a copy to ensure the set ID doesn't collide when exchanging output sets
    auto importedSetCopy = new Set(importedSet.getSetName());
    importedSetCopy->setOutputs(importedSet.getOutputs());
    importedSetCopy->setSetName(name);

    saveSet(importedSetCopy);
    MessageCaster::showCompleteMessage("Imported set " + importedSetCopy->getSetName());

    return importedSetCopy->getID();
}
