#ifndef OUTPUT_H
#define OUTPUT_H

#include <qstring.h>

#include <QJsonObject>

class Output

{
 public:
  int test;
  Output();
  Output(int id, std::string outputName, std::string metric, float updateEvery,
         int dataType, QString cbText, int prefix, int type);
  void setPrefix(int prefixToSet) { this->prefix = prefixToSet; };
  void setId(int idToSet) { this->id = idToSet; };
  int getOffset() { return this->offset; };
  void setOffset(int offsetToSet) { offset = offsetToSet; };
  void setOutputName(std::string outputNameToSet) {
    outputName = outputNameToSet;
  };
  void setUpdateEvery(float updateEveryToSet) {
    updateEvery = updateEveryToSet;
  };
  void setType(int typeToSet) { this->type = typeToSet; };
  void setCbText(QString cbTextToSet) { this->cbText = cbTextToSet; };
  std::string getMetric() { return metric; };
  float getUpdateEvery() { return updateEvery; };
  std::string getOutputName() { return outputName; };
  int getId() { return this->id; };
  int getPrefix() { return this->prefix; };
  int getType() { return this->type; };
  QString getCbText() { return cbText; };
  QJsonObject toJson() const;

 private:
  int id;
  int offset;
  std::string outputName;
  std::string metric;
  float updateEvery;
  int dataType;
  QString cbText;
  int prefix;
  int type;
};

#endif  // OUTPUT_H
