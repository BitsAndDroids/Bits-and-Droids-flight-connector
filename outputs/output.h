#ifndef OUTPUT_H
#define OUTPUT_H

#include <qstring.h>

#include <QJsonObject>

class Output

{
 public:
  int test;
  Output(int id, std::string outputName, std::string metric, float updateEvery,
         int dataType, QString cbText, int prefix, int type);
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
  std::string outputName;
  std::string metric;
  float updateEvery;
  int dataType;
  QString cbText;
  int prefix;
  int type;
};

#endif  // OUTPUT_H
