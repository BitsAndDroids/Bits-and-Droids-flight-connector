#include "sethandler.h"
#include <outputs/output.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <qfile.h>
#include<iostream>
SetHandler::SetHandler()
{

}

void SetHandler::saveSet(set setToSave)
{



    settings->beginGroup("sets");
    settings->beginGroup("keys");
    int counter;
    bool foundKey;
    QString key;
    if(setToSave.getID() != NULL){
         foundKey = true;
         key = QString::number(setToSave.getID());
    }
    else{
        counter = settings->value("lastId").toInt() + 1;
        setToSave.setSetId(counter);
        settings->setValue("lastId", setToSave.getID());
        key = QString::number(counter);
    }



    settings->endGroup();

    QJsonDocument *document = new QJsonDocument();

    QMap<int, Output*> outputs = setToSave.getOutputs();
    QJsonArray jsonArray;
    QMap<int, Output*>::iterator i;
    for (i = outputs.begin(); i != outputs.end(); i ++){
        jsonArray.append(i.value()->toJson());
    }
    QJsonObject object{{
                     "setId", setToSave.getID()
                  },{
                      "setName", setToSave.getSetName()
                       },{
            "outputs", jsonArray
                       }};

    document->setObject(object);
    QVariant jsonVariant = *document;
    settings->setValue(key, jsonVariant);
    settings->endGroup();
    //settings->sync();

}

QList<set>* SetHandler::getSets()
{
 QList<QJsonDocument> documentList;
 QList<set>* setList = new QList<set>();


  settings->beginGroup("sets");

  QStringList keys = settings->childKeys();
  for(int i = 0; i < keys.size(); i++){

       QVariant varFound = settings->value(keys[i]);
       QJsonDocument foundDoc = varFound.toJsonDocument();
       QJsonObject foundObj = foundDoc.object();
       set savedSet = fromJson(&foundDoc);
       setList->append(savedSet);
       documentList.append(foundDoc);
       qDebug()<<varFound <<"VARFOUND";
       qDebug()<<foundDoc;

  }


  settings->endGroup();
  settings->sync();

  return setList;
}
set SetHandler::getSetById(QString id){
    set setFound;
    settings->beginGroup("sets");
    QJsonDocument foundSetJson =settings->value(id).toJsonDocument();
    settings->endGroup();
    setFound = fromJson(&foundSetJson);
    return setFound;
}
set SetHandler::fromJson(QJsonDocument *docToConvert)
{
    set convertedSet;
    QMap<int, Output*> *outputsConverted = new QMap<int, Output*>();
    QJsonObject objToConvert = docToConvert->object();
    convertedSet.setSetId(objToConvert.value("setId").toInt());
    convertedSet.setSetName(objToConvert.value("setName").toString());

    QJsonArray outputJSONArray = objToConvert.value("outputs").toArray();
    qDebug()<<"sizeofjsonaarray"<<outputJSONArray.size();
    foreach(const QJsonValue & value, outputJSONArray){
        QJsonObject tempObj = value.toObject();

        Output *foundOutput = new Output(
                    tempObj.value("id").toInt(),
                    tempObj.value("outputName").toString().toStdString(),
                    tempObj.value("metric").toString().toStdString(),
                    tempObj.value("updateEvery").toDouble(),
                    tempObj.value("dataType").toInt(),
                    tempObj.value("cbText").toString()
                    );
        qDebug()<<"foundOutput"<<foundOutput->getCbText();
        outputsConverted->insert(foundOutput->getId(),foundOutput);
    }
    convertedSet.setOutputs(*outputsConverted);
    qDebug()<<"sizeOut "<<convertedSet.getOutputs().count()<<"found "<<outputsConverted->count();
    return convertedSet;
}

void SetHandler::removeSet(QString id){
    settings->beginGroup("sets");
    settings->remove(id);
    settings->endGroup();
}


