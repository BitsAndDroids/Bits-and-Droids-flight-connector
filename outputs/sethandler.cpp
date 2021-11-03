#include "sethandler.h"


#include <QJsonDocument>

#include <iostream>

SetHandler::SetHandler() { setList = loadSets(); }

set *SetHandler::saveSet(set *setToSave) {
    int counter;
    bool foundKey;

    QString key;
    if (setToSave->getID() != 0) {
        foundKey = true;
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
    QJsonObject object{
            {"setName", setToSave->getSetName()},
            {"setId",   setToSave->getID()},
            {"outputs", jsonArray}};
    qDebug()<<"OBJECT TO SAVE"<<object;
    document->setObject(object);
    QVariant jsonVariant = *document;
    settingsHandler.storeValue("sets", key, jsonVariant);
    setList = loadSets();
    return setToSave;
}

QList<set> *SetHandler::loadSets() {
    QList<QJsonDocument> documentList;
    auto *setListFound = new QList<set>();

    QStringList *keys = settingsHandler.retrieveKeys("sets");
    qDebug() << keys->size() << "keys";
    for (const auto & key : *keys) {
        QVariant *varFound = settingsHandler.retrieveSetting("sets", key);
        QJsonDocument foundDoc = varFound->toJsonDocument();
        QJsonObject foundObj = foundDoc.object();
        set savedSet = fromJson(&foundDoc);
        setListFound->append(savedSet);

        documentList.append(foundDoc);
        qDebug() << varFound << "VARFOUND";
        qDebug() << foundDoc;
    }

    return setListFound;
}

void SetHandler::updateSets(){
    outputHandler.readOutputs();
    for(auto & setFound : *setList){
        auto outputMap = QMap<int,Output *>();
        for(auto & output : setFound.getOutputs()){
            auto outputChecked = outputHandler.findOutputById(output->getId());
            outputMap.insert(outputChecked->getId(),outputChecked);
        }
        setFound.setOutputs(outputMap);
        saveSet(&setFound);
    }

}

set SetHandler::getSetById(QString id) {
    set setFound;
    QJsonDocument foundSetJson =
            settingsHandler.retrieveSetting("sets", id)->toJsonDocument();
    setFound = fromJson(&foundSetJson);
    return setFound;
}

set SetHandler::fromJson(QJsonDocument *docToConvert) {
    set convertedSet;
    QMap<int, Output *> *outputsConverted = new QMap<int, Output *>();
    QJsonObject objToConvert = docToConvert->object();
    convertedSet.setSetId(objToConvert.value("setId").toInt());
    convertedSet.setSetName(objToConvert.value("setName").toString());

    QJsonArray outputJSONArray = objToConvert.value("outputs").toArray();
    qDebug() << "sizeofjsonaarray" << outputJSONArray.size();
            foreach (const QJsonValue &value, outputJSONArray) {
            QJsonObject tempObj = value.toObject();

            Output *foundOutput = new Output(
                    tempObj.value("id").toInt(),
                    tempObj.value("outputName").toString().toStdString(),
                    tempObj.value("metric").toString().toStdString(),
                    tempObj.value("updateEvery").toDouble(),
                    tempObj.value("dataType").toInt(), tempObj.value("cbText").toString(),
                    tempObj.value("prefix").toInt(), tempObj.value("type").toInt());
            qDebug() << "foundOutput" << foundOutput->getCbText();
            outputsConverted->insert(foundOutput->getId(), foundOutput);
        }
    convertedSet.setOutputs(*outputsConverted);
    qDebug() << "sizeOut " << convertedSet.getOutputs().count() << "found "
             << outputsConverted->count();
    return convertedSet;
}

void SetHandler::removeSet(QString id) {
    settingsHandler.removeSetting("sets", id);
    setList = loadSets();
}
