#include "sethandler.h"

#include <QJsonDocument>
#include <iostream>

SetHandler::SetHandler() { setList = loadSets(); }

Set *SetHandler::saveSet(Set *setToSave) {
  int counter;
  std::cout << "SET SAVED" << setToSave->getID() << std::endl;
  QString key;
  if (setToSave->getID() != 0) {
    key = QString::number(setToSave->getID());
  } else {
    if (settingsHandler.retrieveSetting("setKeys", "lastId")->isNull()) {
      counter = 0;
    } else {
      counter =
          settingsHandler.retrieveSetting("setKeys", "lastId")->toInt() + 1;
    }
    setToSave->setSetId(counter);
    settingsHandler.storeValue("setKeys", "lastId", setToSave->getID());
    key = QString::number(counter);
  }

  auto *document = new QJsonDocument();

  QMap<int, Output *> outputs = setToSave->getOutputs();
  QJsonArray jsonArray;
  QMap<int, Output *>::iterator i;
  for (i = outputs.begin(); i != outputs.end(); i++) {
    jsonArray.append(i.value()->toJson());
  }
  QJsonObject object{{"setName", setToSave->getSetName()},
                     {"setId", setToSave->getID()},
                     {"outputs", jsonArray}};
  qDebug() << "OBJECT TO SAVE" << object;
  document->setObject(object);
  QVariant jsonVariant = *document;
  settingsHandler.storeValue("sets", key, jsonVariant);
  setList = loadSets();
  return setToSave;
}

QList<Set> *SetHandler::loadSets() {
  QList<QJsonDocument> documentList;
  auto *setListFound = new QList<Set>();

  QStringList *keys = settingsHandler.retrieveKeys("sets");
  std::cout << keys->size() << "keys" << std::endl;
  for (const auto &key : *keys) {
    QVariant *varFound = settingsHandler.retrieveSetting("sets", key);
    QJsonDocument foundDoc = varFound->toJsonDocument();
    QJsonObject foundObj = foundDoc.object();
    Set savedSet = fromJson(&foundDoc);
    setListFound->append(savedSet);

    documentList.append(foundDoc);
    qDebug() << varFound << "VARFOUND";
    qDebug() << foundDoc;
  }

  return setListFound;
}

void SetHandler::updateSets() {
  outputHandler->readOutputs();
  setList = loadSets();
  for (auto &setFound : *setList) {
    auto outputMap = QMap<int, Output *>();
    for (auto &output : setFound.getOutputs()) {
      auto outputChecked = outputHandler->findOutputById(output->getId());
      if (outputChecked->getId() != -1) {
        outputMap.insert(outputChecked->getId(), outputChecked);
        qDebug() << "after";
      }
    }
    setFound.setOutputs(outputMap);
    saveSet(&setFound);
  }
}

Set SetHandler::getSetById(QString id) {
  Set setFound;
  QJsonDocument foundSetJson =
      settingsHandler.retrieveSetting("sets", id)->toJsonDocument();
  setFound = fromJson(&foundSetJson);
  return setFound;
}

Set SetHandler::fromJson(QJsonDocument *docToConvert) {
  Set convertedSet;
  QMap<int, Output *> *outputsConverted = new QMap<int, Output *>();
  QJsonObject objToConvert = docToConvert->object();
  convertedSet.setSetId(objToConvert.value("setId").toInt());
  convertedSet.setSetName(objToConvert.value("setName").toString());

  QJsonArray outputJSONArray = objToConvert.value("outputs").toArray();
  qDebug() << "sizeofjsonaarray" << outputJSONArray.size();
  for (auto value : outputJSONArray) {
    QJsonObject tempObj = value.toObject();

    Output *foundOutput = new Output(
        tempObj.value("id").toInt(),
        tempObj.value("outputName").toString().toStdString(),
        tempObj.value("metric").toString().toStdString(),
        tempObj.value("updateEvery").toDouble(),
        tempObj.value("dataType").toInt(), tempObj.value("cbText").toString(),
        tempObj.value("prefix").toInt(), tempObj.value("type").toInt());
    foundOutput->setOffset(tempObj.value("offset").toInt());
    auto test = outputHandler->getAvailableOutputs();
    if (outputHandler->getAvailableOutputs().size() > 0) {
      if (outputHandler->findOutputById(foundOutput->getType() != -1)) {
        outputsConverted->insert(foundOutput->getId(), foundOutput);
      } else {
        removeOutputFromSet(convertedSet.getID(), foundOutput->getId());
      }
    }
  }
  convertedSet.setOutputs(*outputsConverted);
  return convertedSet;
}
void SetHandler::removeOutputFromSet(int setId, int outputId) {
  Set setFound = getSetById(QString::number(setId));
  std::cout << "remove from Set" << std::endl;
  setFound.removeOutput(outputId);
  saveSet(&setFound);
}
void SetHandler::removeSet(QString id) {
  settingsHandler.removeSetting("sets", id);
  setList->clear();
  setList = loadSets();
}

void SetHandler::exportSet(QString id) {

}
