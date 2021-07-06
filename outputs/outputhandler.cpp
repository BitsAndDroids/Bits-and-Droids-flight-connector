#include "outputhandler.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonArray>

outputHandler::outputHandler()
{
    readOutputs();
}
//const QJsonObject &json

void outputHandler::readOutputs()
{

    QFile file_obj("release/outputs.json");
    file_obj.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_bytes = file_obj.readAll();
    file_obj.close();

    QJsonParseError error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes, &error);



    if (error.error != QJsonParseError::NoError)
    {
        qDebug().noquote() << json_bytes[0];
        qDebug() << "Parse error: " << error.errorString();
        for(int i = error.offset - 10; i < error.offset + 30; i++){
            qDebug().noquote()<<json_bytes[i];
        }
        return;
    }

    if (!json_doc.isObject())
    {
        qDebug() << "Document is not an object";
        return;
    }


    QJsonObject outputJSON = json_doc.object();
    categoryStrings = outputJSON.keys();
    qDebug()<<categoryStrings;
    foreach(const QString & v, categoryStrings){
        QList<Output> *outputCategory = new QList<Output>;
        QJsonArray array = outputJSON.value(v).toArray();
        foreach (const QJsonValue & v, array)
            {
                QJsonObject obj = v.toObject();

                int JSONid = obj.value("id").toInt();
                std::string JSONoutputName = obj.value("outputName").toString().toStdString();
                std::string JSONmetric = obj.value("metric").toString().toStdString();
                float JSONupdateEvery = obj.value("updateEvery").toDouble();
                int JSONdataType = obj.value("dataType").toInt();
                QString JSONcbText = obj.value("cbText").toString();

                Output *foundOutput = new Output(JSONid, JSONoutputName, JSONmetric, JSONupdateEvery, JSONdataType, JSONcbText);
                outputCategory->append(*foundOutput);
                availableOutputs.append(*foundOutput);
                qDebug() << obj.value("outputName").toString();
                QJsonValue score = obj.value("updateEvery");
                if (!score.isUndefined())
                    qDebug() << score.toInt();
            }
        outputsCategorized.append(*outputCategory);
   }
    qDebug()<<availableOutputs.size()<<" outputs saved";
}
